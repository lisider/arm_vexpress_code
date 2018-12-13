#include<linux/init.h>
#include<linux/module.h>


static int  __init hello_init(void)
{
	printk(KERN_ALERT"-----------------!\n");
	printk(KERN_ALERT"hello world!\n");
	printk(KERN_ALERT"hello vexpress!\n");
	printk(KERN_ALERT"-----------------!\n");

	return 0;
}

static void  __exit hello_exit(void)
{
	printk(KERN_ALERT"goodbye, crazy world!\n");
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
