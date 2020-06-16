
obj-m := smccdev.o

EXTRA_CFLAGS=-I$(PWD)/smclib

smccdev-objs := \
	smccdev/main.o \
	smccdev/pipe.o \
	smccdev/access.o \
	smclib/smc_call.o

KERNEL_DIR ?= /home/aananth/projects/rpi3-tf-a/linux
COMPILER ?= /opt/tools/aarch64-none-linux/bin/aarch64-none-linux-gnu-

all:
	@echo "***************************************************************************************"
	@echo "PLEASE ADD THE FOLLOWING TO THE CMDLINE:"
	@echo "ARCH=arm64 CROSS_COMPILE=${COMPILER}"
	@echo "***************************************************************************************"

	@echo ""
	make -C ${KERNEL_DIR} M=$(PWD) modules
clean:
	make -C ${KERNEL_DIR} M=$(PWD) clean