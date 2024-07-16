#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/syscalls.h>

SYSCALL_DEFINE6(my_mmap, unsigned long, addr, unsigned long, len,
                unsigned long, prot, unsigned long, flags,
                unsigned long, fd, unsigned long, off)
{
        if (off & ~PAGE_MASK)
                return -EINVAL;

        return ksys_mmap_pgoff(addr, len, prot, flags, fd, off >> PAGE_SHIFT);
}


SYSCALL_DEFINE2(my_munmap, unsigned long, addr, size_t, len)
{
        addr = untagged_addr(addr);
//        profile_munmap(addr);
        return vm_munmap(addr, len);
}


