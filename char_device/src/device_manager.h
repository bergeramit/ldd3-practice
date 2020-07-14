#ifndef DEVICE_MANAGER
#define DEVICE_MANAGER

#include <linux/module.h>

extern int CHAR_DRIVER__major;

int __init setup_cdev(struct cdev **my_cdev,
               struct file_operations *fops,
               dev_t char_device_identifier);

int __init setup_device_region(dev_t *region_identifier_out,
                        int first_minor,
                        int number_of_devices);

#endif /* DEVICE_MANAGER */