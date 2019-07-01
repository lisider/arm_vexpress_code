/*************************************************************************
  > File Name: get_cpsr.c
  > Author: Sues
  > Mail: sumory.kaka@foxmail.com 
  > Created Time: Sun 30 Jun 2019 06:20:52 PM PDT
 ************************************************************************/

#include <stdio.h>


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

int main(void){
    printf("%x\n",get_cpsr());
}
