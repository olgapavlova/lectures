#include "dev.h"
#include <linux/cdev.h>
#include <linux/init.h>

static int major = 0;
module_param(major, int, S_IRUGO);

#define EOK 0
static int device_open = 0;

static int dev_open(struct inode *n, struct file *f) {
  if (device_open) return -EBUSY;
  device_open++;
  return EOK;
}

static int dev_release(struct inode *n, struct file *f) {
  device_open--;
  return EOK;
}

static const struct file_operations dev_fops = {
  .owner   = THIS_MODULE,
  .open    = dev_open,
  .release = dev_release,
  .read    = dev_read,
};

#define DEVICE_FIRST 0
#define DEVICE_COUNT 3
#define MODNAME "my_cdev_dev"

static struct cdev hcdev;

static int __init dev_init(void) {
  int ret = 0;
  dev_t dev = 0;

  if (major != 0) {
    dev = MKDEV(major, DEVICE_FIRST);
    ret = register_chrdev_region(dev, DEVICE_COUNT, MODNAME);
  } else {
    ret = alloc_chrdev_region(&dev, DEVICE_FIRST, DEVICE_COUNT, MODNAME);
    major = MAJOR(dev);
  }

  if (ret < 0) {
    unregister_chrdev_region(MKDEV(major, DEVICE_FIRST), DEVICE_COUNT);
    printk(KERN_INFO "=== can not register char device region\n");
    goto err;
  }

  cdev_init(&hcdev, &dev_fops);
  hcdev.owner = THIS_MODULE;
  ret = cdev_add(&hcdev, dev, DEVICE_COUNT);

  if (ret < 0) {
    unregister_chrdev_region(MKDEV(major, DEVICE_FIRST), DEVICE_COUNT);
    printk(KERN_INFO "=== cannot add char device\n");
    goto err;
  }

  printk(KERN_INFO "===== module installed : %d:%d =====\n", MAJOR(dev), MINOR(dev));

err:
  return ret;
}

static void __exit dev_exit(void) {
  cdev_del(&hcdev);
  unregister_chrdev_region(MKDEV(major, DEVICE_FIRST), DEVICE_COUNT);
  printk(KERN_INFO "===== module removed =====\n");
}
