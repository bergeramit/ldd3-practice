#ifndef DEVICE_PRINT
#define DEVICE_PRINT

#ifdef DEBUG

#   ifdef __KERNEL__
#       include <linux/module.h>
#       define PRINT_DEBUG(fmt, args...) printk(KERN_ALERT "DEBUG: " fmt, ##args)
#   else
#       include <stdio.h>
#       define PRINT_DEBUG(fmt, args...) fprintf(stderr, fmt, ##args)
#   endif

#else
#   define PRINT_DEBUG(fmt, args...)
#endif

#endif