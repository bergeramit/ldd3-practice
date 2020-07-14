#ifndef CHAR_DRIVER
#define CHAR_DRIVER

extern struct file_operations example_fops;

#define PRINT_ERROR(err) printk(KERN_NOTICE "Error code: %d\n", err)

void print_device_numbers(void);


#endif /* CHAR_DRIVER */