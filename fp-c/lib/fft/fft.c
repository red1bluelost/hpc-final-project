#include "fft/fft.h"

#include <math.h>

#include "util/math.h"

extern inline void fft_dit_r2(fft_vector_t* vec);

static void bitrev(fft_cmplx_t* data, size_t size) {
  int n = (int)size;
  for (int i = 0, r = 0; i <= n - 2; ++i) {
    if (i < r) {
      fft_cmplx_t tmp = data[i];
      data[i] = data[r];
      data[r] = tmp;
    }
    int k;
    for (k = n / 2; k <= r; k /= 2) r -= k;
    r += k;
  }
}

void fft_dit_r2_r(fft_cmplx_t* data, size_t size) {
  int n = (int)size;
  int nu = log2i(n);
  bitrev(data, size);
  for (int m = 1; m <= nu; ++m) {
    int l = 1 << m;
    int l2 = l / 2;
    for (int ir = 0; ir < l2; ++ir) {
      double d = -2 * M_PI * ((double)ir) / ((double)l);
      fft_cmplx_t w = cos(d) + I * sin(d);
      for (int it = ir; it < n; it += l) {
        int ib = it + l2;
        fft_cmplx_t t = data[ib] * w;
        data[ib] = data[it] - t;
        data[it] = data[it] + t;
      }
    }
  }
}
