#include <linux/console.h>
#include <linux/platform_device.h>
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/serial_core.h>
#include <linux/serial.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <linux/module.h>
#include <linux/timer.h>


#include "simu_uart.h"


#define VUART_MAJOR			0	/* use dynamic node allocation */
#define VUART_MINOR_START	0	/* works best with devtmpfs */
#define	VUART_PORTS			1
#define PORT_VUARTS			117

#define VUART_BASE			0x42C00000
#define VUART_REGISTER_SPACE	0x10000

/*status bits*/
#define VUART_TX_FULL		0x01
#define VUART_RX_EMPTY		0x02
#define VUART_STATUS		0xF0

static struct uart_port		VUART_PORT[];

static struct timer_list vuart_timer;


static int vuart_TXFIFO_FULL(struct uart_port *port)
{
	struct vuart_dev * vuart = (struct vuart_dev*)port->private_data;
	if(!calc_free_fifo(&vuart->v_RX_FIFO))
		return 1;
	return 0;
}

static int writechar_2_vuart(struct uart_port *port, char cc)
{
	int ret;
	struct vuart_dev * vuart = (struct vuart_dev*)port->private_data;

	ret = writechar_2_fifo( &vuart->v_RX_FIFO, cc);
	if(ret)
		return -1;
	return 0;
}

static void vuart_start_tx(struct uart_port *port)
{
	char cc;
	while(!uart_circ_empty(&port->state->xmit))
	{
		printk("xmit is not empty.\n");
		if(vuart_TXFIFO_FULL(port))
			break;

		cc = port->state->xmit.buf[port->state->xmit.tail];
		if(writechar_2_vuart(port,cc))
			break;
		port->state->xmit.tail = (port->state->xmit.tail+1)&(UART_XMIT_SIZE-1);
	}
}

static void vuart_rx_handler(unsigned long arg)
{
	struct uart_port *port = (struct uart_port *)arg;
	struct vuart_dev * vuart = (struct vuart_dev*)port->private_data;
	int size , i;
	unsigned char cc;

	vuart_timer.expires = jiffies + HZ;
	add_timer(&vuart_timer);
	printk("timer  expires.\n");

	size = calc_valid_fifo(&vuart->v_TX_FIFO);

	if(!size)
		return;
	for(i=0;i<size;i++)
	{
		readchar_from_fifo( &vuart->v_TX_FIFO, &cc);
		tty_insert_flip_char(&port->state->port, cc, TTY_NORMAL);
	}

	tty_flip_buffer_push(&port->state->port);

	return;
}


static int vuart_startup(struct uart_port *port)
{
	int ret=0;

	/* ... */

	/* Request IRQ */
	//
	init_timer(&vuart_timer);
	vuart_timer.function = &vuart_rx_handler;
	vuart_timer.data	 = (unsigned long)port;
	vuart_timer.expires	 = jiffies + HZ;
	add_timer(&vuart_timer);
	/* ... */
	return ret;
}

static void vuart_shutdown(struct uart_port *port)
{
	/* ... */

	/* Free IRQ */
	//
	del_timer(&vuart_timer);

	/* Disable interrupt by writing to appropriate regs*/

	/* ... */
}

static const char * vuart_type(struct uart_port *port)
{
	return port->type == PORT_VUARTS ? "vUART" : NULL;
}

static int vuart_request_port(struct uart_port *port)
{
//	if (!request_mem_region(port->mapbase, VUART_REGISTER_SPACE,"vUART")){
//		return -EBUSY;
//	}
//	port->membase = ioremap(port->mapbase, VUART_REGISTER_SPACE);
	if (!request_mem_region(port->mapbase, 16, "vUART")) {
		dev_err(port->dev, "Memory region busy\n");
		return -EBUSY;
	}
	port->membase = ioremap(port->mapbase, 16);
	if (!port->membase) {
		dev_err(port->dev, "Unable to map registers\n");
		release_mem_region(port->mapbase, 16);
		return -EBUSY;
	}
	return 0;
}

static void vuart_release_port(struct uart_port *port)
{
	release_mem_region(port->mapbase, 16);
	if(port->membase)
		iounmap(port->membase);
	port->membase = NULL;
}

static void vuart_config_port(struct uart_port *port, int flags)
{
	if (vuart_request_port(port) == 0)
		port->type = PORT_VUARTS;
}



static unsigned int vuart_get_mctrl(struct uart_port *port)
{
	return TIOCM_CTS | TIOCM_DSR | TIOCM_CAR;
}

static void vuart_set_mctrl(struct uart_port *port, unsigned int mctrl)
{

}

static void vuart_set_termios(struct uart_port *port,
				struct ktermios *termios, struct ktermios *old)
{

}

static void vuart_stop_tx(struct uart_port *port)
{
	/* ... */

	/* Disable the transmitter */

}

