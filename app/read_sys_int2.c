#include <stdio.h>
#include <sys/sendfile.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void){
	ssize_t n = 0;
	int fd  = 0;

	fd = open("/sys/value/node_int",0666);
	while(1){
		//sendfile(int out_fd, int in_fd, off_t *offset, size_t count);
		n = sendfile(1,fd,NULL,32); // 1 为 标准输出
		if (n == 0)
			break;
	}
	return 0;
}

