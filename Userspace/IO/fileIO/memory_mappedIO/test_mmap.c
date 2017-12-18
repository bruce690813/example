#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
//#include <sys/user.h>

int main(int argc, char *argv[])
{
    struct stat sb;
    off_t len;
    char *p;
    int fd;

    if (argc < 2) {
        fprintf(stderr, "usage: %s <file>\n", argv[0]);
        return 1;
    }

    fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    /* stat, fstat, lstat - get file status */
    if (fstat(fd, &sb) == -1) {
        perror("fstat");
        return 1;
    }

    if (!S_ISREG(sb.st_mode)) {
        fprintf(stderr, "%s is not a file\n", argv[1]);
        return 1;
    }

    /* mmap, munmap - map or unmap files or devices into memory */
    p = mmap(0, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if (p == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    if (close(fd) == -1) {
        perror("clsoe");
        return 1;
    }

    for (len = 0; len < sb.st_size; len++)
        /* fputc, fputs, putc, putchar, puts - output of characters and strings */
        putchar(p[len]);

    if (munmap(p, sb.st_size) == -1) {
        perror("munmap");
        return 1;
    }

    /* sysconf - get configuration information at run time
       PAGESIZE - _SC_PAGESIZE
         Size of a page in bytes.  Must not be less than 1.  (Some systems use PAGE_SIZE instead.) */
    long page_size  = sysconf(_SC_PAGESIZE);
    //int  page_size2 = getpagesize();
    //int  page_size3 = PAGE_SIZE;

    printf("page_size = %lu\n", page_size);

    return 0;
}
