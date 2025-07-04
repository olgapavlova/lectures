#include <linux/init.h>
#include <linux/module.h>
#include <linux/mm.h>
#include <asm/io.h>

static  int stack_data = 17;
static  int * my_data = &stack_data;

static  int __init vp_init(void) {
  //int * my_data = kmalloc(1, GFP_KERNEL);
  //if(!my_data) { return -ENOMEM; }


  pr_info("First address:\t\t%p\n", my_data);

  phys_addr_t phys_addr = virt_to_phys(my_data);
  pr_info("Physical address:\t%pa\n", &phys_addr);

  void * virt_addr = phys_to_virt(phys_addr);
  pr_info("Virtual again:\t\t%p\n", virt_addr);

  // kfree(my_data);
  return -2;
}

static void __exit vp_exit(void) { }

module_init(vp_init);
module_exit(vp_exit);

MODULE_LICENSE("GPL");
