#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define check_read() \
    do{\
        if (n < 0){\
            perror("read");\
        } else{\
            printf("buf read :%d,%s\n",n,buf);\
        }\
    }while(0)
#define check_write() \
    do{\
        if (n < 0){\
            perror("write");\
        } else{\
            printf("buf write :%d\n",n);\
        }\
    }while(0)

int get_cpsr(void){
    int tmp;
	asm volatile (
		"mrs r3,cpsr\n\t"
		"mov %0,r3\n\t"
        : "=r" (tmp)
        :
	);
    return tmp;
}

int print_cpsr(void){
    printf("usr space:0x%x\n",get_cpsr());
}

int main(void)
{
	char buf[32];
	int fd = 0;
    int n  = 0;

    print_cpsr();
	fd = open("/dev/hello_device",O_RDWR);
    if (fd < 0){
        perror("open");
    }

	n = read(fd,buf,32);
    check_read();

    lseek(fd,0,SEEK_SET); // 从 SEEK_SET 移动 0 // SEEK_CUR  // SEEK_END

	n = read(fd,buf,32);
    check_read();

    lseek(fd,0,SEEK_END); // 从 SEEK_SET 移动 0 // SEEK_CUR  // SEEK_END

	n = read(fd,buf,32);
    check_read();


    lseek(fd,0,SEEK_SET); // 从 SEEK_SET 移动 0 // SEEK_CUR  // SEEK_END


    n = write(fd,"hello2",6);
    check_write();

    memset(buf,0,32);
	read(fd,buf,32);
    check_read();

	close(fd);
	return 0;
}
