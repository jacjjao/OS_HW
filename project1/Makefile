obj-m += jiffies.o 
obj-m += seconds.o

PWD := $(CURDIR) 
 
all: jiffies.c seconds.c
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean: 
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean