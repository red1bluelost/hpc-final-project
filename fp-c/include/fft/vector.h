#ifndef FP_C_FFT_VECTOR_H
#define FP_C_FFT_VECTOR_H

#include <complex.h>
#include <malloc.h>
#include <stddef.h>

typedef double complex fft_cmplx_t;

typedef struct {
  size_t size;
  fft_cmplx_t* data;
} fft_vector_t;

inline void fft_vector_destroy(fft_vector_t* vec) {
  free(vec->data);
  vec->data = NULL;
}

int fft_vector_read_r(FILE* restrict stream, fft_cmplx_t** restrict data,
                      size_t* restrict size);

inline int fft_vector_read(FILE* restrict stream, fft_vector_t* restrict vec) {
  return fft_vector_read_r(stream, &vec->data, &vec->size);
}

int fft_vector_write_r(FILE* restrict stream, const fft_cmplx_t* restrict data,
                       size_t size);

inline int fft_vector_write(FILE* restrict stream,
                            const fft_vector_t* restrict vec) {
  return fft_vector_write_r(stream, vec->data, vec->size);
}

#endif  // FP_C_FFT_VECTOR_H
