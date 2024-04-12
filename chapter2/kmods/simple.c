#include <linux/hash.h>
#include <linux/gcd.h>
#include <linux/init.h>
#include <linux/jiffies.h>
#include <linux/kernel.h>
#include <linux/module.h>

// called when the module is loaded
int simple_init(void) {
    printk(KERN_INFO "Gonz: Loading Kernel Module\n");
    printk(KERN_INFO "Gonz: %lu\n", GOLDEN_RATIO_PRIME);
    printk(KERN_INFO "Gonz: %lu\n", gcd(3300, 24));
    printk(KERN_INFO "Gonz: jiffies = %lu\n", jiffies);
    return 0;
}

// called when the module is removed
void simple_exit(void) {
    printk(KERN_INFO "Gonz: jiffies = %lu\n", jiffies);
    printk(KERN_INFO "Gonz: Removing Kernel Module\n");
}

// macros for registering module entry and exit points
module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");

