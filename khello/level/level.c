#include <linux/init.h>
#include <linux/module.h>

static const char* lvl[] = {
  KERN_DEBUG, KERN_INFO, KERN_NOTICE, KERN_WARNING, KERN_ERR, KERN_CRIT, KERN_ALERT
};

static int __init hello_init(void) {
  
    char msg[72] = {'\0'};
    int i = 0;
    for(i = 0; i < sizeof(lvl)/sizeof(lvl[0]); i++) {
      sprintf(msg, "%s Level: %s\n", lvl[i], lvl[i]);
      printk(msg);
    }

    return -2;
}

static void __exit hello_exit(void) {
    printk(KERN_INFO "Goodbye, world!\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
