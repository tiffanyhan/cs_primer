#include <stdio.h>

int *start_addr;

void overflow(int n) {
    int *curr_addr = &n;
    int offset = (char*)start_addr - (char*)curr_addr;
    printf("curr_addr: %p start_addr: %p\n", curr_addr, start_addr);
    printf("[%d] Address of n: %p; Size: %d bytes\n", n, curr_addr, offset);
    overflow(n+1);
}

int main(int argc, char *argv[]) {
    int n = 1;
    start_addr = &n;
    overflow(n);
}
