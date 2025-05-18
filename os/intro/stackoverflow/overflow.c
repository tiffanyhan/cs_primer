#include <stdio.h>
#include <unistd.h>

void overflow(int n, int *start_addr) {
    long offset = (long)start_addr - (long)&n;
    printf("[PID: %d F%d] Address of n: %p; Size: %ld bytes\n", getpid(), n, &n, offset);
    overflow(n+1, start_addr);
}

int main(int argc, char *argv[]) {
    int n = 1;
    overflow(n, &n);
}
