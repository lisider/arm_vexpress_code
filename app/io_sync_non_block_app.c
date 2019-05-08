#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(void)
{
	char buf[20];
	int fd ;
    int ret = 0;
	fd = open("/dev/hello_device",O_RDWR|O_NONBLOCK);
    //write(fd,"hello",5);
	ret = read(fd,buf,20);
    if(ret < 0 )
        perror("read:");
    else
        printf("result,%d\n",*(int *)buf);
	close(fd);
	return 0;
}
