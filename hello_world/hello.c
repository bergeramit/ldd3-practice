#include <linux/init.h>
#include <linux/module.h>


MODULE_LICENSE("Dual BSD/GPL - can be anything");

static int hello_init(void) {
    /*
     * printk has the same format as printf ==> printk(char const *fmt, ...)
     * printk(KERN_ALERT "hello world")  is converted to 
     * printk("\000" "hello world") is converted to 
     * printk("\000hello world") -> the c knows to concatinate strings that do
     * not have a anything between them -> works with printf as well
     * */
    printk(KERN_ALERT "Hello!!\n");
    return 0;
}


static void hello_exit(void) {
    printk(KERN_ALERT "Bye!!\n");
}


module_init(hello_init);
module_exit(hello_exit);
