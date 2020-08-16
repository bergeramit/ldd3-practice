#include <linux/cdev.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/slab.h>

#include "device_manager.h"
#include "../logger/logger.h"

static __initdata char DRIVER_NAME[] = "sleep_device";

int __init DEVICE_MANAGER__setup_cdev(
    struct CHAR_DRIVER__example_cdev *first_cdev,
    struct file_operations *fops,
    dev_t char_device_identifier
){
    int rc = -1;
    /*
     * This cdev - charecter device is the structure responsible
     * for a specific charecter device i.e dev_t (a single major and minor == one file in /dev).
     * cdev_alloc allocates (with something like malloc) dynamically a cdev structure
     * struct cdev *first_cdev = cdev_alloc();
     * 
     * But we have already created cdev when we created the struct struct CHAR_DRIVER__example_cdev
     * so we just need to initialize the values with cdev_init()
     */ 
    cdev_init(&(first_cdev->cdev), fops);
    
    first_cdev->cdev.ops = fops;
    first_cdev->cdev.owner = THIS_MODULE;

    /*
     * Making sure the kernel knows which cdev is responsible to handle the charecter device
     * This function makes the connection between the device id (major and minor numbers) 
     * and the char device structure that handles this device (including file operations on this specific file)
     */
    rc = cdev_add(&(first_cdev->cdev), char_device_identifier, 1);

    return rc;
}

int __init DEVICE_MANAGER__setup_device_region(
    dev_t *region_identifier_out,
    int first_minor,
    int number_of_devices
){
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
        LOGGER__LOG_ERROR(rc, "Could Not Alloc char driver region with major: %d\n", CHAR_DRIVER__major);
        goto Exit;
    }

    *region_identifier_out = identifier;

Exit:
    return rc;
}

int DEVICE_MANAGER__init_cdev(struct  CHAR_DRIVER__example_cdev *cdev) {
    int rc = 0, i;
    if (NULL != cdev->stuff) {
        rc = 0;
        goto Exit;
    }

    /*
     *  Example use of kmalloc
     */
    cdev->stuff = (char *)kmalloc(DEVICE_MANAGER_OUTPUT_SIZE, GFP_KERNEL);
    if (NULL == cdev->stuff) {
        rc = -EFAULT;
        LOGGER__LOG_ERROR(rc, "Couldnt kmalloc the required space\n");
        goto Exit;
    }

    cdev->allocated_size = DEVICE_MANAGER_OUTPUT_SIZE;
    cdev->read_pos = 0;
    cdev->write_pos = 0;
    for (i = 0; i < DEVICE_MANAGER_OUTPUT_SIZE; i++) {
        *(cdev->stuff + i) = 0;
    }
    rc = 0;

Exit:
    return rc;
}

void DEVICE_MANAGER__free_cdev(struct CHAR_DRIVER__example_cdev *cdev) {
    if (cdev->stuff != NULL) {
        kfree(cdev->stuff);
        cdev->stuff = NULL;
    }
}
