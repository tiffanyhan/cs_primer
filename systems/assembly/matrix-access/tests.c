#include <assert.h>
#include <stdio.h>

extern int index(int *matrix, int rows, int cols, int rindex, int cindex);

int main(void) {
  // 1 row, 4 cols, rowIndex=0, colIndex=2
  // (rowIndex * cols) + colIndex
  int matrix1[1][4] = {{1, 2, 3, 4}};
  assert(index((int *)matrix1, 1, 4, 0, 2) == 3);

  // 4 rows, 1 col, rowIndex=1, colIndex=0
  // (rowIndex * cols) + colIndex
  int matrix2[4][1] = {{1}, {2}, {3}, {4}};
  assert(index((int *)matrix2, 4, 1, 1, 0) == 2);

  // 2 rows, 3 cols, rowIndex=1, colIndex=2
  // (rowIndex * cols) + colIndex
  int matrix3[2][3] = {{1, 2, 3}, {4, 5, 6}};
  assert(index((int *)matrix3, 2, 3, 1, 2) == 6);

  printf("OK\n");
}
