#ifndef CHAR_FOPS
#define CHAR_FOPS

#include <linux/version.h>

ssize_t example_write(struct file *a, const char __user *b, size_t c, loff_t *d);

ssize_t example_read(struct file *a, char __user *b, size_t c, loff_t *d);

long example_ioctl(struct file *f, unsigned int cmd, unsigned long arg);

int example_open(struct inode *a, struct file *b);

int example_release(struct inode *a, struct file *b);

#endif /* CHAR_FOPS */