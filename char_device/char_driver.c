#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>

static char *magic_string = "default magic";
static int input_array_size = 10;
static int numbers[10] = {0};

MODULE_LICENSE("Dual BSD/GPL - can be anything");

/*
 * S_IRUGO - read by all and not changeable
 * use `modinfo module.ko` to get the information about the module
 *
 * input_array_size - will change if the number or arguments in the cmd line is shorted
 */
module_param(magic_string, charp, S_IRUGO);
module_param_array(numbers, int, &input_array_size, S_IRUGO);


static int param_init(void) {
    int i = 0;
    printk(KERN_ALERT "The magic string: %s\n", magic_string);
    printk(KERN_ALERT "The numbers you entered:\n");
    for (i = 0; i < input_array_size; i++) {
        printk(KERN_ALERT "%d", numbers[i]);
    }
    printk(KERN_ALERT "\n");
    return 0;
}

static void hello_exit(void) {
    printk(KERN_ALERT "Bye!!\n");
}


module_init(param_init);
module_exit(hello_exit);
