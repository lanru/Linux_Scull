obj-m	:= usb-skeleton.o

#KERNELDIR ?= /lib/modules/$(shell uname -r)/build
KERNELDIR ?= /root/linux-5.10.106
PWD       := $(shell pwd)

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD)

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions

