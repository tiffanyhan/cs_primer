#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/mman.h>

int main () {
  int *n = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

  int stat;
  srand(time(NULL));

  if (fork() == 0) {
    // as the child, write a random number to shared memory (TODO!)
    *n = rand();
    printf("Child has written %d to address %p\n", *n, n);
    exit(0);
  } else {
    // as the parent, wait for the child and read out its number
    wait(&stat);
    printf("Parent reads %d from address %p\n", *n, n);
  }
}
