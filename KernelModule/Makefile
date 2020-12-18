#memo_201403904
obj-m += cpu_201403904.o
obj-m += memo_201403904.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

