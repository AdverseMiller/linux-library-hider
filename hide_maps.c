#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kprobes.h>
#include <linux/seq_file.h>
#include <linux/mm.h>

static char *target_lib;
module_param(target_lib, charp, 0444);
MODULE_PARM_DESC(target_lib, "Target shared library to hide (required)");

static struct kprobe kp = {
    .symbol_name  = "show_map_vma",
};

static int pre_show_map_vma(struct kprobe *p, struct pt_regs *regs)
{
    struct vm_area_struct *vma = (struct vm_area_struct *)regs->si;
	if (!vma || !vma->vm_file) return 0;

    if (strcmp(vma->vm_file->f_path.dentry->d_name.name, target_lib) == 0) {
		unsigned long ret = ((unsigned long*)regs->sp)[0];
        regs->ip = ret; //jump to function end
        return 1;  
    }

    return 0;  
}

static void post_show_map_vma(struct kprobe *p,
                              struct pt_regs *regs,
                              unsigned long flags)
{
	//do not optimize away our handler
}

static int __init hide_init(void)
{
    if (!target_lib) {
        pr_err("target_lib parameter is required\n");
        return -EINVAL;
    }

    kp.pre_handler = pre_show_map_vma;
    kp.post_handler = post_show_map_vma;
    return register_kprobe(&kp);
}

static void __exit hide_exit(void)
{
    unregister_kprobe(&kp);
}

module_init(hide_init);
module_exit(hide_exit);
MODULE_LICENSE("GPL");
