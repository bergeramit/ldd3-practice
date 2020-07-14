#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>

#include "char_driver.h"
#include "char_driver_fops.h"
#include "device_manager.h"

MODULE_LICENSE("Dual BSD/GPL");

#define CHAR_DRIVER__MAJOR_DEFAULT (0)
#define CHAR_DRIVER__FIRST_MINOR_DEFAULT (0)

int char_driver__major = CHAR_DRIVER__MAJOR_DEFAULT;

static int number_of_devices = 3;
static int char_driver__first_minor = CHAR_DRIVER__FIRST_MINOR_DEFAULT;
static bool is_device_alive = false;
struct cdev *first_cdev = NULL;
static dev_t region_identifier;

module_param(char_driver__major, int, S_IRUGO);
module_param(char_driver__first_minor, int, S_IRUGO);


void print_device_numbers(void) {

    /*
     * The major number identifies the driver assossiated
     * with that device.
     * for example: /dev/zero and /dev/null are both managed by driver number 1
     *              their major is 1
     */ 
	char_driver__major = MAJOR(region_identifier);

    /*
     * The minor number tells the kernel exactly which device is being referred to
     */
	char_driver__first_minor = MINOR(region_identifier);

    printk(KERN_ALERT "major: %d, first minor: %d\n", char_driver__major, char_driver__first_minor);
}


static int __init char_device_init(void) {
    int rc = 0;
    dev_t first_char_device = 0;

    rc = setup_device_region(&region_identifier, char_driver__first_minor, number_of_devices);
    if (0 != rc) {
        printk(KERN_ALERT "Could not allocate chardev region\n");
        goto Exit;
    }

    (void)print_device_numbers();
    first_char_device = region_identifier + char_driver__first_minor;
    rc = setup_cdev(&first_cdev, &example_fops, first_char_device);

    if (rc) {
        printk(KERN_ALERT "Could not add char device\n");
        PRINT_ERROR(rc);
        goto Cleanup;
    }

    printk(KERN_ALERT "Device Ready to use!\n");
    is_device_alive = true;
    goto Exit;

Cleanup:
    unregister_chrdev_region(region_identifier, number_of_devices);
    printk(KERN_ALERT "Freed the device numbers\n");

Exit:
    return rc;
}

static void __exit char_device_exit(void) {
    if (is_device_alive) {

        /*
         * This function unregisters the number assossiated with this driver/ module
         */
        unregister_chrdev_region(region_identifier, number_of_devices);
        cdev_del(first_cdev);
        printk(KERN_ALERT "Freed the device region and device\n");
        is_device_alive = false;
    }
}

module_init(char_device_init);
module_exit(char_device_exit);
