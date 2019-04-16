#include<linux/init.h>
#include<linux/module.h>

static int hello_init(void)
{
    printk("\001" "0" "\"\\001\" \"0\" level 0!\n");
    printk(KERN_EMERG "KERN_EMERG level 0!\n");
    pr_emerg("pr_emerg level 0!\n");
    /*
     * 1 KERN_ALERT
     * 2 KERN_CRIT
     * 3 KERN_ERR
     * 4 KERN_WARNING
     * 5 KERN_NOTICE
     * */

    printk("\001" "6" "\"\\001\" \"6\" level 6!\n");
    printk(KERN_INFO "KERN_INFO level 6!\n");
    pr_info("pr_info level 6!\n");

    printk("\001" "7" "\"\\001\" \"7\" level 7!\n");
    printk(KERN_DEBUG "KERN_DEBUG level 7!\n");
    pr_debug("pr_debug level 7!\n"); // 在 DEBUG 定义的情况下才和上两行等同,没有定义debug 的情况下不打印

    printk("\001" "d" "\"\\001\" \"d\" level default!\n");
    printk(KERN_DEFAULT "KERN_DEFAULT level d!\n");
    printk("KERN_DEFAULT level d!\n");

    printk("" "printk cont"); // 用的等级是 默认优先级 // 7 4 1 7 中的4 // 作用参考 linux_4.0/include/linux/kern_levels.h 18行
    pr_cont("pr_cont cont\n");

	return 0;
}

static void  __exit hello_exit(void)
{
	printk(KERN_ALERT"goodbye, crazy world!\n");
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
