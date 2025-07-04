#include <linux/init.h>
#include <linux/module.h>

#define MODULE_NICK "km"

#define KMAL_SIZE 1024 * 1024

static int __init km_init(void) {
	pr_info(MODULE_NICK ": module inserted\n");

  static int * reserve = NULL;
  reserve = kmalloc(1024 * 1024, GFP_KERNEL);

  if(!*reserve) {
    pr_err(MODULE_NICK ": kmalloc(%d) failed\n", KMAL_SIZE);
    return -ENOMEM;
  } else {
    pr_info(MODULE_NICK ": reserved address: %pa\n", reserve);
  }

  kfree(reserve);
	return 0;
}

static void __exit km_exit(void) {
	pr_info(MODULE_NICK ": module removed\n");
}

module_init(km_init);
module_exit(km_exit);

MODULE_LICENSE("GPL");
