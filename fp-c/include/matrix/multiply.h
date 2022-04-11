#ifndef FP_C_MATRIX_MULTIPLY_H
#define FP_C_MATRIX_MULTIPLY_H

#include "matrix/matrix.h"

void matrix_multiply_r(const double *Adata, size_t ARows, size_t ACols,
                       const double *Bdata, size_t BRows, size_t BCols,
                       double **Cdata);

inline void matrix_multiply(const matrix_t *A, const matrix_t *B, matrix_t *C) {
  matrix_multiply_r(A->data, A->rows, A->cols, B->data, B->rows, B->cols,
                    &C->data);
  C->rows = A->rows, C->cols = B->cols;
}

#endif  // FP_C_MATRIX_MULTIPLY_H
