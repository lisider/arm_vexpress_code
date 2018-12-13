.PHONY:all clean
KERNEL_ROOT=/home/suweishuai/wlt_kernel_debug/linux-4.0
ifneq ($(KERNELRELEASE),)

obj-m := class_test.o

else

EXTRA_CFLAGS += -DDEBUG
KDIR := $(KERNEL_ROOT)
all:
		make  CROSS_COMPILE=arm-linux-gnueabi- ARCH=arm -C $(KDIR) M=$(PWD) modules && cp *.ko ../../../../rootfs/work
clean:
		rm -fr *.ko *.o *.mod.o *.mod.c *.symvers *.order .*.ko .tmp_versions

endif