static void vuart_stop_rx(struct uart_port *port)
{
	/* Disable RX IRQs */
	//writel(VUART_RX_IRQS, port->membase + offset);

	/* Disable the receiver */

	//writel(regval, port->membase + VUART_CR);
}

static unsigned int vuart_tx_empty(struct uart_port *port)
{
	unsigned int size;
	struct vuart_dev * vuart = (struct vuart_dev*)port->private_data;

	size = calc_valid_fifo(&vuart->v_RX_FIFO);
	if(!size)
		return 1;
	return 0;
}

static void vuart_break_ctl(struct uart_port *port, int ctl)
{

}

static void vuart_pm(struct uart_port *port, unsigned int state, unsigned int oldstate)
{
//	struct vuart_dev * vuart = (struct vuart_dev*)port->private_data;

	/* 增加对定时器的操作. added by xtjin */
}


static const struct uart_ops vuart_ops = {
		.set_mctrl	= vuart_set_mctrl,
		.get_mctrl	= vuart_get_mctrl,
		.start_tx	= vuart_start_tx,
		.stop_tx	= vuart_stop_tx,
		.stop_rx	= vuart_stop_rx,
		.tx_empty	= vuart_tx_empty,
		.break_ctl	= vuart_break_ctl,
		.set_termios	= vuart_set_termios,
		.startup	= vuart_startup,
		.shutdown	= vuart_shutdown,
		.pm 	= vuart_pm,
		.type		= vuart_type,
		.request_port	= vuart_request_port,
		.release_port	= vuart_release_port,
		.config_port	= vuart_config_port,
#ifdef CONFIG_CONSOLE_POLL_XTJIN
		.poll_get_char	= vuart_poll_get_char,
		.poll_put_char	= vuart_poll_put_char,
#endif

};

static struct uart_driver vuart_reg = {
	.owner			= THIS_MODULE,			/* Owner*/
	.driver_name	= "vuart",
	.dev_name		= "ttyVU",				/* node name */
	.major			= VUART_MAJOR,
	.minor			= VUART_MINOR_START,
	.nr				= VUART_PORTS,
#ifdef CONFIG_SERIAL_XTJIN_VUART_CONSOLE
	.cons			= &vuart_console,
#endif

};

static struct uart_port vuart_port[]={
	{
		.mapbase	= VUART_BASE,		/* memory mapped */
		.membase	= NULL,
		.iotype		= UPIO_MEM32,
		.fifosize	= 16,
		.ops		= &vuart_ops,
		.flags		= UPF_BOOT_AUTOCONF,
		.line		= 0,
	},
};

static int vuart_remove(struct platform_device *dev)
{
	platform_set_drvdata(dev,NULL);

	uart_remove_one_port(&vuart_reg, &vuart_port[dev->id]);
	return 0;
}

static int vuart_suspend(struct platform_device *dev, pm_message_t state)
{
	uart_suspend_port(&vuart_reg, &vuart_port[dev->id]);
	return 0;
}

static int vuart_resume(struct platform_device *dev)
{
	uart_resume_port(&vuart_reg, &vuart_port[dev->id]);
	return 0;
}


static int vuart_probe(struct platform_device * dev)
{
	/* ... */

	uart_add_one_port(&vuart_reg, &vuart_port[dev->id]);
	platform_set_drvdata( dev, &vuart_port[dev->id]);

	vuart_port[dev->id].private_data =  get_udev();
	return 0;
}


static struct platform_device * vuart_plat_device;

static struct platform_driver vuart_plat_drv={
	.probe		= vuart_probe,
	.remove		= vuart_remove,
	.suspend	= vuart_suspend,
	.resume		= vuart_resume,
	.driver		= {
		.name		= "vuart"			/* driver name */
	},

};

static int __init vuart_init(void)
{
	int retval;

	if((retval = xtjin_vuart_init()))
		return  retval;

	if((retval = uart_register_driver( &vuart_reg )))
		return retval;
	vuart_plat_device = platform_device_register_simple("vuart", 0, NULL, 0);
	if(IS_ERR(vuart_plat_device))
	{
		uart_unregister_driver(&vuart_reg);
		return PTR_ERR(vuart_plat_device);
	}

	if((retval = platform_driver_register(&vuart_plat_drv))){
		uart_unregister_driver(&vuart_reg);
		platform_device_unregister(vuart_plat_device);
	}
	return 0;

}


static void __exit vuart_exit(void)
{
	platform_driver_unregister(&vuart_plat_drv);

	platform_device_unregister(vuart_plat_device);

	uart_unregister_driver(&vuart_reg);

	xtjin_vuart_exit();
}


module_init(vuart_init);
module_exit(vuart_exit);

MODULE_DESCRIPTION("Driver for vUART");
MODULE_AUTHOR("xtjin.");
MODULE_LICENSE("GPL");

