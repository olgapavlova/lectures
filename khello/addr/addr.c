#include <linux/module.h>

static int __init addr(void) {
  void show(void* p) {
    printk("%20p%20px%20pK%20lx\n", p, p, p, (uintptr_t)p);
  }

  printk("%20s%20s%20s%20s\n", "%p", "%px", "%pK", "int");

  show(_printk);
  show(sscanf);
  show((void*)current);
  show((void*)0xffff800000000000);

  return -2;
}

module_init(addr);

MODULE_LICENSE("GPL");
