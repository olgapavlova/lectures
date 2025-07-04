#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/uaccess.h>

static char* hi = "Hello, world!\n";

static ssize_t dev_read(struct file * f, char __user * buffer, size_t counter, loff_t * ppos) {
	int len = strlen(hi);
	printk(KERN_INFO "=== read : %ld\n", (long)counter);
	if (counter < len) return -EINVAL;

	if (*ppos != 0) {
		printk(KERN_INFO "=== read return : 0\n");
		return 0;
	}

	if (copy_to_user(buffer, hi, len)) return -EINVAL;

	*ppos = len;
	printk(KERN_INFO "=== read return : %d\n", len);
	return len;
}

static int __init dev_init(void);
module_init(dev_init);

static void __exit dev_exit(void);
module_exit(dev_exit);

MODULE_LICENSE("GPL");
