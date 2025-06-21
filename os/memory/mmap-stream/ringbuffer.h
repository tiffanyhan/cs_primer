#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <pthread.h>

#define RB_CAPACITY 4096

// Size of shared memory needed for the ring buffer
#define SHM_SIZE sizeof(struct RingBuffer)

struct RingBuffer {
    int buffer[RB_CAPACITY];  // Inline buffer for cross-process sharing
    int capacity;
    int size;
    int wp;
    int rp;
    pthread_mutex_t m;
    pthread_cond_t not_full;
    pthread_cond_t not_empty;
};

// Initialize a ring buffer struct in shared memory.
// Must be called once by the creator process.
void RB_init(struct RingBuffer *rb);

// Add an item to the ring buffer (blocks if full)
void RB_add(struct RingBuffer *rb, int item);

// Get an item from the ring buffer (blocks if empty)
int RB_get(struct RingBuffer *rb);

#endif // RINGBUFFER_H
