#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>


MODULE_LICENSE("Dual BSD/GPL - can be anything");

static int hello_init(void) {
    printk(
	KERN_ALERT "Current Process (inited me) is \"%s\" (pid: %i)\n",
       	current->comm,
	current->pid
	);

    return 0;
}


static void hello_exit(void) {
    printk(
	KERN_ALERT "Current Process (terminates me) is \"%s\" (pid: %i)\n",
       	current->comm,
	current->pid
	);
}


module_init(hello_init);
module_exit(hello_exit);
