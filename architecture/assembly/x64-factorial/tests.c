#include <assert.h>
#include <stdio.h>

extern int factorial(int n);

int main(void) {
  assert(factorial(0) == 1);
  assert(factorial(1) == 1);
  assert(factorial(4) == 24);
  assert(factorial(5) == 120);
  assert(factorial(10) == 3628800);
  assert(factorial(12) == 479001600);
  printf("OK\n");
}
