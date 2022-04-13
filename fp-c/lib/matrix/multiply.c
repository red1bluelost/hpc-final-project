#include "matrix/multiply.h"

#include <assert.h>
#include <malloc.h>

extern inline void matrix_multiply(const matrix_t *A, const matrix_t *B,
                                   matrix_t *C);

void matrix_multiply_r(const double *Adata, size_t ARows, size_t ACols,
                       const double *Bdata, size_t BRows, size_t BCols,
                       double **Cdata) {
  (void)BRows, assert(ACols == BRows && "mismatch of dimensions");
  double *data_out = malloc(ARows * BCols * sizeof(*data_out));
  for (size_t R = 0; R < ARows; ++R) {
    for (size_t C = 0; C < BCols; ++C) {
      double Sum = 0.0;
      for (size_t K = 0; K < ACols; K++)
        Sum += Adata[R * ACols + K] * Bdata[K * BCols + C];
      data_out[R * BCols + C] = Sum;
    }
  }
  *Cdata = data_out;
}
