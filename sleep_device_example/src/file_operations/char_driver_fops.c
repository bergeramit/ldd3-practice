#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/wait.h>

#include "char_driver_fops.h"
#include "../logger/logger.h"
#include "../device_manager/device_manager.h"

static DECLARE_WAIT_QUEUE_HEAD(sleep_q);
static int flag = 0;

struct file_operations example_fops = {
    .owner = THIS_MODULE,
    .read = sleep_read,
    .write = sleep_write,
    .open = example_open,
};

ssize_t sleep_write(
    struct file *filp,
    const char __user *usr_buf,
    size_t count,
    loff_t *pos
){
    printk(KERN_DEBUG "process %i (%s) wake everyone up...\n", current->pid, current->comm);
    flag = 1;

    /*
     * You must call wake up after the predicate was satisfied
     */
    wake_up_interruptible(&sleep_q);
    return count; /* success to avoid retry */
}

ssize_t sleep_read(
    struct file *filp,
    char __user *usr_buf,
    size_t count,
    loff_t *pos
){
     printk(KERN_DEBUG "process %i (%s) going to sleep\n", current->pid, current->comm);
     wait_event_interruptible(sleep_q, flag != 0);

     /*
      * Assuming the flag was set (that is why we are not sleeping anymore)
      */
     flag = 0;
     printk(KERN_DEBUG "awoken %i (%s)\n", current->pid, current->comm);
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
