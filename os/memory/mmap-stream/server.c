#include "ringbuffer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

#define SIZE (1 << 20) // Test with this many bytes of data

int main () {
  // create shared memory object (assuming no errors)
  int shm_fd = shm_open("/my_shm", O_CREAT | O_RDWR, 0666);
  ftruncate(shm_fd, sizeof(struct RingBuffer));

  // map shared memory object into the process's virtual address space
  void *shm_ptr = mmap(NULL, sizeof(struct RingBuffer) + (4 * sizeof(int)), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
  struct RingBuffer *shared_rb = (struct RingBuffer*)shm_ptr;

  // initialize the shared ring buffer
  RB_init(shared_rb);

  // send ints
  int checksum, max, n;

  srand(0x1234);
  max = SIZE / sizeof(int);

  checksum = 0;
  for (int i = 0; i < max; i++) {
    n = rand();
    checksum ^= n;
    RB_add(shared_rb, n);
  }
  printf("Sent %d random ints to client, checksum %d\n", max, checksum);
}
