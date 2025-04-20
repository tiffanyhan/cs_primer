#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define STARTING_CAPACITY 8

typedef struct DA {
  void **arr_ptr;
  int size;
  int capacity;
} DA;


DA* DA_new (void) {
  DA *da;
  da = (DA*)malloc(sizeof(DA));
  da->arr_ptr = malloc(sizeof(void*) * STARTING_CAPACITY);
  da->capacity = STARTING_CAPACITY;
  return da;
}

int DA_size(DA *da) {
  return da->size;
}

void DA_push (DA* da, void* ptr) {
  int size = DA_size(da);
  int capacity = da->capacity;

  // expand arr if capacity reached
  if (size == capacity) {
    void **old_arr_ptr = da->arr_ptr;
    da->arr_ptr = malloc(sizeof(void*) * (capacity*2));
    da->capacity = capacity*2;
    for (int i = 0; i < size; i++) {
      da->arr_ptr[i] = old_arr_ptr[i];
    }
    free(old_arr_ptr);
  }

  da->arr_ptr[size] = ptr;
  da->size++;
}

void *DA_pop(DA *da) {
  int size = DA_size(da);

  if (size > 0) {
    void *ptr = da->arr_ptr[size-1];
    da->arr_ptr[size-1] = NULL;
    da->size--;
    return ptr;
  }

  return NULL;
}

void DA_set(DA *da, void *ptr, int i) {
  // set at a given index, if already set
  int size = DA_size(da);
  if (0 < i < size) {
    da->arr_ptr[i] = ptr;
  }
}

void *DA_get(DA *da, int i) {
  // get from a given index, if possible
  int size = DA_size(da);
  if (0 < i < size) {
    return da->arr_ptr[i];
  }

  return NULL;
}

void DA_free(DA *da) {
  free(da->arr_ptr);
  free(da);
  return;
}

int main() {
    DA* da = DA_new();
    assert(DA_size(da) == 0);

    // basic push and pop test
    int x = 5;
    float y = 12.4;
    DA_push(da, &x);
    DA_push(da, &y);
    assert(DA_size(da) == 2);

    assert(DA_pop(da) == &y);
    assert(DA_size(da) == 1);

    assert(DA_pop(da) == &x);
    assert(DA_size(da) == 0);
    assert(DA_pop(da) == NULL);

    // basic set/get test
    DA_push(da, &x);
    DA_set(da, &y, 0);
    
    assert(DA_get(da, 0) == &y);
    DA_pop(da);
    assert(DA_size(da) == 0);

    // expansion test
    DA *da2 = DA_new(); // use another DA to show it doesn't get overriden
    DA_push(da2, &x);

    int i, n = 100 * STARTING_CAPACITY, arr[n];
    for (i = 0; i < n; i++) {
      arr[i] = i;
      DA_push(da, &arr[i]);
    }
    assert(DA_size(da) == n);
    for (i = 0; i < n; i++) {
      assert(DA_get(da, i) == &arr[i]);
    }
    for (; n; n--)
      DA_pop(da);
    assert(DA_size(da) == 0);
    assert(DA_pop(da2) == &x); // this will fail if da doesn't expand

    DA_free(da);
    DA_free(da2);
    printf("OK\n");
}
