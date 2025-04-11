#include <linux/string.h>
#include <linux/uaccess.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/fs.h>

#define DEVICE_FIRST 0
#define DEVICE_COUNT 3
#define DGROUP_NAME "my_driver_group_cdev"

static int major = 0;

static int cdrv_open(struct inode *n, struct file *f);
static int cdrv_release(struct inode *n, struct file *f);
static ssize_t cdrv_read(struct file * f, char __user * buffer, size_t counter, loff_t * ppos);

static struct cdev my_dev;

static const struct file_operations cdrv_fops = {
  .owner   = THIS_MODULE,
  .open    = cdrv_open,
  .release = cdrv_release,
  .read    = cdrv_read,
};

#define EOK 0
static int device_open = 0;

static  int __init cdrv_init(void);
static void __exit cdrv_exit(void);

module_init(cdrv_init);
module_exit(cdrv_exit);

static  int __init cdrv_init(void) {
  int result = 0;

  printk(KERN_INFO "Trying to register char device region\n");
	dev_t dev = 0;
	result = alloc_chrdev_region(&dev, DEVICE_FIRST, DEVICE_COUNT, DGROUP_NAME);
	major = MAJOR(dev);

	if (result < 0) {
    unregister_chrdev_region(MKDEV(major, DEVICE_FIRST), DEVICE_COUNT);
    printk(KERN_INFO "Can not register char device region\n");
    goto err;
  }

  printk(KERN_INFO "Char device region created: %d:%d...%d\n", major, DEVICE_FIRST, DEVICE_COUNT);

  cdev_init(&my_dev, &cdrv_fops);
  my_dev.owner = THIS_MODULE;

  result = cdev_add(&my_dev, dev, DEVICE_COUNT);
  if (result < 0) {
    unregister_chrdev_region(MKDEV(major, DEVICE_FIRST), DEVICE_COUNT);
    printk(KERN_INFO "Cannot add char device\n");
    goto err;
  }
  printk(KERN_INFO "Char device added\n");

err:
  return result;
}

static void __exit cdrv_exit(void) {
  cdev_del(&my_dev);
  unregister_chrdev_region(MKDEV(major, DEVICE_FIRST), DEVICE_COUNT);
  printk(KERN_INFO "Char device region %d:%d...%d destroyed\n", major, DEVICE_FIRST, DEVICE_COUNT);
}

static int cdrv_open(struct inode *n, struct file *f) {
  if (device_open) return -EBUSY;
  device_open++;
  return EOK;
}

static int cdrv_release(struct inode *n, struct file *f) {
  device_open--;
  return EOK;
}

static ssize_t cdrv_read(struct file * fp, 
                        char __user * buf,
                             size_t count,
                            loff_t * ppos) {
	const char * reply = "The same answer again and again.\n";
	size_t len = strlen(reply);

	if (*ppos >= len) return 0;  // EOF	
	if (count > len - *ppos) count = len - *ppos;

	if (copy_to_user(buf, reply, len)) return -EINVAL;

	*ppos += count;
	return count;
}

MODULE_LICENSE("GPL");
