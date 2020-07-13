#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>

#include "char_driver_fops.h"

MODULE_LICENSE("Dual BSD/GPL");

#define CHAR_DRIVER__MAJOR_DEFAULT (0)
#define CHAR_DRIVER__FIRST_MINOR_DEFAULT (0)

static char DEVICE_NAME[] = "char_device";
static int number_of_devices = 3;
static int char_driver__major = CHAR_DRIVER__MAJOR_DEFAULT;
static int char_driver__first_minor = CHAR_DRIVER__FIRST_MINOR_DEFAULT;

struct file_operations example_fops = {
    .owner = THIS_MODULE,
    .read = example_read,
    .write = example_write,
    .unlocked_ioctl = example_ioctl,
};

struct cdev *my_cdev = NULL;

module_param(char_driver__major, int, S_IRUGO);
module_param(char_driver__first_minor, int, S_IRUGO);

int setup_cdev(dev_t device_identifier) {

    /*
     * This cdev - charecter device is the structure responsible
     * for a specific charecter device i.e dev_t (a single major and minor)
     */ 
    my_cdev = cdev_alloc();
    my_cdev->ops = &example_fops;
    my_cdev.owner = THIS_MODULE;

    /*
     * Making sure the kernel knows which cdev is reponsible to handle the charecter device
     */
    rc = cdev_add(my_cdev, device_identifier, 1);

    return rc;
}

void print_device_numbers(dev_t device_identifier) {

    /*
     * The major number identifies the driver assossiated
     * with that device.
     * for example: /dev/zero and /dev/null are both managed by driver number 1
     *              their major is 1
     */ 
	char_driver__major = MAJOR(device_identifier);

    /*
     * The minor number tells the kernel exactly which device is being referred to
     */
	char_driver__first_minor = MINOR(device_identifier);

    printk(KERN_ALERT "major: %d, first minor: %d\n", char_driver__major, char_driver__first_minor);
}

int allocate_device_region(dev_t *first_device_id_out) {
    int rc = -1;
    if (char_driver__major) {

        *first_device_id_out = MKDEV(char_driver__major, 0);
        rc = register_chrdev_region(*first_device_id_out, number_of_devices, DEVICE_NAME);
    } else {

        /*
        * This function will allocate number for the char devices that this module/ driver will manage
        * 
        * device_identifier - an output variable that will store the major and minor
        * 0 - the first minor number we request
        * number_of_devices - how many devices we would like our char driver to manage
        * DEVICE_NAME - the name of the device (will appear in /proc/devices and will be assossiated with
        *               the major number)
        */
        rc = alloc_chrdev_region(first_device_id_out, 0, number_of_devices, DEVICE_NAME);
    }

    return rc;
}

static int __init char_device_init(void) {
    int rc = 0;
    dev_t first_device_id = -1;

    rc = allocate_device_region(&first_device_id);
    if (0 != rc) {
        printk(KERN_ALERT "Could Not Alloc char dev region with major: %d\n", char_driver__major);
        printk(KERN_NOTICE "Error code: %d", rc);
        *first_device_id_out = -1;
    }

    (void)print_device_numbers(first_device_id);
    rc = setup_cdev(first_device_id);

    if (rc) {
        printk(KERN_ALERT "Could not add char device", char_driver__major);
        printk(KERN_NOTICE "Error code: %d", rc);
    }

    return rc;
}

static void __exit char_device_exit(void) {
    if (-1 != device_identifier) {

        /*
         * This function unregisters the number assossiated with this driver/ module
         */
        unregister_chrdev_region(device_identifier, number_of_devices);
        printk(KERN_ALERT "Freed the device numbers\n");
    }
}


module_init(char_device_init);
module_exit(char_device_exit);

