#ifndef DEVICE_MANAGER
#define DEVICE_MANAGER

#include <linux/module.h>

#include "../module_handlers/char_driver.h"

extern int CHAR_DRIVER__major;

#define DEVICE_MANAGER__MAJOR_DEFAULT (0)
#define DEVICE_MANAGER_FIRST_MINOR_DEFAULT (0)

typedef struct CHAR_DRIVER__example_cdev {
    struct cdev cdev;
    int id;
    char stuff[10];
};

int __init DEVICE_MANAGER__setup_cdev(
    struct CHAR_DRIVER__example_cdev *my_cdev,
    struct file_operations *fops,
    dev_t char_device_identifier
);

int __init DEVICE_MANAGER__setup_device_region(
    dev_t *region_identifier_out,
    int first_minor,
    int number_of_devices
);

#endif /* DEVICE_MANAGER */