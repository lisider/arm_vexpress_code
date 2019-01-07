#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/param.h>
#include <linux/jiffies.h>
#include <asm/processor.h>
#include <asm/signal.h>
#include <linux/sched.h>
#include <linux/delay.h>            // mdelay()
#include <linux/kthread.h>

static struct task_struct *task;
static int flag = 0;
static char exit_state = 0;

int my_fuction(void *arg)
{
	printk("fuction %s init\n", __FUNCTION__);
	allow_signal(SIGKILL); //使得线程可以接收SIGKILL信号 
	// 注意 kthread_should_stop 使得进程可以接收 kthread_stop 信号,如果没有 kthread_should_stop ,kthread_stop不会杀死进程
	// 使得线程可以可以被kill -9 的时候结束,也可以在 kthread_stop 的时候结束
	while (!signal_pending(current) && (exit_state = 1) && !kthread_should_stop()) {
		printk("jiffies is %lu\n", jiffies);
		printk("do something\n");
		schedule_timeout(HZ * 5);
		set_current_state(TASK_INTERRUPTIBLE);  // 不能去掉,如果去掉,会不停打印,就没有延时5s的效果
		exit_state = 0;
	}
	printk("Leaving my_function,exit state is %s\n",(exit_state == 1 ?"rmmod":"signal"));
	printk("fuction %s deinit\n", __FUNCTION__);
	flag = 1; //防止 进程死后,rmmod 发送 kthread_stop 信号
	return 0;
}

static int __init init(void)
{
	task = kthread_run(my_fuction,NULL,"my_function");
	return 0;
}

static void __exit finish(void)
{
	if(!flag) // 防止进程死后,kthread_stop运行
	{
		if (!IS_ERR(task))
		{
			int ret = kthread_stop(task);
			printk(KERN_INFO "First thread function has stopped ,return %d\n", ret);
		}
	}
	printk("task_struct: 0x%x",(int)task);
	printk("Goodbye\n");
}

module_init(init);
module_exit(finish);
MODULE_LICENSE("GPL");

