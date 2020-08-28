# Comment this line to disable debug prints
DEBUG = y

obj-m += sleep_driver.o
sleep_driver-objs := src/file_operations/char_driver_fops.o \
					src/module_handlers/module_handler.o \
					src/device_manager/device_manager.o \
					src/logger/logger.o

ifeq ($(DEBUG), y)
	
	#
	# if we want specific flags for a specific object file
	# CFLAGS_char_driver.o := -DDEBUG -g
	#
	ccflags-y += -g -DDEBUG
endif

module = sleep_device
device = sleep_dev_
mode = "777"

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

run:
	sudo insmod sleep_driver.ko || exit 1

register:
	$(eval MAJOR := $(shell awk "{if (\$$2 == \"$(module)\") print \$$1}" /proc/devices))
	sudo mknod /dev/$(device)0 c $(MAJOR) 0
	sudo chmod $(mode) /dev/$(device)0

remove:
	sudo rmmod sleep_driver.ko
	sudo rm -fr /dev/$(device)0

purge: remove clean

