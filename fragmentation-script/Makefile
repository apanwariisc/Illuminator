obj-m += alloc-module.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	gcc user-alloc.c -lpthread -o user-alloc

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm user-alloc
