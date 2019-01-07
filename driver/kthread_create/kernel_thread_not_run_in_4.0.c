#include <linux/module.h>
#include <linux/init.h>
#include <linux/kmod.h>
#include <linux/sched.h>
#include <linux/delay.h>
MODULE_LICENSE("GPL");

#define CLONE_KERNEL    (CLONE_FS | CLONE_FILES | CLONE_SIGHAND)

int my_kernel_thread(void *arg)
{
	int n = 0;

	while(1)
	{
		printk("%s: %d\n",__func__,n++);
		ssleep(3);
	}

	return 0;
}
static int __init practiceCall(void)
{
	printk("%s:\n",__func__);
	kernel_thread(my_kernel_thread,NULL,CLONE_KERNEL);

	return 0;
}
static void __exit practiceCallExit(void)
{
	printk("%s:\n",__func__);
}

module_init(practiceCall);
module_exit(practiceCallExit);
