#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>

#include "char_driver_fops.h"
#include "../logger/logger.h"
#include "../device_manager/device_manager.h"


struct file_operations example_fops = {
    .owner = THIS_MODULE,
    .read = example_read,
    .write = example_write,
    .open = example_open,
};

ssize_t example_write(
    struct file *filp,
    const char __user *usr_buf,
    size_t count,
    loff_t *pos
){
    LOGGER__LOG_DEBUG("Write was called\n");
    return count; /* success to avoid retry */
}

ssize_t example_read(
    struct file *filp,
    char __user *usr_buf,
    size_t count,
    loff_t *pos
){
    LOGGER__LOG_DEBUG("Read was called\n");
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
     * This technique uses the minor number to detect which cdev struct is responsible
     * for that inode
     */
    if (iminor(inode) == MINOR(DEVICE_MANAGER__cdev_g.cdev.dev)) {
        LOGGER__LOG_DEBUG("Init Successful (found cdev for the special file)\n");

        /* 
        * We will save it for easy access
        */ 
        filp->private_data = &DEVICE_MANAGER__cdev_g;
    }
    return 0;
}
