all:

ifeq ("$(origin O)", "undefined")
		@echo "please input make O=xxx , xxx is object"
else
	arm-linux-gnueabi-gcc ${O}.c -o ${O}
	mv ${O} ../../busybox/rootfs/work/
endif

clean:
	rm ${O} -rf
