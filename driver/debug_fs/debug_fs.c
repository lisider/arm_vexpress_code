// dbgfs.c
/*
 * (C) 05-07-2012 Yang Honggang (Joseph), Dslab <eagle.rtlinux@gmail.com>
 */
#include <linux/debugfs.h>
#include <linux/module.h>
#include <linux/mm.h>
#include <asm/uaccess.h>

struct dentry *parent, *sw, *inf;
u32 tr_on = 0;

struct my_data_struct {
	void* data;
	unsigned long size;
} mds;

struct page* pg;

static ssize_t data_read(struct file *file, char __user *user_buf,
		size_t count, loff_t *ppos)
{
	unsigned long i;
	size_t cnt;

	printk("tr_on:%d\n", tr_on);
	/* If the tracing_on is Y, fill the data buffer with debug info */
	if (tr_on) {
		tr_on = 0; /* Automaticlly clear the 'tracing_on' flag */
		for (i = 0; i < (mds.size - 11) / 11; i ++) {
			sprintf((char*)((char*)mds.data + i * 11 ), "%ld\n", i + 1000000000);
		}

		/* Copy debug info to userspace */
		cnt = copy_to_user(user_buf, (const void *)mds.data, mds.size);
		return (mds.size - cnt);
	}

	return 0;
}

const struct file_operations fops =
{
	.read = data_read,
};

static int __init dbgfs_demon_init(void)
{

	printk("dbgfs init\n");
	/* Create dbgfs_demon directory in the debugfs root */
	parent = debugfs_create_dir("dbgfs_demon", NULL);

	if (!parent){
		return -1;
	}

	/* Create a output switch in dbgfs_demon */
	sw = debugfs_create_bool("tracing_on", S_IRWXU,
			parent, &tr_on);
	if (!sw)
		goto p_out;

	/* Create a file for debug information exporting */
	mds.size = 4 * 1024;

	/* Allocate one page for info. storing */
	pg = alloc_pages(__GFP_HIGHMEM | __GFP_ZERO, 0);
	/* Covert to Memory address */
	mds.data = (void*) page_address(pg);
	if (!pg)
		goto p_out;

	inf = debugfs_create_file("data", S_IRUSR,//|S_IWUSR,
			parent, &mds, &fops);
	if (!inf)
		goto sw_out;

	return 0;

sw_out:
	debugfs_remove(sw);
	__free_pages(pg, 0);
p_out:
	debugfs_remove(parent);

	return -1;
}

static void __exit dbgfs_demon_exit(void)
{

	if (pg)
		__free_pages(pg, 0);

	debugfs_remove(inf);
	debugfs_remove(sw);
	debugfs_remove(parent);
	printk("dbgfs exit\n");

	return;
}

module_init(dbgfs_demon_init);
module_exit(dbgfs_demon_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yang Honggang (Joseph) <eagle.rtlinux@gmail.com>");
