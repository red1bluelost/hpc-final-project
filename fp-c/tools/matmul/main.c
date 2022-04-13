#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "matrix/matrix.h"
#include "matrix/multiply.h"
#include "util/argparse.h"

int main(const int argc, const char *argv[]) {
  bool verify = arg_bool(argc, argv, "-v");

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

  if (verify) {
    matrix_t mat_v;
    matrix_multiply(&mat_a, &mat_b, &mat_v);
    if (!matrix_equal(&mat_c, &mat_v)) {
      fprintf(stderr, "multiplication failed\n");
      return EXIT_FAILURE;
    }
    matrix_destroy(&mat_v);
  }

  fclose(afile);
  fclose(bfile);
  matrix_destroy(&mat_a);
  matrix_destroy(&mat_b);
  matrix_destroy(&mat_c);
  return 0;
}
