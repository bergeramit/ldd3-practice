obj-m += hello.o


all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm no_commas_example va_list_example

test_no_commas:
	gcc no_commas_example.c -o no_commas_example

test_va_args:
	gcc va_list_example.c -o va_list_example
