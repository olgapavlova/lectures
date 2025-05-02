#include <linux/init.h>
#include <linux/mm.h>
#include <linux/gfp.h>
#include <linux/module.h>
#include <linux/slab.h>

#define MODULE_NICK "slab"

#define UNIT_Q 39

struct unit {
  int n;
  char name[127];
};

static struct kmem_cache * ca;
static struct unit * un;

static int __init slab_init(void) {
  pr_info(MODULE_NICK ": trying to insert\n");

  ca = kmem_cache_create("my cache",
                          sizeof(struct unit),
                          0,
                          SLAB_HWCACHE_ALIGN,
                          NULL);

  if(!ca) {
    pr_err(MODULE_NICK ": kmem_cache_create() failed\n");
    return -ENOMEM;
  }

  kmem_cache_free(ca, un);
  kmem_cache_destroy(ca);
	return -2;
}

static void __exit slab_exit(void) {
  pr_info(MODULE_NICK ": module removed\n");
}

module_init(slab_init);
module_exit(slab_exit);

MODULE_LICENSE("GPL");
