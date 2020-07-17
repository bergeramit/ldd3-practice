#ifndef DEVICE_MANAGER
#define DEVICE_MANAGER

#include <linux/module.h>

#include "../module_handlers/char_driver.h"

extern int CHAR_DRIVER__major;

typedef struct CHAR_DRIVER__example_cdev {
    struct cdev cdev;
    int id;
    char stuff[10];
} CHAR_DRIVER__example_cdev_t;

int __init setup_cdev(CHAR_DRIVER__example_cdev_t *my_cdev,
                      struct file_operations *fops,
                      dev_t char_device_identifier);

int __init setup_device_region(dev_t *region_identifier_out,
                               int first_minor,
                               int number_of_devices);

#endif /* DEVICE_MANAGER */