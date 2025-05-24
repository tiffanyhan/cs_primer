#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

int main() {
    char fname[] = "/Users/tiffanyhan/resources/cs_primer/os/intro/bytewrite/XXXXXX";
    int fd;

    fd = mkstemp(fname);
    if (fd == -1) {
        return 1;
    }
    unlink(fname);

    printf("Temporary file name: %s\n", fname);

    FILE *fp;
    fp = fdopen(fd, "w");
    if (fp == NULL) {
        close(fd);
        return 1;
    }

    struct stat filestats;
    blkcnt_t prevblocks = 0;

    for (int i = 0; i < 1000000; i++) {
        fprintf(fp, "a");
        fflush(fp); // default behavior is flush for every 4096 bytes

        if (fstat(fd, &filestats) == -1) {
            close(fd);
            return 1;
        }

        off_t fsize = filestats.st_size;
        blkcnt_t blocks = filestats.st_blocks;

        // printf("[Fsize: %lld] Disk space: %lld\n", fsize, blocks * 512);

        if (prevblocks != blocks) {
            prevblocks = blocks;
            printf("[Fsize: %lld] Disk space: %lld\n", fsize, blocks * 512);
        }
    }

    fclose(fp);
    return 0;
}
