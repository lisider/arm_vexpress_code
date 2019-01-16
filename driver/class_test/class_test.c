#include <linux/device.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/fs.h>

#define CLASS_NAME "hello_class"
static dev_t dev;
static struct class *hello_class;
static struct cdev *hello_cdev;


static int hello_open(struct inode *inode,struct file *flp)
{
	printk("my device is open\n");
	dump_stack();
	return 0;
}
ssize_t hello_read(struct file *fp, char __user *buf, size_t size, loff_t *loff)
{
	printk("my device is read\n");
	dump_stack();
	return 0;
}

ssize_t hello_write(struct file *file, const char __user *buf, size_t size, loff_t *loff)
{
	printk("my device is write\n");
	dump_stack();
	return 0;
}

static int hello_close(struct inode *inode,struct file *flp)
{
	printk("my device is close\n");
	dump_stack();
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

	error = alloc_chrdev_region(&dev, 0, 2, "hello_dev");
	if (error)
	{
		printk("hello: alloc_chardev_region failed! ");
		goto out;
	}
	hello_cdev = cdev_alloc();
	if (hello_cdev == NULL)
	{
		printk("hello: alloc cdev failed! ");
		error = -ENOMEM;
		goto out_chrdev;
	}
	hello_cdev->ops = &hello_fops;
	hello_cdev->owner = THIS_MODULE;
	error = cdev_add(hello_cdev, dev, 1);
	if (error)
	{
		printk("hello: cdev_add failed! ");
		goto out_cdev;
	}
	hello_class = class_create(THIS_MODULE, CLASS_NAME);
	if (IS_ERR(hello_class))
	{
		error = PTR_ERR(hello_class);
		goto out_chrdev;
	}
	device_create(hello_class, NULL, dev, NULL, CLASS_NAME);
	//memset (hello_buf, 0, sizeof(hello_buf));
	//memcpy(hello_buf, DEFAULT_MSG, sizeof(DEFAULT_MSG));
	printk("hello: Hello World!\n");
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
	device_destroy(hello_class, dev);
	class_destroy(hello_class);
	unregister_chrdev_region(hello_cdev->dev, 2);
	cdev_del(hello_cdev);
	printk("hello: Goodbye World ");
}


module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("YWW");
MODULE_DESCRIPTION("HELLO_CLASS_AUTO_DEV");
