#include <linux/init.h>
#include <linux/module.h>

static int __init silent_init(void) { return 0; }
static void __exit silent_exit(void) { }

module_init(silent_init);
module_exit(silent_exit);

MODULE_LICENSE("GPL");
