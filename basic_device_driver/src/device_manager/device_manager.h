#ifndef DEVICE_MANAGER
#define DEVICE_MANAGER

#include <linux/module.h>

#include "../module_handlers/module_handler.h"

extern int MODULE_HANDLER__major_g;

#define DEVICE_MANAGER__MAJOR_DEFAULT (0)
#define DEVICE_MANAGER_FIRST_MINOR_DEFAULT (0)
#define DEVICE_MANAGER_OUTPUT_SIZE (100)

struct DEVICE_MANAGER__example_cdev {
    struct cdev cdev;
    int id;
    char *stuff;
    int write_pos;
    int read_pos;
    int allocated_size;
};

int __init DEVICE_MANAGER__setup_cdev(
    struct DEVICE_MANAGER__example_cdev *example_cdev,
    struct file_operations *fops,
    dev_t char_device_identifier
);

int __init DEVICE_MANAGER__setup_device_region(
    dev_t *region_identifier_out,
    int first_minor,
    int number_of_devices
);

int DEVICE_MANAGER__init_cdev(struct  DEVICE_MANAGER__example_cdev *cdev);

void DEVICE_MANAGER__free_cdev(struct  DEVICE_MANAGER__example_cdev *cdev);

#endif /* DEVICE_MANAGER */