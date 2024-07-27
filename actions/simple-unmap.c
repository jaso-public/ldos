#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <errno.h>
#include <sys/mman.h>

// Define the system call number for my_mmap
#define SYS_my_mmap 548
#define SYS_my_munmap 549

int main() {
    unsigned long addr = 0;  // Desired address or 0 for the kernel to choose
    unsigned long len = 4096 * 1024;  // Length of the mapping
    unsigned long prot = PROT_READ | PROT_WRITE;  // Protection flags
    unsigned long flags = MAP_PRIVATE | MAP_ANONYMOUS;  // Mapping flags
    unsigned long fd = -1;  // File descriptor, -1 for MAP_ANONYMOUS
    unsigned long pgoff = 0;  // Offset in the file or 0 for anonymous

    // Call the custom system call
    unsigned long result = syscall(SYS_my_mmap, addr, len, prot, flags, fd, pgoff);
    printf("my_mmap returned: %lu\n", result);

    // Check the result
    if (result == (unsigned long)-1) {
        // Print the error if the system call failed
        perror("my_mmap syscall failed");
        return 1;
    }

    // make a hole in the allocated memory
    addr = result + 8192;
    
    // Call the custom system call
    result = syscall(SYS_my_munmap, addr, 8192);
    printf("my_munmap returned: %lu\n", result);

    // Check the result
    if (result) {
        // Print the error if the system call failed
        perror("my_munmap syscall failed");
        return 1;
    }

    // Print the address of the mapped memory
    printf("my_mmap syscall succeeded, mapped address: %lx\n", result);

    return 0;
}

