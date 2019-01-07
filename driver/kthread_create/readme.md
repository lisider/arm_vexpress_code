## 综述

内核中没有创建内核进程的概念,只有创建内核线程的概念
	kernel_thread
	kthread_create
	kthread_run


- kernel_thread
	kernel_thread()是通过do_fork()进行创建的线程
	在2.6的版本中，这种创建内核线程的方式还可以用于驱动模块中
	在4.x的内核版本中就不可以在驱动模块中使用，也就是说想要使用kernel_thread之内将驱动程序编译进内核才能够创建线程，想要使用insmod方式创建内核线程，就会被拒绝insmod，因为kernel_thread没有EXPORT_SYSMBOL 出来。
	创建线程之后,线程会马上执行


- kthread_create
	创建一个内核线程,但不会马上执行
	在内核中和在驱动模块中，都可以创建出新线程。
	kthread_create()创建出的新线程的父进程是kthreadd
		在kthread_create创建出的线程也对其上下文环境也进行了清理
		所以kthread_create()是比较正统的创建线程的方法。
		推荐使用kthread_create()的方式创建线程。

	让线程马上执行的方法
	struct task_struct * _task = kthread_create(thread_func, NULL, "thread_func2");
	wake_up_process(_task);


- kthread_run
	kthread_run()是一个宏定义，最终调用到 kthread_create()
	kthread_run 开了进程之后,进程会马上执行
	在内核中和在驱动模块中，都可以创建出新线程。

	#define kthread_run(threadfn, data, namefmt, ...)                          \
			({                                                                         \
			 struct task_struct *__k                                            \
			 = kthread_create(threadfn, data, namefmt, ## __VA_ARGS__); \
			 if (!IS_ERR(__k))                                                  \
			 wake_up_process(__k);                                      \
			 __k;                                                               \
			 })

