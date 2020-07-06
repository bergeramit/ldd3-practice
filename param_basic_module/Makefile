obj-m += param_example.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean


run_with_args:
	sudo insmod param_example.ko magic_string="cmd arg!" numbers=1,2,3,4

run_with_default:
	sudo insmod param_example.ko

remove:
	sudo rmmod param_example.ko

