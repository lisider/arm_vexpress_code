#include <stdio.h>
#include <stdlib.h>


int main() {
	int n = 0;
	FILE * fp = NULL;
	char p[20] = {0};
	char p_out[20]= {0};
	//printf("请输入一个整数:\n");
	//fgets(p, 20, stdin); //从终端获取一个字符串
	fp = fopen("/sys/value/node_int","r");
	fgets(p, 20, fp); //从文件获取一个字符串
	fclose(fp);
	//n = str2int(p);      //把字符串转换成整型数
	n = atoi(p);      //把字符串转换成整型数
	printf("int:%d\n",n);
	snprintf(p_out,20,"%d",n);
	printf("str:%s\n",p_out);
	return 0;

}
