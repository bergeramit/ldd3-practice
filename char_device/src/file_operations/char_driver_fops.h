#ifndef CHAR_FOPS
#define CHAR_FOPS

#include <linux/version.h>

#include "../module_handlers/char_driver.h"
#include "../device_manager/device_manager.h"

extern CHAR_DRIVER__example_cdev_t my_cdev;

ssize_t example_write(struct file *a, const char __user *b, size_t c, loff_t *d);

ssize_t example_read(struct file *a, char __user *b, size_t c, loff_t *d);

long example_ioctl(struct file *f, unsigned int cmd, unsigned long arg);

int example_open(struct inode *inode, struct file *filp);

int example_release(struct inode *a, struct file *b);

#endif /* CHAR_FOPS */