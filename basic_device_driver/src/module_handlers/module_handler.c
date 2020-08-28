#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>

#include "module_handler.h"
#include "../file_operations/char_driver_fops.h"
#include "../device_manager/device_manager.h"
#include "../logger/logger.h"

MODULE_LICENSE("Dual BSD/GPL");

int MODULE_HANDLER__major_g = DEVICE_MANAGER__MAJOR_DEFAULT;
struct DEVICE_MANAGER__example_cdev DEVICE_MANAGER__cdev_g = {0};

static int module_handler__number_of_devices = 1;
static int module_handler__first_minor = DEVICE_MANAGER_FIRST_MINOR_DEFAULT;
static bool module_handler__is_driver_alive = false;
static dev_t module_handler__region_identifier;

static int __init module_handler__init(void) {
    int rc = 0;
    dev_t first_char_device = 0;

    rc = DEVICE_MANAGER__setup_device_region(
            &module_handler__region_identifier,
            module_handler__first_minor,
            module_handler__number_of_devices
         );

    /*
     * In order to test the errors - uncomment this line :)
     * LOGGER__LOG_ERROR(rc, "Best error log eva\n");
     */

    if (0 != rc) {
        LOGGER__LOG_ERROR(rc, "Could not allocate chardev region\n");
        goto Exit;
    }

    LOGGER__log_device_numbers(
        module_handler__region_identifier,
        module_handler__number_of_devices
    );

     /*
     * The major number identifies the driver assossiated
     * with that device.
     * for example: /dev/zero and /dev/null are both managed by driver number 1
     *              their major is 1
     */ 
    MODULE_HANDLER__major_g = MAJOR(module_handler__region_identifier);

    /*
     * The minor number tells the kernel exactly which device is being referred to
     */
    module_handler__first_minor = MINOR(module_handler__region_identifier);

    /*
     * Registering the first device only
     */ 
    first_char_device = module_handler__region_identifier + module_handler__first_minor;
    rc = DEVICE_MANAGER__setup_cdev(
            &DEVICE_MANAGER__cdev_g,
            &example_fops,
            first_char_device
         );

    if (rc) {
        LOGGER__LOG_ERROR(rc, "Could not add char device\n");
        goto Cleanup;
    }

    DEVICE_MANAGER__init_cdev(&DEVICE_MANAGER__cdev_g);
    LOGGER__LOG_DEBUG("Device Ready to use!\n");
    module_handler__is_driver_alive = true;
    goto Exit;

Cleanup:
    unregister_chrdev_region(
        module_handler__region_identifier,
        module_handler__number_of_devices
    );
    LOGGER__LOG_DEBUG("Freed the device numbers\n");

Exit:
    return rc;
}

static void __exit module_handler__exit(void) {
    if (module_handler__is_driver_alive) {
        DEVICE_MANAGER__free_cdev(&DEVICE_MANAGER__cdev_g);
        /*
         * This function unregisters the number assossiated with this driver/ module
         */
        unregister_chrdev_region(
            module_handler__region_identifier,
            module_handler__number_of_devices
        );

        cdev_del(&(DEVICE_MANAGER__cdev_g.cdev));
        LOGGER__LOG_DEBUG("Freed the device region and device\n");
        module_handler__is_driver_alive = false;
    }
}

module_init(module_handler__init);
module_exit(module_handler__exit);