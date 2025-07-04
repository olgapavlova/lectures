#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <asm/io.h>

static int __init v2p_init(void) {
    int * my_data = kmalloc(1, GFP_KERNEL);
    pr_info("Virtual address of my_data:\t%p\n", my_data);

    phys_addr_t phys_addr = virt_to_phys(my_data);
    pr_info("Physical address of my_data:\t%pa\n", &phys_addr);

    void * virt_addr = phys_to_virt(phys_addr);
    pr_info("Back to virtual address:\t\t%p\n", virt_addr);

    if (virt_addr == my_data) {
        pr_info("SUCCESS: Addresses match!\n");
    } else {
        pr_info("FAIL: Addresses DO NOT match!\n");
    }

    kfree(my_data);
    return -2;
}

static void __exit v2p_exit(void) {
    pr_info("V2P exit\n");
}

module_init(v2p_init);
module_exit(v2p_exit);

MODULE_LICENSE("GPL");
