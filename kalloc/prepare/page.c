#include <linux/init.h>
#include <linux/mm.h>
#include <linux/gfp.h>
#include <linux/module.h>

#define MODULE_NICK "page"

#define PAGE_ORDER 10

static int __init page_init(void) {
  pr_info(MODULE_NICK ": trying to insert\n");
  struct page * reserve = alloc_pages(GFP_KERNEL, PAGE_ORDER);
  if(!reserve) {
    pr_err(MODULE_NICK ": alloc_pages(..., %d) failed\n", PAGE_ORDER);
    return -ENOMEM;
  }

  void * ptr = page_address(reserve);
  if(!ptr) {
    pr_err(MODULE_NICK ": page_address() failed\n");
    return -ENOMEM;
  } else {
    pr_info(MODULE_NICK ": reserved address: %pa\n", ptr);
  }

  strcpy(ptr, "* * * Text from allocated memory");
  pr_info(MODULE_NICK ": %s\n", (char*)ptr);

  __free_pages(reserve, PAGE_ORDER);
	return 0;
}

static void __exit page_exit(void) {
  pr_info(MODULE_NICK ": module removed\n");
}

module_init(page_init);
module_exit(page_exit);

MODULE_LICENSE("GPL");
