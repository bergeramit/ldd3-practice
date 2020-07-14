#include <linux/module.h>
#include <linux/fs.h>

#include "char_driver_fops.h"

struct file_operations example_fops = {
    .owner = THIS_MODULE,
    .read = example_read,
    .write = example_write,
    .open = example_open,
    .release = example_release,
    .unlocked_ioctl = example_ioctl,
};

ssize_t example_write(struct file *a, const char __user *b, size_t c, loff_t *d) {
        printk(KERN_ALERT "write syscall called\n");
        return 0;
}

ssize_t example_read(struct file *a, char __user *b, size_t c, loff_t *d) {
        printk(KERN_ALERT "read syscall called\n");
        return 0;
}

int example_open(struct inode *a, struct file *b) {
        printk(KERN_ALERT "open syscall called\n");
        return 0;
}

int example_release(struct inode *a, struct file *b) {
        printk(KERN_ALERT "release syscall called\n");
        return 0;
}

long example_ioctl(struct file *f, unsigned int cmd, unsigned long arg) {
        printk(KERN_ALERT "ioctl syscall called\n");
        return 0;
}
