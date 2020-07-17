#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kernel.h>

#include "char_driver_fops.h"
#include "../logger/logger.h"

struct file_operations example_fops = {
    .owner = THIS_MODULE,
    .read = example_read,
    .write = example_write,
    .open = example_open,
    .release = example_release,
    .unlocked_ioctl = example_ioctl,
};

ssize_t example_write(struct file *a, const char __user *b, size_t c, loff_t *d) {
        LOGGER__LOG_DEBUG("write syscall called\n");
        return 0;
}

ssize_t example_read(struct file *a, char __user *b, size_t c, loff_t *d) {
        LOGGER__LOG_DEBUG("read syscall called\n");
        return 0;
}

/*
 * inode - the way the kernel represent files in the system,
 * if the fie is a special device file than this struct will
 * have the cdev struct (inode->i_cdev)
 * 
 * filp - file pointer this struct represent open files in the system
 * 
 */
int example_open(struct inode *inode, struct file *filp) {
        LOGGER__LOG_DEBUG("open syscall called\n");
        
        /*
         * container_of function finds the struct that contains the same cdev
         * as the inode. This function get the cdev to look for, the type of the
         * struct that contains this cdev and the field name in that struct 
         * (which in this case is just cdev)
         * Example use here: dev = container_of(inode->i_cdev, struct CHAR_DRIVER__example_cdev, cdev);
         * 
         */ 

        /*
         * This technique uses the minor number to detect which cdev struct is responsible
         * for that inode
         */
        if (iminor(inode) == MINOR(my_cdev.cdev.dev)) {
                LOGGER__LOG_DEBUG("Init Successful (found cdev for the special file)\n");

                /* 
                * We will save it for easy access
                */ 
                filp->private_data = &my_cdev;
        }

        return 0;
}

/*
 * The release method is called only when the refrence counter
 * for that inode reaches 0 - which means that when a process
 * forks and the open fd's are copied it will only update the inode
 * counter to 2 instead. This way when the child is dead and the
 * counter updates to 1 release will not be called even though 
 * there will be a call to "close" only the kernel will handle it
 * for us
 */
int example_release(struct inode *inode, struct file *filp) {
        LOGGER__LOG_DEBUG("release syscall called\n");
        return 0;
}

long example_ioctl(struct file *f, unsigned int cmd, unsigned long arg) {
        LOGGER__LOG_DEBUG("ioctl syscall called\n");
        return 0;
}
