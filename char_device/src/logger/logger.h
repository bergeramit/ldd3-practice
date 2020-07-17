#ifndef LOGGER
#define LOGGER

#ifdef DEBUG

#   ifdef __KERNEL__
#       include <linux/module.h>
#       define LOGGER__LOG_DEBUG(fmt, args...) printk(KERN_WARNING "DEBUG: " fmt, ##args)
#       define LOGGER__LOG_ERROR(rc, fmt, args...)                     \
            do {                                                       \
                pr_err("Error: in %s:%d:\n", __FILE__, __LINE__);      \
                pr_err("\t" fmt, ##args);                              \
                pr_err("\tError code: %d\n", rc);                      \
            } while(0)

#   else
#       include <stdio.h>
#       define LOGGER__LOG_DEBUG(fmt, args...) fprintf(stdout, fmt, ##args)
#       define LOGGER__LOG_ERROR(fmt, args...) fprintf(stderr, fmt, ##args)  
#   endif

#else
#   define LOGGER__LOG_DEBUG(fmt, args...)
#endif

void LOGGER__log_device_numbers(dev_t identifier, int number_of_devices);

void LOGGER__log_repetitive(const char *fmt, ...);

#endif /* LOGGER */