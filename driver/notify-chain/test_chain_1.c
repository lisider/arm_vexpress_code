/* test_chain_1.c ：1. 定义回调函数；2. 定义notifier_block；3. 向chain_0注册notifier_block；*/
#include <linux/notifier.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>	/* printk() */
#include <linux/fs.h>	/* everything() */

// 通知的内容
#define TESTCHAIN_INIT 0x52U

// 这是通知者和 被通知者 中间的 媒介
static RAW_NOTIFIER_HEAD(test_chain);

/* define our own notifier_call_chain */
// 这个函数给 通知者用 , 发送的 通知 只能被 注册test_chain的被通知者 接收
static int call_test_notifiers(unsigned long val, void *v)
{
	return raw_notifier_call_chain(&test_chain, val, v);
}
EXPORT_SYMBOL(call_test_notifiers);

/* define our own notifier_chain_register func */
//这个函数给 被 通知者用
static int register_test_notifier(struct notifier_block *nb)
{
	int err;
	err = raw_notifier_chain_register(&test_chain, nb);

	if(err)
		goto out;

out:
	return err;
}

//EXPORT_SYMBOL(register_test_notifier);

// 被通知者接收通知后
/* realize the notifier_call func */
int test_init_event(struct notifier_block *nb, unsigned long event,
	void *v)
{
	switch(event){
	case TESTCHAIN_INIT:
		printk(KERN_DEBUG "I am test_chain_1 ,I got the chain event: test_chain_2 is on the way of init\n");
		break;

	default:
		break;
	}

	return NOTIFY_DONE;
}
/* define a notifier_block */
static struct notifier_block test_init_notifier = {
	.notifier_call = test_init_event,
};
static int __init test_chain_1_init(void)
{
	printk(KERN_DEBUG "I'm in test_chain_1\n");
	register_test_notifier(&test_init_notifier);	// 由chain_0提供的设施
	return 0;
}

static void __exit test_chain_1_exit(void)
{
	printk(KERN_DEBUG "Goodbye to test_clain_l\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("fishOnFly");

module_init(test_chain_1_init);
module_exit(test_chain_1_exit);

