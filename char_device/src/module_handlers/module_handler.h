#ifndef MODULE_HANDLER
#define MODULE_HANDLER

#include <linux/cdev.h>

extern struct file_operations example_fops;

extern struct DEVICE_MANAGER__access_control DEVICE_MANAGER__access_control_g;

#endif /* MODULE_HANDLER */