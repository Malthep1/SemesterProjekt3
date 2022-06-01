#FILE: Makefile (WITH CAPITAL M!!)
obj-m := button.o
ccflags-y := -std=gnu99 -Wno-declaration-after-statement

KERNELDIR ?= ~/sources/rpi-5.4.83

all default: modules
install: modules_install

modules modules_install help clean:
	$(MAKE) ARCH=arm CROSS_COMPILE=arm-poky-linux-gnueabi- -C $(KERNELDIR) M=$(shell pwd) $@

	