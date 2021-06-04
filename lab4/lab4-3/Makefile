MODULE_NAME=led_dd
KDIR=/home/ubuntu/working/linux/
PWD=$(shell pwd)
TARGET=arm
TOOLCHAIN=arm-linux-gnueabihf-
APP_NAME=digit_app

obj-m:=$(MODULE_NAME).o

all: dd app

dd:
	$(MAKE) -C $(KDIR) M=$(PWD) ARCH=$(TARGET) CROSS_COMPILE=$(TOOLCHAIN) modules

app:
	arm-linux-gnueabihf-gcc -o $(APP_NAME) $(APP_NAME).c

clean:
	rm -f *.ko
	rm -f *.o
	rm -f *.mod.*
	rm -f modules.order
	rm -f Module.symvers
	rm -f .*.cmd
	rm -rf .tmp_versions
	rm -f $(APP_NAME)
