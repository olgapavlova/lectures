#include <linux/module.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/file.h>
#include <linux/fcntl.h>

#define UART_PATH "/dev/ttyAMA0"
#define DEVICE_NAME "led_uart"

static struct file *uart_filp = NULL;

static ssize_t led_write(struct file *file, const char __user *buf, size_t len, loff_t *ppos) {
    char kbuf;

    if (len < 1) return -EINVAL;

    if (copy_from_user(&kbuf, buf, 1)) return -EFAULT;

    if (uart_filp) {
        kernel_write(uart_filp, &kbuf, 1, &uart_filp->f_pos);
    }

    return 1;
}

static int led_open(struct inode *inode, struct file *file) {
    uart_filp = filp_open(UART_PATH, O_WRONLY | O_NOCTTY | O_NONBLOCK, 0);
    if (IS_ERR(uart_filp)) {
        pr_err("led_uart: can't open %s\n", UART_PATH);
        return PTR_ERR(uart_filp);
    }
    return 0;
}

static int led_release(struct inode *inode, struct file *file) {
    if (uart_filp) filp_close(uart_filp, NULL);
    uart_filp = NULL;
    return 0;
}

static const struct file_operations led_fops = {
    .owner = THIS_MODULE,
    .write = led_write,
    .open  = led_open,
    .release = led_release,
};

static struct miscdevice led_misc_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = DEVICE_NAME,
    .fops = &led_fops,
    .mode = 0666,
};

static int __init led_init(void) {
    int ret = misc_register(&led_misc_device);
    pr_info("led_uart: loaded\n");
    return ret;
}

static void __exit led_exit(void) {
    misc_deregister(&led_misc_device);
    pr_info("led_uart: unloaded\n");
}

module_init(led_init);
module_exit(led_exit);

MODULE_LICENSE("GPL");
