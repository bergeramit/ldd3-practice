#ifndef CHAR_FOPS
#define CHAR_FOPS

#include <linux/version.h>
#include <linux/poll.h>

#include "../module_handlers/char_driver.h"
#include "../device_manager/device_manager.h"

extern struct DEVICE_MANAGER__example_cdev first_cdev;

ssize_t example_write(struct file *filp, const char __user *b, size_t c, loff_t *d);

ssize_t example_read(struct file *filp, char __user *usr_buf, size_t count, loff_t *pos);

long example_ioctl(struct file *f, unsigned int cmd, unsigned long arg);

int example_open(struct inode *inode, struct file *filp);

int example_release(struct inode *a, struct file *b);

unsigned int example_poll(struct file *filp, poll_table *wait);

#endif /* CHAR_FOPS */