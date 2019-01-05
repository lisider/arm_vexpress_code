#include <linux/module.h>
#include <linux/input.h>
#include <linux/hrtimer.h>

/**
  参考：
  输入子系统的写法demo　https://www.kernel.org/doc/html/latest/input/input-programming.html
  输入子系统的函数api：https://www.kernel.org/doc/html/latest/driver-api/input.html
  测试:cat /proc/bus/input/devices会发现注册的设备
 **/

static struct input_dev *button_dev;
static struct hrtimer vibe_timer;

#define MAG_MAX_NEG -500

#define MAG_MAX_POS 500

#define value 2000

static int count = 0;

enum hrtimer_restart  timer_func(struct hrtimer * timer){
    //input_report_key(button_dev, KEY_POWER,1);
    //input_sync(button_dev);
    input_report_abs(button_dev,ABS_GAS,count);
    input_sync(button_dev);

    count ++;
    if(count == 500)
        count = 0;
    hrtimer_start(&vibe_timer,ktime_set(value / 1000, (value % 1000) * 1000000),HRTIMER_MODE_REL);
    //return HRTIMER_RESTART; // 重新开始
    return HRTIMER_NORESTART; // 结束
}


static int __init test_init(void)
{
    int error;
    button_dev = input_allocate_device();//分配输入设备内存，失败返回NULL
    if (!button_dev) {
        printk(KERN_ERR "button.c: Not enough memory\n");//如果分配失败，没有足够内存
        error = -ENOMEM;
        return error;
    }
    //输入设备名字
    button_dev->name="I am simplest input subsystem";
#if 0
    //输入设备时间类型为键盘事件
    button_dev->evbit[0] = BIT_MASK(EV_KEY);
    //输入设备的键盘掩码为：拥有方向上键
    button_dev->keybit[BIT_WORD(KEY_UP)] = BIT_MASK(KEY_UP);

    //设置产品id
    button_dev->id.bustype=BUS_I8042;
    button_dev->id.vendor=0x1111;
    button_dev->id.product=0x2222;
    button_dev->id.version=0x5555;
#endif

    set_bit(EV_ABS, button_dev->evbit);
    input_set_abs_params(button_dev, ABS_GAS, MAG_MAX_NEG, MAG_MAX_POS, 0, 0);

    //将设备注册进输入子系统
    error = input_register_device(button_dev);
    if (error) {
        printk(KERN_ERR "button.c: Failed to register device\n");//注册失败
        input_free_device(button_dev);//释放内存
        return error;
    }

    hrtimer_init(&vibe_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
    vibe_timer.function = timer_func;
    hrtimer_start(&vibe_timer,ktime_set(value / 1000, (value % 1000) * 1000000),HRTIMER_MODE_REL);
    printk(KERN_DEBUG "OK,input subsystem inited!\n");
    printk("OK,input subsystem inited!\n");
    return 0;
}

static void __exit test_exit(void)
{
    input_unregister_device(button_dev);
    hrtimer_cancel(&vibe_timer);
}

module_init(test_init);
module_exit(test_exit);
MODULE_LICENSE("GPL");

