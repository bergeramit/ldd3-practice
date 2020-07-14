#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kernel.h>

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

int example_open(struct inode *inode, struct file *filp) {
        CHAR_DRIVER__example_cdev_t *dev = NULL;
        printk(KERN_ALERT "open syscall called\n");
        
        /*
         * container_of function finds the struct that contains the same cdev
         * as the inode. This function get the cdev to look for, the type of the
         * struct that contains this cdev and the field name in that struct 
         * (which in this case is just cdev)
         * Example use here: dev = container_of(inode->i_cdev, CHAR_DRIVER__example_cdev_t, cdev);
         */ 

        /*
         * This technique uses the minor number to detect which cdev struct is responsible
         * for that inode
         */
        if (MINOR(inode->i_rdev) == MINOR(my_cdev.cdev.dev)) {
                filp->private_data = &my_cdev;
        }
        /* 
         * We will save it for easy access
         */ 
        filp->private_data = dev;
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
