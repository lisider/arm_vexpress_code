#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/sched.h>


static __init int testDriver1_init(void){
	int result = 0;
	char cmdPath[] = "/bin/touch";
	char* cmdArgv[] = {cmdPath,"/touchX.txt",NULL};
	char* cmdEnvp[] = {"HOME=/","PATH=/sbin:/bin:/usr/bin",NULL};

	result = call_usermodehelper(cmdPath,cmdArgv,cmdEnvp,UMH_WAIT_PROC); //UMH_NO_WAIT
	printk("testDriver1_init exec!Theresult of call_usermodehelper is %d\n",result);
	printk("testDriver1_init exec!Theprocess is %s,sys_getpid is %d\n",current->comm,current->pid);
	return result;
}


static __exit void testDriver1_exit(void){
	int result=0;
	char cmdPath[]="/bin/rm";
	char* cmdArgv[]={cmdPath,"/touchX.txt",NULL};
	char* cmdEnvp[]={"HOME=/","PATH=/sbin:/bin:/usr/bin",NULL};

	result = call_usermodehelper(cmdPath,cmdArgv,cmdEnvp,UMH_WAIT_PROC);
	printk("testDriver1_exit exec!Theresult of call_usermodehelper is %d\n",result);
	printk("testDriver1_exit exec!Theprocess is \"%s\",pidis %d \n",current->comm,current->pid);
}


module_init(testDriver1_init);
module_exit(testDriver1_exit);
MODULE_LICENSE("GPL");
