#include <linux/init.h>
#include <linux/module.h>
#include <linux/vmalloc.h>

#define MODULE_NICK "vm"

#define VMAL_SIZE 1

static int * reserve = NULL;

static int __init vm_init(void) {
  pr_info(MODULE_NICK ": trying to insert\n");
  reserve = vmalloc(VMAL_SIZE * PAGE_SIZE);
  if(!*reserve) {
    pr_err(MODULE_NICK ": vmalloc(%ld) failed\n", VMAL_SIZE * PAGE_SIZE);
    return -ENOMEM;
  } else {
    pr_info(MODULE_NICK ": reserved address: %pa\n", reserve);
  }

  vfree(reserve);
	return 0;
}

static void __exit vm_exit(void) {
  pr_info(MODULE_NICK ": module removed\n");
}

module_init(vm_init);
module_exit(vm_exit);

MODULE_LICENSE("GPL");
