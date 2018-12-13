#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>


static void test_tasklet_handler(unsigned long data){

    printk("hello handler\n");
}


static DECLARE_TASKLET(test_tasklet, test_tasklet_handler, 0);


static int hello_init(void)
{
	printk(KERN_ALERT"-----------------!\n");
	printk(KERN_ALERT"hello world!\n");
    tasklet_schedule(&test_tasklet);
	printk(KERN_ALERT"hello vexpress!\n");
	printk(KERN_ALERT"-----------------!\n");

	return 0;
}

static void  __exit hello_exit(void)
{
	printk(KERN_ALERT"goodbye, crazy world!\n");
}

MODULE_LICENSE("GPL");
module_init(hello_init);
module_exit(hello_exit);
