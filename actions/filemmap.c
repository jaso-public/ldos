#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>


// Define the system call number for my_mmap
#define SYS_my_mmap 548

int main() {
    const char *filepath = "example.txt";
    int fd;
    struct stat sb;
    char *mapped;

    // Open the file
    fd = open(filepath, O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Get the size of the file
    if (fstat(fd, &sb) == -1) {
        perror("Error getting the file size");
        close(fd);
        exit(EXIT_FAILURE);
    }

    unsigned long addr = 0;  // Desired address or 0 for the kernel to choose
    unsigned long len = sb.st_size;  // Length of the mapping
    unsigned long prot = PROT_READ;  // Protection flags
    unsigned long flags = MAP_PRIVATE;  // Mapping flags
    unsigned long pgoff = 0;  // Offset in the file or 0 for anonymous

    // Call the custom system call
     mapped = (char*) syscall(SYS_my_mmap, addr, len, prot, flags, fd, pgoff);

    // mapped = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (mapped == MAP_FAILED) {
        perror("Error mmapping the file");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Print the file contents
    printf("File contents:\n%.*s\n", (int)sb.st_size, mapped);

    // Clean up
    if (munmap(mapped, sb.st_size) == -1) {
        perror("Error unmapping the file");
    }
    close(fd);

    return 0;
}

