#include<linux/init.h>
#include<linux/module.h>

// 头文件
#include <linux/hrtimer.h>



// 全局
static struct timer_list timer; //定时器结构体
static int value = 2; // 2s

//handler
static void second_timer_handle(unsigned long arg)

{

    printk( "current jiffies is %ld\n", jiffies);
    mod_timer(&timer,jiffies + value*HZ);




    //printk( "count is %d\n", test_dev->count);

}

// 初始化
static void timer_init(void){
    init_timer(&timer);

    timer.function = second_timer_handle;

    timer.expires = jiffies + value*HZ;

    add_timer(&timer);
}

//去初始化
static void timer_deinit(void){
    del_timer(&timer);
}


static int hello_init(void)
{
	printk(KERN_ALERT"-----------------!\n");
	printk(KERN_ALERT"hello world!\n");
    timer_init();
	printk(KERN_ALERT"hello vexpress!\n");
	printk(KERN_ALERT"-----------------!\n");

	return 0;
}

static void  __exit hello_exit(void)
{
    timer_deinit();
	printk(KERN_ALERT"goodbye, crazy world!\n");
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
