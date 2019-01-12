/*************************************************************************
  > File Name: sys_value.c
  > Author: Sues
  > Mail: sumory.kaka@foxmail.com
  > Created Time: Fri 11 Jan 2019 10:05:44 PM PST
 ************************************************************************/
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sysfs.h>
#include <linux/platform_device.h>


/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  可以将字符串转换为 int 和 unsigned int , 自动识别
 * 				相较于 内核提供的kstrtouint(只能将字符串转换为 unsigned int)
 *
 * @Param str 字符串
 *
 * @Returns   返回转换后的整数值
 */
/* ----------------------------------------------------------------------------*/
static int str2int(const char *str) {
	int temp = 0;
	const char *ptr = str;  //ptr保存str字符串开头
	if (*str == '-' || *str == '+')  //如果第一个字符是正负号，
	{                      //则移到下一个字符
		str++;
	}

	while(*str != 0)
	{
		if ((*str < '0') || (*str > '9'))  //如果当前字符不是数字
		{                       //则退出循环
			break;
		}
		temp = temp * 10 + (*str - '0'); //如果当前字符是数字则计算数值
		str++;      //移到下一个字符
	}

	if (*ptr == '-')     //如果字符串是以“-”开头，则转换成其相反数
	{
		temp = -temp;
	}
	return temp;
}


//---------------------------- 节点序列中的 节点 str

static char node_str_buf[32] = {0};

static ssize_t sysfs_demo_show_node_str(struct kobject *kobj, struct kobj_attribute *attr, char * buf)
{
    pr_info("enter show node: %s\n", attr->attr.name);
    return sprintf(buf, "%s\n", node_str_buf);
}

static ssize_t sysfs_demo_store_node_str(struct kobject *kobj, struct kobj_attribute *attr, const char * buf, size_t n)
{
    pr_info("enter write node: %s\n", attr->attr.name);
    snprintf(node_str_buf,(n <= sizeof(node_str_buf)) ? n : sizeof(node_str_buf), "%s", buf);
    return n;
}

static struct kobj_attribute node_str_attribute =
    __ATTR(node_str, S_IWUSR|S_IRUGO, sysfs_demo_show_node_str, sysfs_demo_store_node_str); //读写属性

//---------------------------- 节点序列中的 节点 int
static int node_int_value = 100;
static ssize_t sysfs_demo_show_node_int(struct kobject *kobj, struct kobj_attribute *attr, char * buf)
{
    pr_info("enter show node: %s\n", attr->attr.name);
    return snprintf(buf,12, "%d\n", node_int_value);
}

static ssize_t sysfs_demo_store_node_int(struct kobject *kobj, struct kobj_attribute *attr, const char * buf, size_t n)
{
    pr_info("enter write node: %s\n", attr->attr.name);
    pr_info("buf: %s\n", buf);
	if(strlen(buf) > 11) // int 32 最多 11 个字节 -2147483648 ~ 2147483647
		pr_info("negative long then 11,positive long then 10\n");
	else
		//kstrtouint(buf,10,&node_int_value); // kstrtouint 只能转字符串到 unsigned int
		node_int_value = str2int(buf);
    return n; //如果不是n ,则会 不断循环进入这个函数
}
static struct kobj_attribute node_int_attribute =
    __ATTR(node_int, S_IWUSR|S_IRUGO, sysfs_demo_show_node_int, sysfs_demo_store_node_int); // 读属性

//---------------------- 文件序列包括 两个节点
static struct attribute *sysfs_demo_attributes[] = {
    &node_str_attribute.attr,
    &node_int_attribute.attr,
    NULL
};

static const struct attribute_group sysfs_demo_attr_group = {
    .attrs = sysfs_demo_attributes,
};


static struct kset * value_kset;
int __init _driver_init(void)
{
	// 在 /sys 下创建一个目录 /sys/value
	value_kset = kset_create_and_add("value", NULL, NULL);
	//------------------ 在 目录下 创建 一个 文件序列

	// 在 /sys/value/ 下面创建一系列文件
	if(sysfs_create_group(&value_kset->kobj, &sysfs_demo_attr_group) ) { //在 /sys/devices/platform/node-platform 下创建文件序列
		pr_err("sysfs_create_group failed\n");
	}
	return 0;
}

void __exit _driver_exit(void)
{
	sysfs_remove_group(&value_kset->kobj, &sysfs_demo_attr_group);
	kobject_del(&value_kset->kobj);
	return ;
}

/********模块三要素**********/
module_init(_driver_init);
module_exit(_driver_exit);
MODULE_LICENSE("GPL");
