#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>

MODULE_LICENSE("Dual BSD/GPL - can be anything");

static char DEVICE_NAME[] = "char_device";
static int number_of_devices = 1;
static dev_t device_identifier;

static int __init char_device_init(void) {
    int major, minor;
	alloc_chrdev_region(&device_identifier, 0, number_of_devices, DEVICE_NAME);
	major = MAJOR(device_identifier);
	minor = MINOR(device_identifier);

    printk(KERN_ALERT "major: %d, minor: %d\n", major, minor);
    return 0;
}

static void __exit char_device_exit(void) {
    printk(KERN_ALERT "Bye!!\n");
}


module_init(char_device_init);
module_exit(char_device_exit);
