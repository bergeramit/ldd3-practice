#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>

MODULE_LICENSE("Dual BSD/GPL");

static char DEVICE_NAME[] = "char_device";
static int number_of_devices = 1;
static dev_t device_identifier = -1;

static int __init char_device_init(void) {
    int major, minor, return_code;

    /*
     * This function will allocate number for the char devices that this module/ driver will manage
     * 
     * device_identifier - an output variable that will store the major and minor
     * 0 - the first minor number we request
     * number_of_devices - how many devices we would like our char driver to manage
     * DEVICE_NAME - the name of the device (will appear in /proc/devices and will be assossiated with
     *               the major number)
     */
	return_code = alloc_chrdev_region(&device_identifier, 0, number_of_devices, DEVICE_NAME);

    if (0 != return_code) {
        printk(KERN_ALERT "Could Not Alloc char dev region\n");
        printk(KERN_ALERT "Error code: %d", return_code);
        goto Exit;
    }
    
    /*
     * The major number identifies the driver assossiated
     * with that device.
     * for example: /dev/zero and /dev/null are both managed by driver number 1
     *              their major is 1
     */ 
	major = MAJOR(device_identifier);

    /*
     * The minor number tells the kernel exactly which device is being referred to
     */
	minor = MINOR(device_identifier);

    printk(KERN_ALERT "major: %d, minor: %d\n", major, minor);

Exit:
    return 0;
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
