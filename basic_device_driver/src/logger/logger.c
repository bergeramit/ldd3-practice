#include <stdarg.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>

#include "logger.h"

/*
 * printk_ratelimit - This function should be called
 * before you consider printing a message that could be repeated often.
 * If the function returns a nonzero value, go ahead and print your message, otherwise skip it
 */ 
void LOGGER__log_repetitive(const char *fmt, ...) {
    va_list args;

	va_start(args, fmt);
    if (printk_ratelimit()) {
        printk(fmt, args);
    }
    va_end(args);
}

void LOGGER__log_device_numbers(dev_t identifier, int number_of_devices) {
    int i = 0, first_minor = MINOR(identifier);
    for (i = 0; i < number_of_devices; i++) {
        LOGGER__LOG_DEBUG(
            "Device %d, Major: %d, Minor: %d\n",
            i + 1,
            MAJOR(identifier),
            first_minor + i
        );
    }
}
