#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/wait.h>
#include <linux/jiffies.h>
#include <linux/timer.h>
#include <linux/interrupt.h>

#include "char_driver_fops.h"
#include "../logger/logger.h"
#include "../device_manager/device_manager.h"

DEFINE_TIMER(CHAR_DRIVER_OPS__sleep_timer_g, handle_timeout);
DECLARE_TASKLET(CHAR_DRIVER_OPS__tasklet_g, handle_tasklet, 0);
static DECLARE_WAIT_QUEUE_HEAD(sleep_q);
static DECLARE_WAIT_QUEUE_HEAD(timers_q);
static int rw_flag = 0;

static int init_repeat = 2;
static int init_flag = 0;

struct file_operations example_fops = {
    .owner = THIS_MODULE,
    .read = sleep_read,
    .write = sleep_write,
    .open = example_open,
    .release = example_release
};

int example_release(struct inode *inode, struct file *filp) {

    tasklet_kill(&CHAR_DRIVER_OPS__tasklet_g);
    
    LOGGER__LOG_DEBUG("release killed the tasklet\n");
    return 0;
}
/*
 * This code is ran inside a "soft interrupt" handler - must be atomic (semi-atomic). no sleeps bruh
 * Tasklet cannot be schedules at a specific time. usually we use Tasklet when we have to handle
 * hardware interrupts and we want to schedule to run after recieving any signal from the hardware
 */
void handle_tasklet(unsigned long int data) {
    printk(KERN_DEBUG "process %i (%s) Run inside the tasklet, in a kernel thread!\n", current->pid, current->comm);
}

/*
 * This code is ran inside a "soft interrupt" handler - must be atomic (semi-atomic). no sleeps bruh
 */
void handle_timeout(struct timer_list *my_timer) {
    unsigned long int timer_delay_in_seconds = CHAR_DRIVER_OPS__TIMER_DELAY / HZ;
    printk(KERN_DEBUG "process %i (%s) finished timer and will signal to finish init!\n", current->pid, current->comm);

    if (init_repeat > 0) {
        init_repeat--;
        CHAR_DRIVER_OPS__sleep_timer_g.function = handle_timeout;
        CHAR_DRIVER_OPS__sleep_timer_g.expires = jiffies + CHAR_DRIVER_OPS__TIMER_DELAY;
        printk(KERN_DEBUG "repeat - process %i (%s) is setting a timer to finish init in about: %ld\n",
            current->pid,
            current->comm,
            timer_delay_in_seconds);
        add_timer(&CHAR_DRIVER_OPS__sleep_timer_g);
    } else {
        init_flag = 1;
        wake_up_interruptible(&timers_q);
    }
}

ssize_t sleep_write(
    struct file *filp,
    const char __user *usr_buf,
    size_t count,
    loff_t *pos
){
    printk(KERN_DEBUG "process %i (%s) wake everyone up...\n", current->pid, current->comm);
    rw_flag = 1;

    /*
     * You must call wake up after the predicate was satisfied
     */
    wake_up_interruptible(&sleep_q);
    return count; /* success to avoid retry */
}

ssize_t sleep_read(
    struct file *filp,
    char __user *usr_buf,
    size_t count,
    loff_t *pos
){
     printk(KERN_DEBUG "process %i (%s) going to sleep\n", current->pid, current->comm);
     wait_event_interruptible(sleep_q, rw_flag != 0);

     /*
      * Assuming the rw_flag was set (that is why we are not sleeping anymore)
      */
     rw_flag = 0;
     printk(KERN_DEBUG "awoken %i (%s)\n", current->pid, current->comm);
     return 0;
}

/*
 * inode - the way the kernel represent files in the system,
 * if the fie is a special device file than this struct will
 * have the cdev struct (inode->i_cdev)
 * 
 * filp - file pointer this struct represent open files in the system
 * 
 */
int example_open(struct inode *inode, struct file *filp) {
    unsigned long int timer_delay_in_seconds = CHAR_DRIVER_OPS__TIMER_DELAY / HZ;
    LOGGER__LOG_DEBUG("open syscall called\n");

    /*
     * This technique uses the minor number to detect which cdev struct is responsible
     * for that inode
     */
    if (iminor(inode) == MINOR(DEVICE_MANAGER__cdev_g.cdev.dev)) {
        LOGGER__LOG_DEBUG("Init Successful (found cdev for the special file)\n");

        /* 
        * We will save it for easy access
        */ 
        filp->private_data = &DEVICE_MANAGER__cdev_g;

        /*
         * Register the timer
         * 
         */
        CHAR_DRIVER_OPS__sleep_timer_g.function = handle_timeout;
        CHAR_DRIVER_OPS__sleep_timer_g.expires = jiffies + CHAR_DRIVER_OPS__TIMER_DELAY;
        printk(KERN_DEBUG "process %i (%s) is setting a timer to finish init in about: %ld\n",
            current->pid,
            current->comm,
            timer_delay_in_seconds);

        add_timer(&CHAR_DRIVER_OPS__sleep_timer_g);

        /* wait for the buffer to fill */
        wait_event_interruptible(timers_q, init_flag != 0);

        printk(KERN_DEBUG "process %i (%s) is done waiting for timer - finished init, begin tasklet initializtion ", current->pid, current->comm);

        tasklet_schedule(&CHAR_DRIVER_OPS__tasklet_g);

    }
    return 0;
}
