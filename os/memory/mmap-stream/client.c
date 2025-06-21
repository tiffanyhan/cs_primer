#include "ringbuffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <pthread.h>

#define SIZE (1 << 20) // Test with this many bytes of data

int main () {
  // open the shared memory object created by server
  int shm_fd = shm_open("/my_shm", O_RDWR, 0);
  if (shm_fd < 0) {
    perror("shm_open failed");
    exit(1);
  }

  // map shared memory object into the process's virtual address space
  void *shm_ptr = mmap(NULL, sizeof(struct RingBuffer), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
  if (shm_ptr == MAP_FAILED) {
    perror("mmap failed");
    exit(1);
  }

  // get a ptr to the existing shared ring buffer
  struct RingBuffer *shared_rb = (struct RingBuffer*)shm_ptr;

  int checksum, max, n;
  struct timespec start, end;

  max = SIZE / sizeof(int);

  // receive a bunch of data
  clock_gettime(CLOCK_MONOTONIC, &start);
  checksum = 0;
  for (int i = 0; i < max; i++) {
    // receive ints from server
    n = RB_get(shared_rb);
    checksum ^= n;
  }
  clock_gettime(CLOCK_MONOTONIC, &end);

  float secs =
      (float)(end.tv_nsec - start.tv_nsec) / 1e9 + (end.tv_sec - start.tv_sec);
  float mibs = (float)SIZE / secs / (1 << 20);

  // Benchmark: Received at 6.329 MiB/s. Checksum: 1159889755
  // Unlimited buffer: Received at 740.741 MiB/s. Checksum: 1286510572
  // Buffer size of 4 bytes: Received at 0.607 MiB/s. Checksum: 1286510572
  // Buffer size of 4096 bytes: Received at 25.654 MiB/s. Checksum: 1286510572
  printf("Received at %.3f MiB/s. Checksum: %d\n", mibs, checksum);
}
