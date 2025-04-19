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
#define DEVICE_NAME "servo90"

static struct file *uart_filp = NULL;

static ssize_t servo_write(struct file *file, const char __user *buf, size_t len, loff_t *ppos) {
    char kbuf;

    if (len < 1) return -EINVAL;
    if (copy_from_user(&kbuf, buf, 1)) return -EFAULT;

    if (kbuf == '1' && uart_filp) {
        char cmd[] = "ROTATE\n";
        kernel_write(uart_filp, cmd, strlen(cmd), &uart_filp->f_pos);
    }

    return 1;
}

static int servo_open(struct inode *inode, struct file *file) {
    uart_filp = filp_open(UART_PATH, O_WRONLY | O_NOCTTY | O_NONBLOCK, 0);
    if (IS_ERR(uart_filp)) {
        pr_err("servo90: can't open %s\n", UART_PATH);
        return PTR_ERR(uart_filp);
    }
    return 0;
}

static int servo_release(struct inode *inode, struct file *file) {
    if (uart_filp) filp_close(uart_filp, NULL);
    uart_filp = NULL;
    return 0;
}

static const struct file_operations servo_fops = {
    .owner = THIS_MODULE,
    .write = servo_write,
    .open  = servo_open,
    .release = servo_release,
};

static struct miscdevice servo_misc_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = DEVICE_NAME,
    .fops = &servo_fops,
    .mode = 0666,
};

static int __init servo_init(void) {
    int ret = misc_register(&servo_misc_device);
    pr_info("servo90: driver loaded\n");
    return ret;
}

static void __exit servo_exit(void) {
    misc_deregister(&servo_misc_device);
    pr_info("servo90: driver unloaded\n");
}

module_init(servo_init);
module_exit(servo_exit);

MODULE_LICENSE("GPL");
