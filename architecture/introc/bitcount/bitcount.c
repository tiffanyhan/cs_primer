#include <assert.h>
#include <stdio.h>


int bitcount(unsigned num) {
    int set = 0;
    int numBytes = sizeof(num);

    printf("Num of bytes: %d\n", numBytes);

    while (num > 0) {
        num &= num - 1;
        set++;
    }

    printf("Set: %d\n", set);
    return set;
}


int main() {
    assert(bitcount(0) == 0);
    assert(bitcount(1) == 1);
    assert(bitcount(3) == 2);
    assert(bitcount(8) == 1);
    // harder case:
    assert(bitcount(0xffffffff) == 32);
    printf("OK\n");

    int size = sizeof(size_t);
    printf("Num of bytes of size_t type: %d\n", size);
}
