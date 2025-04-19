.PHONY: all
.SILENT: clean

CURRENT = $(shell uname -r)
KDIR = /lib/modules/$(CURRENT)/build
PWD = $(shell pwd)

CC = $(shell grep CONFIG_CC_VERSION /boot/config-$(CURRENT) | cut -d'"' -f2 | awk '{print $$1}')

TARGET = servo90_driver
# TARGET = led_uart_driver
obj-m := $(TARGET).o

all:
	$(MAKE) CC=$(CC) -C $(KDIR) M=$(PWD) modules

clean:
	rm -f *.ko *.cmd .*.cmd *.mod .*.mod *.mod.* *.o *.order *.symvers
