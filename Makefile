MODULE_FILENAME=practica2
obj-m += practica2v2.o

all:
	$(MAKE) -C /lib/modules/`uname -r`/build M=$(PWD) modules

clean:
	$(MAKE) -C /lib/modules/`uname -r`/build M=$(PWD) clean
	$(RM) Module.markers modules.order
	
remove:
	sudo rmmod $(MODULE_FILENAME)