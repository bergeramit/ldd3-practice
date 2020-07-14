#ifndef CHAR_DRIVER
#define CHAR_DRIVER

#include <linux/cdev.h>

extern struct file_operations example_fops;

#define PRINT_ERROR(err) printk(KERN_NOTICE "Error code: %d\n", err)

typedef struct CHAR_DRIVER__example_cdev {
    struct cdev cdev;
    int id;
    char stuff[10];
} CHAR_DRIVER__example_cdev_t;

void print_device_numbers(void);


#endif /* CHAR_DRIVER */