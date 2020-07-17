#ifndef DEVICE_PRINT
#define DEVICE_PRINT

/*
 * printk_ratelimit - This function should be called
 * before you consider printing a message that could be repeated often.
 * If the function returns a nonzero value, go ahead and print your message, otherwise skip it
 */ 

#ifdef DEBUG

#   ifdef __KERNEL__
#       include <linux/module.h>
#       define PRINT_DEBUG(fmt, args...) printk(KERN_ALERT "DEBUG: " fmt, ##args)
#       define REPETITIVE_PRINT_DEBUG(fmt, args...)             \
            do {                                                \
                if (printk_ratelimit()) {                       \
                    printk(KERN_ALERT "DEBUG: " fmt, ##args)    \
                }                                               \
            } while(0);
#   else
#       include <stdio.h>
#       define PRINT_DEBUG(fmt, args...) fprintf(stderr, fmt, ##args)
#   endif

#else
#   define PRINT_DEBUG(fmt, args...)
#endif

#endif