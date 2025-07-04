#include <linux/init.h>
#include <linux/module.h>

static  int __init init(void) { return -2; }
static void __exit exit(void) { }

module_init(init);
module_exit(exit);

MODULE_LICENSE("GPL");
