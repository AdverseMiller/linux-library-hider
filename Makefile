# Makefile
obj-m += hide_maps.o
KDIR   := /lib/modules/$(shell uname -r)/build
CC     := gcc

all:
	$(MAKE) -C $(KDIR) M=$(PWD) CC=$(CC) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
