# Comment this line to disable debug prints
DEBUG = y

obj-m += driver_module_name.o
driver_module_name-objs := src/file_operations/char_driver_fops.o \
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

module = device_driver_name
device = special_device_file_name_
mode = "777"

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

run:
	sudo insmod driver_module_name.ko || exit 1

register:
	$(eval MAJOR := $(shell awk "{if (\$$2 == \"$(module)\") print \$$1}" /proc/devices))
	sudo mknod /dev/$(device)0 c $(MAJOR) 0
	sudo chmod $(mode) /dev/$(device)0

remove:
	sudo rmmod driver_module_name.ko
	sudo rm -fr /dev/$(device)0

purge: remove clean

