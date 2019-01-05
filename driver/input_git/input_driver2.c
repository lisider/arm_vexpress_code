#include <linux/module.h>
#include <linux/input.h>
#include <linux/hrtimer.h>
#include <linux/slab.h>


#define MAG_MAX_NEG -500

#define MAG_MAX_POS 500

struct test_device{

    struct input_dev *test_input_dev;

    struct timer_list s_timer; //定时器结构体

    unsigned int count;

};

struct test_device * test_dev;

//定时器处理函数

static void second_timer_handle(unsigned long arg)

{

    mod_timer(&test_dev->s_timer,jiffies + HZ);

    //提交input事件

    input_report_abs(test_dev->test_input_dev,ABS_GAS,test_dev->count);

    input_sync(test_dev->test_input_dev);

    test_dev->count++;

    if(test_dev->count ==100)

        test_dev->count =0;

    //printk( "current jiffies is %ld\n", jiffies);

    //printk( "count is %d\n", test_dev->count);

}

static int __init demo_init(void) //加载函数

{

    int ret =0;

    test_dev = kzalloc(sizeof(struct test_device),GFP_KERNEL);

    if(test_dev == NULL)

    {

        printk("Failed to allocate driver struct\n");

        ret = -ENOMEM;

        goto err_kzalloc_device;

    }

    test_dev->test_input_dev = input_allocate_device();

    if(test_dev->test_input_dev == NULL) {

        printk( "Failed to allocate input dev\n");

        ret = -ENOMEM;

        goto err_input_allocate_device;

    }

    test_dev->test_input_dev->name = "test_input";

    set_bit(EV_ABS, test_dev->test_input_dev->evbit);

    input_set_abs_params(test_dev->test_input_dev, ABS_GAS, MAG_MAX_NEG, MAG_MAX_POS, 0, 0);

    ret =input_register_device(test_dev->test_input_dev);

    if(ret)

    {

        printk( "Failed to input_unregister_device\n");

        goto err_input_unregister_device;

    }

    //初始化定时器，注册定时器

    test_dev->count =0;

    init_timer(&test_dev->s_timer);

    test_dev->s_timer.function = second_timer_handle;

    test_dev->s_timer.expires = jiffies + HZ;

    add_timer(&test_dev->s_timer);

    printk("demo_init");

    return ret;

err_input_unregister_device:

    input_free_device(test_dev->test_input_dev);

err_input_allocate_device:

    kfree(test_dev);

err_kzalloc_device:

    return ret;

}

static void __exit demo_exit(void) //卸载函数

{

    del_timer(&test_dev->s_timer);

    input_unregister_device(test_dev->test_input_dev);

    input_free_device(test_dev->test_input_dev);

    kfree(test_dev);

    printk("demo_exit");

}

module_init(demo_init);
module_exit(demo_exit);
MODULE_LICENSE("GPL");
