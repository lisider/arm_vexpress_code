#include <linux/device.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#define CLASS_NAME "hello_class"
#define DEVICE_NAME "hello_device"

static dev_t dev_number;
//static struct device * hello_device;
static struct class * hello_class;
static struct cdev * hello_cdev;

//代表资源为空,可以写入,但不可以读出
static int resources = 0;

static int hello_open(struct inode *inode,struct file *flp)
{
	printk("my device is open\n");
    resources = 1;
	return 0;
}
ssize_t hello_read(struct file *file, char __user *buf, size_t size, loff_t *loff)
{
	printk("my device is read\n");
    if (!resources)
        if(file->f_flags & O_NONBLOCK)
            return -EAGAIN;

    copy_to_user(buf,&resources+*loff,size);

	return 0;
}

ssize_t hello_write(struct file *file, const char __user *buf, size_t size, loff_t *loff)
{
	printk("my device is write\n");
    if (resources)
        if(file->f_flags & O_NONBLOCK)
            return -EAGAIN;
	return 0;
}

static int hello_close(struct inode *inode,struct file *flp)
{
	printk("my device is close\n");
	return 0;
}
static struct file_operations hello_fops={
	.owner=THIS_MODULE,
	.open	=hello_open,
	.read 	=hello_read,
	.write 	=hello_write,
	.release=hello_close,
};

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

	printk("hello: Hello World \n");
	return 0;

out_cdev:
	cdev_del(hello_cdev);
out_chrdev:
	unregister_chrdev_region(hello_cdev->dev, 2);
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

	printk("hello: Goodbye World \n");
}


module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("YWW");
MODULE_DESCRIPTION("HELLO_CLASS_AUTO_DEV");
