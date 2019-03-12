#ifndef __SIMU_UART_H__
#define __SIMU_UART_H__

/*

通过对sys属性文件的读写，来模拟对tty硬件FIFO的读写
1.rx属性文件
对应模拟TTY硬件的TX-FIFO
2.tx属性文件
对应模拟TTY硬件的RX-FIFO

*/

/*define FIFO*/

#define V_FIFO_SIZE	16

struct v_fifo{
	char dat[V_FIFO_SIZE];
	int head;
	int tail;
};

struct vuart_dev{
	struct v_fifo v_TX_FIFO;
	struct v_fifo v_RX_FIFO;
	struct semaphore sem_tx;
	struct semaphore sem_rx;
};

/*计算有效数据size*/
int calc_valid_fifo(struct v_fifo* fifo);
/*计算空闲空间*/
int calc_free_fifo(struct v_fifo* fifo);
/*写fifo*/
int writechar_2_fifo(struct v_fifo* fifo,char cc);
/*读fifo*/
int readchar_from_fifo(struct v_fifo* fifo,char *cc);

struct vuart_dev * get_udev(void);


int xtjin_vuart_init(void);
void xtjin_vuart_exit(void);


#endif




