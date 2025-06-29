/*
  gcc -Wall -D_FILE_OFFSET_BITS=64 `pkg-config fuse --cflags --libs` birds_macfuse.c -o birds
*/

#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "birds.h"
#include <stdbool.h>

static const char *species_str = "This is a species!\n";

int order_index(const char *target) {
    for (int i = 0; i < NUM_ORDERS; i++) {
        if (strcmp(Orders[i], target) == 0) {
            return i;
        }
    }
    return -1;
}

int family_index(const char *target) {
    for (int i = 0; i < NUM_FAMILIES; i++) {
        if (strcmp(Families[i].name, target) == 0) {
            return i;
        }
    }
    return -1;
}

int species_index(const char *target) {
    for (int i = 0; i < NUM_SPECIES; i++) {
        if (strcmp(Species[i].name, target) == 0) {
            return i;
        }
    }
    return -1;
}

/* 
    In FUSE, the passed `path` param is a fully qualified
    path relative to the root of the mounted filesystem
 */
typedef struct {
    char order[MAX_ORDER_NAME_LEN];
    char family[MAX_FAMILY_NAME_LEN];
    char species[MAX_SPECIES_NAME_LEN];
} BirdPath;

void parse_path(const char *path, BirdPath *bp) {
    char tmp[256];
    strncpy(tmp, path + 1, sizeof(tmp));
    tmp[sizeof(tmp) - 1] = '\0';

    char *first = strtok(tmp, "/");
    char *second = strtok(NULL, "/");
    char *third = strtok(NULL, "/");

    if (first && order_index(first) > -1) strncpy(bp->order, first, MAX_ORDER_NAME_LEN);
    if (second && family_index(second) > -1) strncpy(bp->family, second, MAX_FAMILY_NAME_LEN);
    if (third && species_index(third) > -1) strncpy(bp->species, third, MAX_SPECIES_NAME_LEN);
}

static int birds_getattr(const char *path, struct stat *stbuf) {
    BirdPath bp = {0};
    parse_path(path, &bp);

    memset(stbuf, 0, sizeof(struct stat));

    if (strlen(bp.species) > 0) {
        stbuf->st_mode = S_IFREG | 0444;
        stbuf->st_nlink = 1;
        stbuf->st_size = strlen(species_str);
        return 0;
    }
    
    stbuf->st_mode = S_IFDIR | 0755;
    stbuf->st_nlink = 2;
    return 0;
}

static int birds_readdir(
    const char *path,
    void *buf,
    fuse_fill_dir_t filler,
    off_t offset,
    struct fuse_file_info *fi
) {
    (void) offset;
    (void) fi;

    BirdPath bp = {0};
    parse_path(path, &bp);

    if (strlen(bp.family) > 0) {
        filler(buf, ".", NULL, 0);
        filler(buf, "..", NULL, 0);

        for (int i = 0; i < NUM_SPECIES; i++) {
            if (Species[i].family == family_index(bp.family)) {
                filler(buf, Species[i].name, NULL, 0);
            }
        }
        return 0;
    }

    if (strlen(bp.order) > 0) {
        filler(buf, ".", NULL, 0);
        filler(buf, "..", NULL, 0);

        for (int i = 0; i < NUM_FAMILIES; i++) {
            if (Families[i].order == order_index(bp.order)) {
                filler(buf, Families[i].name, NULL, 0);
            }
        }
        return 0;
    }
    
    if (strcmp(path, "/") == 0) {
        filler(buf, ".", NULL, 0);
        filler(buf, "..", NULL, 0);

        for (int i = 0; i < NUM_ORDERS; i++) {
            filler(buf, Orders[i], NULL, 0);
        }
        return 0;
    }
    
    return -ENOENT;
}

static int birds_open(const char *path, struct fuse_file_info *fi) {
    BirdPath bp = {0};
    parse_path(path, &bp);

    if (strlen(bp.species) > 0 && (fi->flags & 3) == O_RDONLY) {
        return 0;
    }

    return -ENOENT;
}

static int birds_read(
    const char *path,
    char *buf,
    size_t size,
    off_t offset,
    struct fuse_file_info *fi
) {
    BirdPath bp = {0};
    parse_path(path, &bp);
    
    size_t len = strlen(species_str);
    if (offset < len) {
        if (offset + size > len) {
            size = len - offset;
        }
        memcpy(buf, species_str + offset, size);
    } else {
        size = 0;
    }

    return size;
}

static struct fuse_operations birds_oper = {
    .getattr = birds_getattr,
    .readdir = birds_readdir,
    .open = birds_open,
    .read = birds_read,
};

int main(int argc, char *argv[]) {
    return fuse_main(argc, argv, &birds_oper, NULL);
}
