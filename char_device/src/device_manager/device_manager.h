#ifndef DEVICE_MANAGER
#define DEVICE_MANAGER

#include <linux/module.h>

#include "../module_handlers/char_driver.h"

extern int CHAR_DRIVER__major;

#define DEVICE_MANAGER__MAJOR_DEFAULT (0)
#define DEVICE_MANAGER_FIRST_MINOR_DEFAULT (0)
#define DEVICE_MANAGER_OUTPUT_SIZE (100)

struct CHAR_DRIVER__example_cdev {
    struct cdev cdev;
    int id;
    char *stuff;
    int write_pos;
    int read_pos;
    int allocated_size;
};

int __init DEVICE_MANAGER__setup_cdev(
    struct CHAR_DRIVER__example_cdev *first_cdev,
    struct file_operations *fops,
    dev_t char_device_identifier
);

int __init DEVICE_MANAGER__setup_device_region(
    dev_t *region_identifier_out,
    int first_minor,
    int number_of_devices
);

int DEVICE_MANAGER__init_cdev(struct  CHAR_DRIVER__example_cdev *cdev);

void DEVICE_MANAGER__free_cdev(struct  CHAR_DRIVER__example_cdev *cdev);

#endif /* DEVICE_MANAGER */