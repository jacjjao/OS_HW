CONFIG_MODULE_SIG=n

obj-m += jiffies.o 
 
PWD := $(CURDIR) 
 
all: jiffies.c
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean: 
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean