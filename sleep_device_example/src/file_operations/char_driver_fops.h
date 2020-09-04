#ifndef CHAR_FOPS
#define CHAR_FOPS

#include <linux/version.h>
#include <linux/jiffies.h>

#include "../module_handlers/module_handler.h"
#include "../device_manager/device_manager.h"

/*
 * HZ - the number of times the timer interrupts occur in a second
 *      this is not the CPU speed as this is only for timers.
 *      usually HZ is between 100 and 1000 meaning - interrupts times per second
 * the jiffies variable counts the number of times the timer interrupts occurs
 * 
 * so delay of 15 * HZ is a delay for 15 seconds
 */
#define CHAR_DRIVER_OPS__TIMER_DELAY (15 * HZ)

extern struct DEVICE_MANAGER__example_cdev DEVICE_MANAGER__cdev_g;

ssize_t sleep_write(struct file *filp, const char __user *b, size_t c, loff_t *d);

ssize_t sleep_read(struct file *filp, char __user *usr_buf, size_t count, loff_t *pos);

int example_open(struct inode *inode, struct file *filp);

void handle_timeout(struct timer_list *my_timer);

#endif /* CHAR_FOPS */