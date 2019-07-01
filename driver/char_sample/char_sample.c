#include <linux/device.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <asm/uaccess.h>

#define CLASS_NAME "hello_class"
#define DEVICE_NAME "hello_device"
#define DATA_SIZE   32

static dev_t dev_number;
//static struct device * hello_device;
static struct class * hello_class;
static struct cdev * hello_cdev;


typedef struct{
    void * data;
    int data_size;
} resource_t;

static resource_t resource;

int get_cpsr(void){
    int tmp;
	asm volatile (
		"mrs r3,cpsr\n\t"
		"mov %0,r3\n\t"
        : "=r" (tmp)
        :
	);
    return tmp;
}

int print_cpsr(void){
    printk("kernel space:0x%x\n",get_cpsr());
}

static int hello_open(struct inode *inode, struct file *file)
{
    print_cpsr();
	printk("my device is open\n");
    file->private_data = &resource;
	return 0;
}

static ssize_t hello_read(struct file *file, char __user *buffer, size_t count, loff_t *ppos)
{
    int ret = 0;
    int actual_readed;
    int max_free;
    int need_read;


    resource_t * tmp_resource = file->private_data;
	printk("my device is read\n");

    max_free = tmp_resource->data_size - *ppos;
    printk("max_free:%d\n",max_free);
    need_read = max_free > count? count : max_free;
    printk("need_read:%d\n",need_read);

    if (need_read == 0)
        printk("hello_device,no space for read\n");

    ret = copy_to_user(buffer,((resource_t *)(file->private_data))->data + *ppos, need_read);
    if (ret == need_read)
        return -EFAULT;

    actual_readed = need_read - ret;
    printk("actual_readed:%d\n",actual_readed);
    *ppos += actual_readed;

	return actual_readed;
}

static ssize_t hello_write(struct file *file, const char __user *buffer, size_t count, loff_t *ppos)
{
    int ret = 0;
    int free;
    int actual_write;
    int need_write;

    resource_t * tmp_resource = file->private_data;
	printk("my device is write\n");

    free = tmp_resource->data_size - *ppos;
    printk("free:%d\n",free);
    need_write = free > count ? count : free;
    printk("need_write:%d\n",need_write);
    if (need_write == 0){
        printk("hello_device,no space for write\n");

    }

    ret = copy_from_user(((resource_t *)(file->private_data))->data + *ppos,buffer,need_write);
    if (ret == need_write)
        return -EFAULT;

    actual_write = need_write - ret;
    printk("actual_write:%d\n",actual_write);
    *ppos += actual_write;

	return actual_write;
}

static loff_t hello_llseek (struct file * file, loff_t ppos, int whence){

    loff_t newpos;
    resource_t * tmp_resource = file->private_data;

    switch(whence)
    {
        case 0: /* SEEK_SET */
            newpos = ppos;
            break;

        case 1: /* SEEK_CUR */
            newpos = file->f_pos + ppos;
            break;

        case 2: /* SEEK_END */
            newpos = tmp_resource->data_size + ppos;
            break;

        default: /* can't happen */
            return -EINVAL;
    }
    if (newpos < 0)
        return -EINVAL;
    file->f_pos = newpos;
    return newpos;

}

static int hello_close(struct inode *inode, struct file *file)
{
	printk("my device is close\n");
	return 0;
}

static struct file_operations hello_fops = {
	.owner  = THIS_MODULE,
	.open	= hello_open,
	.read 	= hello_read,
	.write 	= hello_write,
    .llseek = hello_llseek,
	.release = hello_close,

    .aio_read = NULL, // 如果不填充,默认为NULL
    .aio_write = NULL,
    .read_iter = NULL,
    .write_iter = NULL,
    .iterate = NULL,
    .poll = NULL,
    .unlocked_ioctl = NULL,
    .compat_ioctl = NULL,
    .mmap = NULL,
    .mremap = NULL,
    .flush = NULL,
    .fsync = NULL,
    .aio_fsync = NULL,
    .fasync = NULL,
    .lock = NULL,
    .sendpage = NULL,
    .get_unmapped_area = NULL,
    .check_flags = NULL,
    .flock = NULL,
    .splice_write = NULL,
    .splice_read = NULL,
    .setlease = NULL,
    .fallocate = NULL,
    .show_fdinfo = NULL,
};


static void data_init(void){
    resource.data_size = DATA_SIZE;
    resource.data = kzalloc(resource.data_size,GFP_KERNEL);
    strcpy(resource.data,"hello world");
}

static void data_deinit(void){

    kfree(resource.data);
    resource.data = NULL;
}
static int __init hello_init(void)
{
	int error;

    // 申请设备号
	error = alloc_chrdev_region(&dev_number, 0, 1, "hello_dev");
	if (error)
	{
		printk("hello: alloc_chardev_region failed! ");
		goto out;
	}

    // 申请 cdev
	hello_cdev = cdev_alloc();
	if (hello_cdev == NULL)
	{
		printk("hello: alloc cdev failed! ");
		error = -ENOMEM;
		goto out_chrdev;
	}
    // 填充 cdev
	hello_cdev->ops = &hello_fops;
	hello_cdev->owner = THIS_MODULE;

    // 绑定设备号到cdev,并注册cdev
	error = cdev_add(hello_cdev, dev_number, 1);
	if (error)
	{
		printk("hello: cdev_add failed! ");
		goto out_cdev;
	}
    // 创建 /sys/CLASS_NAME 文件夹
	hello_class = class_create(THIS_MODULE, CLASS_NAME);
	if (IS_ERR(hello_class))
	{
		error = PTR_ERR(hello_class);
		goto out_chrdev;
	}
    // 在 /sys/CLASS_NAME 文件夹下创建 /sys/CLASS_NAME/DEVICE_NAME
	device_create(hello_class, NULL, dev_number, NULL, DEVICE_NAME);

    data_init();
	printk("hello: Hello World \n");
	return 0;

out_cdev:
	cdev_del(hello_cdev);
out_chrdev:
	unregister_chrdev_region(hello_cdev->dev, 1);
out:
	return error;
}

static void __exit hello_exit(void)
{
    // 销毁文件
	device_destroy(hello_class, dev_number);
    // 销毁 /sys/CLASS_NAME 文件夹
	class_destroy(hello_class);
    // 注销设备号
	unregister_chrdev_region(hello_cdev->dev, 1);
    // 注销 cdev
	cdev_del(hello_cdev);

    data_deinit();

	printk("hello: Goodbye World \n");
}


module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("YWW");
MODULE_DESCRIPTION("HELLO_CLASS_AUTO_DEV");
