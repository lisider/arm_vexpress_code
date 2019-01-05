/* test_chain_2.c：发出通知链事件*/

#include <linux/notifier.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>	/* printk() */
#include <linux/fs.h>	/* everything() */

extern int call_test_notifiers(unsigned long val, void *v);
#define TESTCHAIN_INIT 0x52U

static int __init test_chain_2_init(void)
{
	printk(KERN_DEBUG "I'm in test_chain_2\n");
	call_test_notifiers(TESTCHAIN_INIT, "no_use");

	return 0;
}

static void __exit test_chain_2_exit(void)
{
	printk(KERN_DEBUG "Goodbye to test_chain_2\n");
}

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("fishOnFly");

module_init(test_chain_2_init);
module_exit(test_chain_2_exit);

