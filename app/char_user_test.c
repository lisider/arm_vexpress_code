#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	char buf[20];
	int fd ;
	fd = open("/dev/hello_class",O_RDWR);
	write(fd,"hello",5);
	read(fd,buf,20);
	close(fd);
	return 0;
}
