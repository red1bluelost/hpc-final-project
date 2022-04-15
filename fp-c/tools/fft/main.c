#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "fft/fft.h"
#include "util/argparse.h"

static uint64_t clock_us() {
  struct timespec t;
  clock_gettime(CLOCK_MONOTONIC, &t);
  return (t.tv_sec * 1000000) + (t.tv_nsec / 1000);
}

int main(const int argc, const char *argv[]) {
  bool verify = arg_bool(argc, argv, "-v");

  uint64_t start = clock_us();
  fft_vector_t vec;
  {
    FILE *vfile = fopen("X.vec", "r");
    assert(vfile != NULL && "could not open file X");
    int err = fft_vector_read(vfile, &vec);
    (void)err, assert(err == 0 && "failed to load vector X");
    fclose(vfile);
  }
  uint64_t end_import = clock_us();

  fft_dit_r2(&vec);
  uint64_t end_fft = clock_us();

  {
    FILE *xffile = fopen("Xf.vec", "w");
    assert(xffile != NULL && "could not open file XF");
    int err = fft_vector_write(xffile, &vec);
    (void)err, assert(err == 0 && "failed to write vector X");
    fclose(xffile);
  }
  uint64_t end_export = clock_us();

  {
    uint64_t dur_import = end_import - start, dur_fft = end_fft - end_import,
             dur_export = end_export - end_fft, dur_total = end_export - start;
    printf("-- Timing FFT C --\n");
    printf("Import: %lu us\n", dur_import);
    printf("FFT: %lu us\n", dur_fft);
    printf("Export: %lu us\n", dur_export);
    printf("Total: %lu us\n", dur_total);
  }

  if (verify) {
    fft_vector_t vec_c;
    FILE *vfile = fopen("X.vec", "r");
    assert(vfile != NULL && "could not open file X");
    fseek(vfile, 0, SEEK_SET);
    int err = fft_vector_read(vfile, &vec_c);
    (void)err, assert(err == 0 && "failed to load vector X");
    fclose(vfile);
    fft_dit_r2(&vec_c);
    for (size_t i = 0, e = vec_c.size; i < e; ++i) {
      if (fabs(creal(vec.data[i]) - creal(vec_c.data[i])) > 0.0001 ||
          fabs(cimag(vec.data[i]) - cimag(vec_c.data[i])) > 0.0001) {
        fprintf(stderr, "fft failed verification with unequal vectors\n");
        return 1;
      }
    }
    fft_vector_destroy(&vec_c);
  }

  fft_vector_destroy(&vec);
  return 0;
}
