# Comment this line to disable debug prints
DEBUG = y

obj-m += char_driver.o
char_driver-objs := ./src/file_operations/char_driver_fops.o ./src/module_handlers/char_driver.o ./src/device_manager/device_manager.o

ifeq ($(DEBUG), y)
	
	#
	# if we want specific flags for a specific object file
	# CFLAGS_char_driver.o := -DDEBUG -g
	#
	ccflags-y += -g -DDEBUG
endif

module = char_device
device = driver_dev_name
mode = "664"

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

run:
	sudo insmod char_driver.ko || exit 1

register:
	$(eval MAJOR := $(shell awk "{if (\$$2 == \"$(module)\") print \$$1}" /proc/devices))
	sudo mknod /dev/$(device)0 c $(MAJOR) 0
	sudo mknod /dev/$(device)1 c $(MAJOR) 1
	sudo mknod /dev/$(device)2 c $(MAJOR) 2
	sudo chmod $(mode) /dev/$(device)[0-2]

remove:
	sudo rmmod char_driver.ko
	sudo rm -fr /dev/$(device)[0-2]

purge: remove clean

