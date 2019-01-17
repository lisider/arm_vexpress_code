#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>   /* For SYS_xxx definitions */
int main(void){
	int ret;
	ret = syscall(388,1,2);
	printf("ret : %d\n",ret);
	return 0;
}
