obj-m += char_driver.o

module = char_driver
device = driver_dev_name

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

run_and_register:
	sudo insmod char_driver.ko $* || exit 1
	$(eval MAJOR := $(shell awk "{if (\$$2 == \"$(module)\") print \$$1}" /proc/devices))
	mknod /dev/$(device)0 c $(MAJOR) 0

remove:
	sudo rmmod char_driver.ko

