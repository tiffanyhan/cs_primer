#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>

int main() {
    const char *fname = ".";
    DIR* dirptr = opendir(fname);

    struct dirent *entry;
    struct stat dets;

    while ((entry = readdir(dirptr)) != NULL) {
        const char *name = entry->d_name;

        stat(name, &dets);

        printf("%llu %d %2d %6llu %3lld %d %s\n", dets.st_ino, dets.st_nlink, entry->d_type, dets.st_size, dets.st_blocks, dets.st_blksize, name);
    }
}
