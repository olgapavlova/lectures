#include <linux/init.h>
#include <linux/module.h>
#include <linux/vmalloc.h>

#define MODULE_NICK "vm"

#define KMAL_SIZE 1024 * 1024

static int __init vm_init(void) {
	pr_info(MODULE_NICK ": module inserted\n");

  static void * reserve = NULL;
  reserve = vmalloc(1024 * 1024);

  if(!reserve) {
    pr_err(MODULE_NICK ": vmalloc() failed\n");
    return -ENOMEM;
  } else {
    pr_info(MODULE_NICK ": reserved address: %pa\n", reserve);
  }

  kfree(reserve);
	return 0;
}

static void __exit vm_exit(void) {
	pr_info(MODULE_NICK ": module removed\n");
}

module_init(vm_init);
module_exit(vm_exit);

MODULE_LICENSE("GPL");
