#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "matrix/matrix.h"
#include "matrix/multiply.h"

int main() {
  FILE *afile = fopen("A.mat", "r");
  assert(afile != NULL && "could not open file A");
  FILE *bfile = fopen("B.mat", "r");
  assert(bfile != NULL && "could not open file B");

  matrix_t mat_a, mat_b;
  {
    int err = matrix_read(afile, &mat_a);
    assert(err == 0 && "failed to load matrix A");
    err = matrix_read(bfile, &mat_b);
    assert(err == 0 && "failed to load matrix B");
  }

  matrix_t mat_c;
  matrix_multiply(&mat_a, &mat_b, &mat_c);

  {
    int err = matrix_write(stdout, &mat_c);
    assert(err == 0 && "failed to write matrix C");
  }

  fclose(afile);
  fclose(bfile);
  matrix_destroy(&mat_a);
  matrix_destroy(&mat_b);
  matrix_destroy(&mat_c);
  return 0;
}
