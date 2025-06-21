#include "ringbuffer.h"
#include <stdio.h>
#include <pthread.h>

void RB_init(struct RingBuffer *rb) {
    rb->capacity = RB_CAPACITY;
    rb->size = 0;
    rb->wp = 0;
    rb->rp = 0;

    pthread_mutexattr_t mattr;
    pthread_mutexattr_init(&mattr);
    pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&rb->m, &mattr);
    pthread_mutexattr_destroy(&mattr);

    pthread_condattr_t cattr;
    pthread_condattr_init(&cattr);
    pthread_condattr_setpshared(&cattr, PTHREAD_PROCESS_SHARED);
    pthread_cond_init(&rb->not_full, &cattr);
    pthread_cond_init(&rb->not_empty, &cattr);
    pthread_condattr_destroy(&cattr);
}

void RB_add(struct RingBuffer *rb, int item) {
    pthread_mutex_lock(&rb->m);
    while (rb->size == rb->capacity) {
        pthread_cond_wait(&rb->not_full, &rb->m);
    }

    rb->buffer[rb->wp] = item;
    rb->wp = (rb->wp + 1) % rb->capacity;
    rb->size++;

    pthread_cond_signal(&rb->not_empty);
    pthread_mutex_unlock(&rb->m);
}

int RB_get(struct RingBuffer *rb) {
    pthread_mutex_lock(&rb->m);
    while (rb->size == 0) {
        pthread_cond_wait(&rb->not_empty, &rb->m);
    }

    int item = rb->buffer[rb->rp];
    rb->rp = (rb->rp + 1) % rb->capacity;
    rb->size--;

    pthread_cond_signal(&rb->not_full);
    pthread_mutex_unlock(&rb->m);
    return item;
}
