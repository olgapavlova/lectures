#include <linux/init.h>
#include <linux/module.h>

#define MODULE_NICK "op"

static int __init t_init(void) {
  pr_info(MODULE_NICK ": module inserted\n");
	return 0;
}
static void __exit t_exit(void) {
  pr_info(MODULE_NICK ": module removed\n");
}

module_init(t_init);
module_exit(t_exit);

MODULE_LICENSE("GPL");
