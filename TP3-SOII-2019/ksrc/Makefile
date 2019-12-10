MODSRC=/var/www/html/ksrc
obj-m+=helloWorld.o

all: module

module:
	make -C /lib/modules/$(shell uname -r)/build M=${MODSRC} modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=${MODSRC} clean

up:
	sudo insmod helloWorld.ko

down:
	sudo rmmod helloWorld.ko
