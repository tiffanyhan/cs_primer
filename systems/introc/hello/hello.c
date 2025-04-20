#include <stdio.h>

int main(int argc, char **argv) {
    printf("Hello, World!\n");
    int count = (int) sizeof(argv[0]);
    printf("size of argv is: %d\n", count);
    printf("first arg is: %s\n", argv[0]);

    int size = sizeof(argv) / sizeof(argv[0]);
    for (int i = 0; i < size; i++) {
        printf("%s\n", argv[i]);
    }

    return 0;
}
