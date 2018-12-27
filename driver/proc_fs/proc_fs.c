#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <linux/seq_file.h>

struct proc_dir_entry * mytest_dir  = NULL;
struct proc_dir_entry * mytest_file = NULL;

static int proc_show_ver(struct seq_file *file, void *v)
{
	int cnt = 0;
	seq_printf(file, "Driver: %s\n", "driver name");    /*函数seq_printf是最常用的输出函数，它用于把给定参数按照给定的格式输出到seq_file文件*/
	seq_printf(file, "Author: %s\n", "author");
	seq_printf(file, "Version: %s\n", "ver1.0");
	return cnt;
}

static int proc_key_open(struct inode *inode, struct file *file)
{
	single_open(file, proc_show_ver, NULL);  /*将proc_show_ver 和 seq_file文件关联起来*/
	return 0;
}

static struct file_operations mytest_proc_fops = {
	.owner   = THIS_MODULE,
	.open     = proc_key_open,
	.read     = seq_read,     /*定义在linux/seq_file.c*/
	.release = single_release,  /*定义在linux/seq_file.c*/
};

static void proc_example_init(void){
	printk("suws_kernel proc_fs create dir %s,%s,%d\n",__FILE__,__func__,__LINE__);
	mytest_dir = proc_mkdir("mytest_dir", NULL);
	if(mytest_dir == NULL)
		printk("suws_kernel proc_fs create dir fail %s,%s,%d\n",__FILE__,__func__,__LINE__);

	printk("suws_kernel proc_fs create file %s,%s,%d\n",__FILE__,__func__,__LINE__);
	mytest_file = proc_create("mytest_file", 0x0644, mytest_dir, &mytest_proc_fops);
	if (mytest_file == NULL)
		printk("suws_kernel proc_fs create file fail %s,%s,%d\n",__FILE__,__func__,__LINE__);
	//proc_create(PROC_NAME, 0, NULL, &mytest_proc_fops); //在 proc 下创建
}

static void proc_example_deinit(void){
	proc_remove(mytest_file);
	proc_remove(mytest_dir);

}

static int hello_init(void)
{
    printk(KERN_ALERT"-----------------!\n");
    printk(KERN_ALERT"hello world!\n");
    proc_example_init();
    printk(KERN_ALERT"hello vexpress!\n");
    printk(KERN_ALERT"-----------------!\n");

    return 0;
}

static void  __exit hello_exit(void)
{
    proc_example_deinit();
    printk(KERN_ALERT"goodbye, crazy world!\n");
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
