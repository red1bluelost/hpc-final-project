#include "fft/vector.h"

extern inline void fft_vector_destroy(fft_vector_t* vec);
extern inline int fft_vector_read(FILE* restrict stream,
                                  fft_vector_t* restrict vec);
extern inline int fft_vector_write(FILE* restrict stream,
                                   const fft_vector_t* restrict vec);

int fft_vector_read_r(FILE* restrict stream, fft_cmplx_t** restrict data,
                      size_t* restrict size) {
  int ret;
  fft_cmplx_t* data_in = NULL;
  if ((ret = fscanf(stream, "%zu\n", size)) != 1) goto error;

  data_in = malloc((*size) * sizeof(*data_in));
  for (size_t i = 0; i < (*size); ++i) {
    double real, imag;
    if ((ret = fscanf(stream, "(%lf%lfi)\n", &real, &imag)) != 2) goto error;
    data_in[i] = real + imag * I;
  }
  *data = data_in;
  return 0;
error:
  if (!data_in) free(data_in);
  return ret;
}

int fft_vector_write_r(FILE* restrict stream, const fft_cmplx_t* restrict data,
                       size_t size) {
  int ret;
  if ((ret = fprintf(stream, "%zu\n", size)) < 2) return ret;
  for (size_t i = 0; i < size; ++i)
    if ((ret =
             fprintf(stream, "(%f%+fi)\n", creal(data[i]), cimag(data[i])) < 7))
      return ret;
  return 0;
}
