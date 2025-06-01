#include <stdio.h>
#include <pthread.h>

#define EACH_COUNT 1000000000

pthread_mutex_t mutex;
volatile int counter = 0;

void* thread_entry(void *arg) {
  for (int i = 0; i < EACH_COUNT; i++) {
    pthread_mutex_lock(&mutex);
    counter++;
    pthread_mutex_unlock(&mutex);
  }
  return NULL;
}


int main () {
  pthread_t p1, p2;
  pthread_mutex_init(&mutex, NULL);
  pthread_create(&p1, NULL, thread_entry, NULL);
  pthread_create(&p2, NULL, thread_entry, NULL);
  pthread_join(p1, NULL);
  pthread_join(p2, NULL);
  pthread_mutex_destroy(&mutex);
  printf("Final count: %d (expected %d)\n", counter, 2 * EACH_COUNT);
}
