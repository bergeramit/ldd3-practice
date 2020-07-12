#include "char_fops.h"

ssize_t example_write(struct file *, const char __user *, size_t, loff_t *) {
        printk(KERN_ALERT "write syscall called\n");
        return 0;
}

ssize_t example_read(struct file *, char __user *, size_t, loff_t *) {
        printk(KERN_ALERT "read syscall called\n");
        return 0;
}

long example_ioctl(struct file *f, unsigned int cmd, unsigned long arg) {
        printk(KERN_ALERT "ioctl syscall called\n");
        return 0;
}
