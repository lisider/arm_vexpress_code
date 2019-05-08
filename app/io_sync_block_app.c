#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(void)
{
	char buf[20];
	int fd ;
	fd = open("/dev/hello_device",O_RDWR);
	read(fd,buf,20);
    printf("hello\n");
	close(fd);
	return 0;
}
