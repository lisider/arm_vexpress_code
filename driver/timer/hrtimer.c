#include<linux/init.h>
#include<linux/module.h>

//头文件
#include <linux/hrtimer.h>

//全局变量
static struct hrtimer vibe_timer;
static int value = 2000; // 2000ms

//handler
static enum hrtimer_restart  timer_handler(struct hrtimer * timer){

    //printk("helloxxx\n");

    hrtimer_start(&vibe_timer,ktime_set(value / 1000, (value % 1000) * 1000000),HRTIMER_MODE_REL);
    //return HRTIMER_RESTART; // 重新开始
    return HRTIMER_NORESTART; // 结束
}

//初始化
static void timer_init(void){
    hrtimer_init(&vibe_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
    vibe_timer.function = timer_handler;
    hrtimer_start(&vibe_timer,ktime_set(value / 1000, (value % 1000) * 1000000),HRTIMER_MODE_REL);
}

static void timer_deinit(void){
    hrtimer_cancel(&vibe_timer);
}


static int hello_init(void)
{
    timer_init();
	printk(KERN_ALERT"-----------------!\n");
	printk(KERN_ALERT"hello world!\n");
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
