CURRENT = $(shell uname -r)
KDIR = /lib/modules/$(CURRENT)/build
PWD = $(shell pwd)

CC = $(shell grep CONFIG_CC_VERSION /boot/config-$(CURRENT) | cut -d'"' -f2 | awk '{print $$1}')

%.run:
	-sudo rmmod $*
	$(MAKE) TARGET=$* CC=$(CC) -C $(KDIR) M=$(PWD) modules
	@sudo insmod $*.ko

obj-m := $(TARGET).o

clear: clean

clean:
	@rm -f *.ko *.cmd .*.cmd *.mod .*.mod *.mod.* *.o *.order *.symvers *.out
