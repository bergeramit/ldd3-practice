#include <linux/module.h>
#include <linux/fs.h>

#include "char_driver_fops.h"

ssize_t example_write(struct file *a, const char __user *b, size_t c, loff_t *d) {
        printk(KERN_ALERT "write syscall called\n");
        return 0;
}

ssize_t example_read(struct file *a, char __user *b, size_t c, loff_t *d) {
        printk(KERN_ALERT "read syscall called\n");
        return 0;
}

long example_ioctl(struct file *f, unsigned int cmd, unsigned long arg) {
        printk(KERN_ALERT "ioctl syscall called\n");
        return 0;
}
