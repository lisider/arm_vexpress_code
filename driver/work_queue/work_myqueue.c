#include<linux/init.h>
#include<linux/module.h>


// 头文件
#include <linux/workqueue.h>

// 全局
static struct work_struct test_work;
static struct workqueue_struct * test_workqueue;

//handler
static void test_work_func(struct work_struct *work){
    printk("hello\n");
    return ;
}


// 初始化
static void work_init(void){
    INIT_WORK(&test_work, test_work_func);
    test_workqueue = create_workqueue("test_workqueue");
    // 提交任务到 event
    queue_work(test_workqueue, &test_work);

}


static void work_deinit(void){
    destroy_workqueue(test_workqueue);

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
