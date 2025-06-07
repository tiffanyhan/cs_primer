#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "cputime.h"

// merge accepts an arr whose left and right halves
// are independently sorted. It then merges both halves
// together into a complete sorted arr.
void merge(int *arr, int n, int mid) {
  int left = 0, right = mid, i;
  int *x = malloc(n * sizeof(int));
  // copy the ith item from either the left or right part
  for (i = 0; i < n; i++) {
    if (right == n)
      x[i] = arr[left++];
    else if (left == mid)
      x[i] = arr[right++];
    else if (arr[right] < arr[left])
      x[i] = arr[right++];
    else
      x[i] = arr[left++];
  }
  // transfer from temporary array back to given one
  for (i = 0; i < n; i++)
    arr[i] = x[i];
  free(x);
}

// msort divides the given arr instead two halves.
// n represents the number of items in the arr.
void msort(int *arr, int n) {
  if (n < 2)
    return;
  int mid = n / 2;
  msort(arr, mid);
  msort(arr + mid, n - mid);
  merge(arr, n, mid);
}

typedef struct {
  int *arr;
  int n;
} ThreadArgs;

// thread_entry should be called from main
// with 1/2 of the array
void* thread_entry(void *arg) {
  ThreadArgs *args = (ThreadArgs *)arg;
  msort(args->arr, args->n);
  return NULL;
}

// Benchmark:
// [pid 74077] real: 3.055s user: 3.008s sys: 0.047
// 
// Two Threads:
// [pid 76933] real: 1.679s user: 3.174s sys: 0.057
int main () {
  int n = 1 << 24;
  int *arr = malloc(n * sizeof(int));
  // populate array with n many random integers
  srand(1234);
  for (int i = 0; i < n; i++)
    arr[i] = rand();

  fprintf(stderr, "Sorting %d random integers\n", n);

  int mid = n / 2; // boundary btwn the two halves
  

  pthread_t p1, p2;
  ThreadArgs args1 = {
    .arr = arr,
    .n = mid,
  };
  ThreadArgs args2 = {
    .arr = arr + mid,
    .n = n - mid,
  };


  // actually sort, and time cpu use
  struct profile_times t;
  profile_start(&t);

  pthread_create(&p1, NULL, thread_entry, (void *)&args1);
  pthread_create(&p2, NULL, thread_entry, (void *)&args2);

  pthread_join(p1, NULL);
  pthread_join(p2, NULL);

  merge(arr, n, mid); // now that both halves are sorted, perform final merge

  profile_log(&t);

  // assert that the output is sorted
  for (int i = 0; i < n - 1; i++)
    if (arr[i] > arr[i + 1]) {
      printf("error: arr[%d] = %d > arr[%d] = %d", i, arr[i], i + 1,
             arr[i + 1]);
      return 1;
    }
    return 0;
}
