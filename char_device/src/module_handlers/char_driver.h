#ifndef CHAR_DRIVER
#define CHAR_DRIVER

#include <linux/cdev.h>

extern struct file_operations example_fops;

#define PRINT_ERROR_CODE(err) printk(KERN_ERR "Error code: %d\n", err)

#endif /* CHAR_DRIVER */