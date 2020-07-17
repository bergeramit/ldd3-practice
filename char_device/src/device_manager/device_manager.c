#include <linux/cdev.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>

#include "device_manager.h"
#include "../device_print.h"

static __initdata char DRIVER_NAME[] = "char_device";

int __init setup_cdev(CHAR_DRIVER__example_cdev_t *my_cdev,
                      struct file_operations *fops,
                      dev_t char_device_identifier)
{
    int rc = -1;
    /*
     * This cdev - charecter device is the structure responsible
     * for a specific charecter device i.e dev_t (a single major and minor == one file in /dev).
     * cdev_alloc allocates (with something like malloc) dynamically a cdev structure
     * struct cdev *my_cdev = cdev_alloc();
     * 
     * But we have already created cdev when we created the struct CHAR_DRIVER__example_cdev_t
     * so we just need to initialize the values with cdev_init()
     */ 
    cdev_init(&(my_cdev->cdev), fops);
    
    my_cdev->cdev.ops = fops;
    my_cdev->cdev.owner = THIS_MODULE;

    /*
     * Making sure the kernel knows which cdev is responsible to handle the charecter device
     * This function makes the connection between the device id (major and minor numbers) 
     * and the char device structure that handles this device (including file operations on this specific file)
     */
    rc = cdev_add(&(my_cdev->cdev), char_device_identifier, 1);

    return rc;
}

int __init setup_device_region(dev_t *region_identifier_out,
                               int first_minor,
                               int number_of_devices)
{
    int rc = -1;
    dev_t identifier;
    if (CHAR_DRIVER__major) {

        identifier = MKDEV(CHAR_DRIVER__major, first_minor);
        rc = register_chrdev_region(identifier, number_of_devices, DRIVER_NAME);
    } else {

        /*
        * This function will allocate number for the char devices that this module/ driver will manage
        * 
        * device_identifier - an output variable that will store the major and minor
        * 0 - the first minor number we request
        * number_of_devices - how many devices we would like our char driver to manage
        * DEVICE_NAME - the name of the driver (will appear in /proc/devices and will be assossiated with
        *               the major number)
        */
        rc = alloc_chrdev_region(&identifier, 0, number_of_devices, DRIVER_NAME);
    }

    if (0 != rc) {
        printk(KERN_ALERT "Could Not Alloc char driver region with major: %d\n", CHAR_DRIVER__major);
        PRINT_ERROR_CODE(rc);
        goto Exit;
    }

    *region_identifier_out = identifier;

Exit:
    return rc;
}