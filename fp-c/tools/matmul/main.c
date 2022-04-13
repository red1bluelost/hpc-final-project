#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "matrix/matrix.h"
#include "matrix/multiply.h"
#include "util/argparse.h"

static uint64_t clock_us() {
  struct timespec t;
  clock_gettime(CLOCK_MONOTONIC, &t);
  return (t.tv_sec * 1000000) + (t.tv_nsec / 1000);
}

int main(const int argc, const char *argv[]) {
  bool verify = arg_bool(argc, argv, "-v");

  uint64_t start = clock_us();
  matrix_t mat_a, mat_b;
  {
    FILE *afile = fopen("A.mat", "r");
    assert(afile != NULL && "could not open file A");
    FILE *bfile = fopen("B.mat", "r");
    assert(bfile != NULL && "could not open file B");
    int err = matrix_read(afile, &mat_a);
    (void)err, assert(err == 0 && "failed to load matrix A");
    err = matrix_read(bfile, &mat_b);
    (void)err, assert(err == 0 && "failed to load matrix B");
    fclose(afile);
    fclose(bfile);
  }
  uint64_t end_import = clock_us();

  matrix_t mat_c;
  matrix_multiply(&mat_a, &mat_b, &mat_c);
  uint64_t end_multiply = clock_us();

  {
    FILE *cfile = fopen("C.mat", "w");
    assert(cfile != NULL && "could not open file C");
    int err = matrix_write(cfile, &mat_c);
    (void)err, assert(err == 0 && "failed to write matrix C");
    fclose(cfile);
  }
  uint64_t end_export = clock_us();

  {
    uint64_t dur_import = end_import - start,
             dur_multiply = end_multiply - end_import,
             dur_export = end_export - end_multiply;
    printf("-- Timing MatMul C --\n");
    printf("Import: %lu us\n", dur_import);
    printf("Multiply: %lu us\n", dur_multiply);
    printf("Export: %lu us\n", dur_export);
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

  matrix_destroy(&mat_a);
  matrix_destroy(&mat_b);
  matrix_destroy(&mat_c);
  return 0;
}
