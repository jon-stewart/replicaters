#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>

#define LEN (1024 * 1024)
#define FD  ("/germfd")

int main(void)
{
    int fd = shm_open(FD, (O_RDWR), S_IRWXU);
    if (fd == -1) {
        printf("[!] shm_open failure\n");
    }

    void *addr = mmap(0, LEN, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) {
        printf("[!] mmap failure\n");
    }

    /* Break point here and 'x/100qx addr', you are looking at the germ code :D*/
    printf(": %s\n", addr);

    munmap(addr, LEN);

    shm_unlink(FD);

    return (0);
}
