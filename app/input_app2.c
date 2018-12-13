#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <linux/input.h>

int main(int argc, const char *argv[])

{

    int fd;

    struct input_event env;

    int c = 0;

    int i;

    fd =open(argv[1],0666);

    if(fd<=0)

    {

        puts("open error");

        return -1;

    }

    while(1)

    {

        c=read(fd,&env,sizeof(struct input_event));

        if(c<0)

        {

            perror("read error");

            return -1;

        }

        printf("type:%d code:%d value:%d\n",

                env.type,

                env.code,

                env.value);

    }

    return 0;

}
