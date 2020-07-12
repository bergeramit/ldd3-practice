#ifndef CHAR_FOPS
#define CHAR_FOPS

#include <linux/version.h>

ssize_t example_write(struct file *, const char __user *, size_t, loff_t *);

ssize_t example_read(struct file *, char __user *, size_t, loff_t *);

long example_ioctl(struct file *f, unsigned int cmd, unsigned long arg);

#endif /* CHAR_FOPS */