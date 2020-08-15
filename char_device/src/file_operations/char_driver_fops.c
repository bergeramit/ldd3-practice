#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>

#include "char_driver_fops.h"
#include "../logger/logger.h"
#include "../device_manager/device_manager.h"

#define CHAR_DRIVER_IOCTL_MAGIC 'B'

/*
 * The first param is the magic we define that lets the entire system know
 * where to direct the ioctl command with this magic (here :)).
 * The second param is for internal use to let use couple types or operations
 * with an internal number (usually a sequence number like here).
 * Last param is the datatype used by the user to call ioctl. can be anything
 *
 */
#define CHAR_DEVICE_IOCTL_RESET _IO(CHAR_DRIVER_IOCTL_MAGIC, 0)
#define CHAR_DEVICE_IOCTL_SPECIAL_READ _IOR(CHAR_DRIVER_IOCTL_MAGIC, 1, int)
#define CHAR_DEVICE_IOCTL_SPECIAL_WRITE _IOW(CHAR_DRIVER_IOCTL_MAGIC, 2, int)

struct file_operations example_fops = {
    .owner = THIS_MODULE,
    .read = example_read,
    .write = example_write,
    .open = example_open,
    .release = example_release,
    .unlocked_ioctl = example_ioctl,
};

ssize_t example_write(
    struct file *filp,
    const char __user *usr_buf,
    size_t count,
    loff_t *pos
){
    int rc = 0;
    unsigned long still_to_copy = 0;
    struct CHAR_DRIVER__example_cdev *cdev = filp->private_data;
    LOGGER__LOG_DEBUG("write syscall called\n");
    if (cdev->allocated_size < cdev->write_pos + count) {
        rc = -1;
        LOGGER__LOG_ERROR(
            rc,
            "Not enough space to write, allocated: %d, pos: %d, asked to read: %ld\n",
            cdev->allocated_size,
            cdev->write_pos,
            count
        );
        goto Exit;
    }
    still_to_copy = copy_from_user(cdev->stuff + cdev->write_pos, usr_buf, count);
    if (0 != still_to_copy) {
        rc = -EFAULT;
        LOGGER__LOG_ERROR(rc, "Couldnt write to user buffer\n");
        goto Exit;
    }
    cdev->write_pos += count;
    rc = count;

Exit:
    return rc;
}

ssize_t example_read(
    struct file *filp,
    char __user *usr_buf,
    size_t count,
    loff_t *pos
){
    int rc = 0;
    unsigned long still_to_copy = 0;
    struct CHAR_DRIVER__example_cdev *cdev = filp->private_data;

    if (cdev->allocated_size < cdev->read_pos + count) {
        rc = -1;
        LOGGER__LOG_ERROR(
            rc,
            "Not enough space to read, allocated: %d, pos: %d, asked to read: %ld\n",
            cdev->allocated_size,
            cdev->read_pos,
            count
        );
        goto Exit;
    }
    LOGGER__LOG_DEBUG("read syscall called\n");
    still_to_copy = copy_to_user(usr_buf, cdev->stuff + cdev->read_pos, count);
    if (0 != still_to_copy) {
        rc = -EFAULT;
        LOGGER__LOG_ERROR(rc, "Couldnt write to user buffer\n");
        goto Exit;
    }
    cdev->read_pos += count;
    rc = count;

Exit:
    return rc;
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
    if (iminor(inode) == MINOR(first_cdev.cdev.dev)) {
        LOGGER__LOG_DEBUG("Init Successful (found cdev for the special file)\n");
        
        /* 
        * We will save it for easy access
        */ 
        filp->private_data = &first_cdev;
    }

    return 0;
}

/*
 * The release method is called only when the reference counter
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

    switch (cmd) {
        case CHAR_DEVICE_IOCTL_RESET:
            LOGGER__LOG_DEBUG("Request for Reset call\n");
            break;
        case CHAR_DEVICE_IOCTL_RESET:
            LOGGER__LOG_DEBUG("Request for Reset call\n");
            break;
        case CHAR_DEVICE_IOCTL_SPECIAL_READ:
            LOGGER__LOG_DEBUG("Request for special read call\n");
            break;
        case CHAR_DEVICE_IOCTL_SPECIAL_WRITE:
            LOGGER__LOG_DEBUG("Request for special write call\n");
            break;
    }
    return 0;
}
