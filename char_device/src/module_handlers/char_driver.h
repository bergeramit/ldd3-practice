#ifndef CHAR_DRIVER
#define CHAR_DRIVER

#include <linux/cdev.h>

extern struct file_operations example_fops;

extern struct DEVICE_MANAGER__access_control DEVICE_MANAGER__access_control_g;

#endif /* CHAR_DRIVER */