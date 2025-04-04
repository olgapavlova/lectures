.PHONY: all
.SILENT: clean

CURRENT = $(shell uname -r)
KDIR = /lib/modules/$(CURRENT)/build
PWD = $(shell pwd)

TARGET = silent
obj-m := $(TARGET).o

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	rm -f *.ko *.cmd .*.cmd *.mod .*.mod *.mod.* *.o *.order *.symvers
