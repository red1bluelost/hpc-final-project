#include <assert.h>
#include <pthread.h>
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

typedef struct {
  const char *fname;
  matrix_t *mat;
} file_thread_arg_t;

static void *file_thread(void *argp) {
  file_thread_arg_t *args = (file_thread_arg_t *)argp;
  FILE *file = fopen(args->fname, "r");
  assert(file != NULL && "could not open file");
  int err = matrix_read(file, args->mat);
  (void)err, assert(err == 0 && "failed to load matrix");
  fclose(file);
  return NULL;
}

int main(const int argc, const char *argv[]) {
  bool verify = arg_bool(argc, argv, "-v");

  uint64_t start = clock_us();
  matrix_t mat_a, mat_b;
  {
    pthread_t thrd_a, thrd_b;
    file_thread_arg_t args_a = {"A.mat", &mat_a}, args_b = {"B.mat", &mat_b};
    pthread_create(&thrd_a, NULL, file_thread, &args_a);
    pthread_create(&thrd_b, NULL, file_thread, &args_b);
    pthread_join(thrd_a, NULL);
    pthread_join(thrd_b, NULL);
  }
  uint64_t end_import = clock_us();

  matrix_t mat_c;
  matrix_multiply_fast(&mat_a, &mat_b, &mat_c);
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
