#include <linux/module.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/kthread.h>

static int   dmatest_work (void *data)
{
	allow_signal(SIGTERM);
	current->state = TASK_INTERRUPTIBLE;

	printk("New kernel thread run\n");

	return 0;
}

static int __init dmatest_init(void)
{
	/* Schedule the test thread */
	kthread_run (dmatest_work, NULL, "dmatest");

	return 0;
}

static void __exit dmatest_exit(void)
{
	return;
}

MODULE_LICENSE("Dual BSD/GPL");
module_init(dmatest_init);
module_exit(dmatest_exit);
