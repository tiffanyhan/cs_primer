#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>

pthread_mutex_t m;
pthread_cond_t c;
volatile int n = 1, f = 0, b = 0;

bool fizz_cond(int n, int f) {
  return n % 3 == 0 && f <= 0;
}

void *fizz(void *arg) {
  while (1) {
    // printf("fizz iteration, checking condition...\n");
    pthread_mutex_lock(&m);

    while (!fizz_cond(n, f)) {
      // printf("\tfizz condition not met, waiting...\n");
      pthread_cond_wait(&c, &m);
      // printf("\tfizz resumed\n");
    }

    if (fizz_cond(n, f)) {
      f = 1;
      printf("\tFIZZ");
    }
    pthread_mutex_unlock(&m); 
  }
}

bool buzz_cond(int n, int b) {
  return n % 5 == 0 && b <= 0;
}

void *buzz(void *arg) {
  while (1) {
    // printf("buzz iteration, checking condition...\n");
    pthread_mutex_lock(&m);

    while (!buzz_cond(n, b)) {
        // printf("\tbuzz condition not met, waiting...\n");
        pthread_cond_wait(&c, &m);
        // printf("\tbuzz resumed\n");
    }

    if (buzz_cond(n, b)) {
      b = 1;
      printf("\tBUZZ");
    }
    pthread_mutex_unlock(&m); 
  }
}

int main () {
  // start two threads, running fizz and buzz respectively
  pthread_mutex_init(&m, NULL);
  pthread_cond_init(&c, NULL);
  pthread_t t1, t2;

  pthread_create(&t1, NULL, fizz, NULL);
  pthread_create(&t2, NULL, buzz, NULL);

  // every 100ms, update n randomly from the range [0, 16), indefinitely
  while (1) {
    // printf("main iteration, acquiring lock...\n");
    pthread_mutex_lock(&m);
    n = rand() & 0xf;
    // n = 15;
    f = 0;
    b = 0;
    printf("\n%d:", n);
    pthread_cond_broadcast(&c);
    pthread_mutex_unlock(&m);

    usleep(100000);
  }
}
