#
# Mkefile for miscellaneous device
#
PWD         := $(shell pwd)
KVERSION    := $(shell uname -r)
KERNEL_DIR   = /lib/modules/$(KVERSION)/build
#KERNEL_DIR   = /usr/src/linux-headers-$(KVERSION)/

#
# module name and path
#
MODULE = test_misc_register

#
# CFLAGS
#
MODULE_CFLAGS += -Wall -Werror

#SRCS-y := test_misc_register.c
obj-m       := $(MODULE).o

all:
	make -C $(KERNEL_DIR) M=$(PWD) modules
clean:
	make -C $(KERNEL_DIR) M=$(PWD) clean
