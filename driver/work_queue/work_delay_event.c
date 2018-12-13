#include<linux/init.h>
#include<linux/module.h>

// 头文件
#include <linux/workqueue.h>

// 全局
static struct delayed_work  test_delay_work;

// handler
static void test_delay_work_func(struct work_struct *work)
{
    printk("hello\n");

}

static void work_init(void){
    // 初始化
    INIT_DELAYED_WORK(&test_delay_work, test_delay_work_func);

    // 提交任务到 event
    schedule_delayed_work(&test_delay_work, msecs_to_jiffies(1000)); // 50ms // 转换到 jiffies 为 5

}

static void work_deinit(void){

}

static int hello_init(void)
{
    printk(KERN_ALERT"-----------------!\n");
    printk(KERN_ALERT"hello world!\n");
    work_init();
    printk(KERN_ALERT"hello vexpress!\n");
    printk(KERN_ALERT"-----------------!\n");

    return 0;
}

static void  __exit hello_exit(void)
{
    work_deinit();
    printk(KERN_ALERT"goodbye, crazy world!\n");
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
