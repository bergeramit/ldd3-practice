#ifndef logger
#define logger

#ifdef DEBUG

#   ifdef __KERNEL__
#       include <linux/module.h>
#       define LOGGER__LOG_DEBUG(fmt, args...) printk(KERN_ALERT "DEBUG: " fmt, ##args)
#   else
#       include <stdio.h>
#       define LOGGER__LOG_DEBUG(fmt, args...) fprintf(stderr, fmt, ##args)
#   endif

#else
#   define LOGGER__LOG_DEBUG(fmt, args...)
#endif

void LOGGER__log_device_numbers(dev_t identifier, int number_of_devices);

void LOGGER__log_repetitive(const char *fmt, ...);

void LOGGER__log_error(int rc, const char *fmt, ...);

#endif