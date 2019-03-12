#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/init.h>
#include <linux/semaphore.h>
#include <linux/device.h>


#include <linux/types.h>
#include <linux/major.h>
#include <linux/errno.h>
#include <linux/signal.h>
#include <linux/fcntl.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/tty.h>
#include <linux/timer.h>
#include <linux/ctype.h>
#include <linux/mm.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/poll.h>
#include <linux/bitops.h>
#include <linux/audit.h>
#include <linux/file.h>
#include <linux/uaccess.h>
#include <linux/module.h>
#include <linux/ratelimit.h>
#include <linux/vmalloc.h>

#include <linux/console.h>
#include <linux/platform_device.h>
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/serial_core.h>
#include <linux/serial.h>
#include <asm/irq.h>
#include <asm/io.h>

#include "simu_uart.h"

/*

通过对sys属性文件的读写，来模拟对tty硬件FIFO的读写
1.rx属性文件
对应模拟TTY硬件的TX-FIFO
2.tx属性文件
对应模拟TTY硬件的RX-FIFO

*/


/*计算有效数据size*/
int calc_valid_fifo(struct v_fifo* fifo)
{
	if((fifo->head)==(fifo->tail))
		return 0;
	if((fifo->head) >= (fifo->tail))
		return ((fifo->head) - (fifo->tail));
	else{
		return ((fifo->head) + (V_FIFO_SIZE-fifo->tail));
	}
}
/*计算空闲空间*/
int  calc_free_fifo(struct v_fifo* fifo)
{
	int valid = calc_valid_fifo(fifo);
	return (V_FIFO_SIZE-valid-1);
}

int writechar_2_fifo(struct v_fifo* fifo,char cc)
{
	fifo->dat[fifo->head] = cc;
	fifo->head = (fifo->head+1)&(V_FIFO_SIZE-1);

	return 0;
}

/*读fifo*/
int readchar_from_fifo(struct v_fifo* fifo,char *cc)
{
	cc[0] = fifo->dat[fifo->tail];
	fifo->tail = (fifo->tail+1)&(V_FIFO_SIZE-1);
	return 0;
}


static int init_fifo(struct v_fifo* fifo)
{
	if(!fifo)
		return -1;
	fifo->head = fifo->tail = 0;
	return 0;
}


/*定义TX-FIFO  RX-FIFO*/
static struct vuart_dev *my_vuart_dev;

struct vuart_dev * get_udev(void)
{
	return my_vuart_dev;
}


/*sysfs属性文件操作方法*/
static ssize_t curr_fifotail_show(struct device * dev, struct device_attribute *attr,char *buf)
{
	int size,i;
	struct vuart_dev * vdev = (struct vuart_dev *)dev_get_drvdata(dev);
	/*同步*/
	if(down_interruptible(&vdev->sem_rx))
		return -ERESTARTSYS;
	/*计算fifo中有效数据size*/
	size = calc_valid_fifo(&vdev->v_RX_FIFO);
	/*copy数据*/
	for(i=0;i<size;i++){
		buf[i] = vdev->v_RX_FIFO.dat[vdev->v_RX_FIFO.tail];
		vdev->v_RX_FIFO.tail = (vdev->v_RX_FIFO.tail+1)&(V_FIFO_SIZE-1);
	}

	up(&vdev->sem_rx);
	return size;
}

static ssize_t curr_fifohead_store(struct device *dev, struct device_attribute * attr, const char *buf, size_t count)
{
	int size,i;
	struct vuart_dev * vdev = (struct vuart_dev *)dev_get_drvdata(dev);

	/*同步*/
	if(down_interruptible(&vdev->sem_tx))
		return -ERESTARTSYS;
	/*计算fifo中空闲size*/
	size = calc_free_fifo(&vdev->v_TX_FIFO);
	if(size <= 0){
		printk("Failed to store data: TX FIFO is FULL.\n");
		up(&vdev->sem_tx);
		return -1;
	}
	size = min_t(int,size,count);
	/*copy数据*/
	for(i=0;i<size;i++){
		vdev->v_TX_FIFO.dat[vdev->v_TX_FIFO.head] = buf[i];
		vdev->v_TX_FIFO.head = (vdev->v_TX_FIFO.head+1)&(V_FIFO_SIZE-1);
	}

	up(&vdev->sem_tx);
	return size;
}

/*sysfs设备属性*/
static DEVICE_ATTR(rx, 0660, curr_fifotail_show ,  NULL);//S_IRUGO
static DEVICE_ATTR(tx, 0660, NULL ,  curr_fifohead_store);//S_IWUGO


static int vuart_dev_init(struct vuart_dev * udev)
{
	if((init_fifo(&udev->v_RX_FIFO)) || (init_fifo(&udev->v_TX_FIFO)) )
		return -1;
	sema_init(&udev->sem_rx,1);
	sema_init(&udev->sem_tx,1);
	return 0;
}


static struct device *my_dev;
static struct class *my_class;


//static int __init xtjin_vuart_init(void)
int xtjin_vuart_init(void)
{
	int err=-1;
	/*分配 vuart_dev 结构体*/
	my_vuart_dev = kmalloc(sizeof(struct vuart_dev),GFP_KERNEL);
	if(!my_vuart_dev){
		err = -ENOMEM;
		printk("Failed to alloc vuart device.\n");
		goto fail;
	}
	/*vuart_dev init*/
	if(vuart_dev_init(my_vuart_dev)){
		printk("Failed to init vuart device.\n");
		goto cleanup;
	}
	/*class init*/
	my_class = class_create(THIS_MODULE,"vuart");
	if(IS_ERR(my_class)){
		printk("Failed to create vuart device class.\n");
		goto cleanup;
	}
	/*在/sys/class/下建立设备文件目录:/sys/class/vuart/*/
	my_dev = device_create( my_class,NULL,0,NULL,"%s","vuart");
	if(IS_ERR(my_dev)){
		printk("Failed to create vuart device.\n");
		goto destroy_class;
	}
	/*在/sys/class/vuart/下建立属性文件:tx/rx*/
	err = device_create_file(my_dev,&dev_attr_tx) | device_create_file(my_dev,&dev_attr_rx);
	if(err<0){
		printk("Failed to create attribute file.\n");
		goto destroy_dev;
	}

	dev_set_drvdata(my_dev,my_vuart_dev);

	return 0;

destroy_dev:
	device_del(my_dev);
destroy_class:
	class_destroy(my_class);
cleanup:
	kfree(my_vuart_dev);
fail:
	return -1;
}


//static void __exit xtjin_vuart_exit(void)
void xtjin_vuart_exit(void)
{
	if(my_dev)
		device_del(my_dev);
	if(my_class)
		class_destroy(my_class);
	if(my_vuart_dev)
		kfree(my_vuart_dev);
}


//module_init(xtjin_vuart_init);
//module_exit(xtjin_vuart_exit);

//MODULE_DESCRIPTION("Driver for vUART");
//MODULE_AUTHOR("xtjin.");
//MODULE_LICENSE("GPL");


