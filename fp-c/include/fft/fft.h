#ifndef FP_C_FFT_FFT_H
#define FP_C_FFT_FFT_H

#include "fft/vector.h"

void fft_dit_r2_r(fft_cmplx_t* data, size_t size);

inline void fft_dit_r2(fft_vector_t* vec) {
  fft_dit_r2_r(vec->data, vec->size);
}

#endif  // FP_C_FFT_FFT_H
