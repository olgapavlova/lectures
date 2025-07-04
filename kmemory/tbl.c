#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <asm/pgtable.h>  // Для pgd_offset() и pgd_val()

static int __init show_pgd_init(void)
{
    unsigned long va = (unsigned long)&show_pgd_init;  // Просто взять свой собственный адрес
    pgd_t *pgd;

    pr_info("Virtual address: %lx\n", va);

    pgd = pgd_offset(current->mm, va);
    if (pgd_none(*pgd) || pgd_bad(*pgd)) {
        pr_err("Invalid or missing PGD entry!\n");
        return -1;
    }

    pr_info("PGD entry: %p -> 0x%016llx\n", pgd, (unsigned long long)pgd_val(*pgd));

    return -2;
}

static void __exit show_pgd_exit(void)
{
    pr_info("Show PGD module exit\n");
}

module_init(show_pgd_init);
module_exit(show_pgd_exit);

MODULE_LICENSE("GPL");
