#ifndef FP_C_MATRIX_MULTIPLY_H
#define FP_C_MATRIX_MULTIPLY_H

#include "matrix/matrix.h"

void matrix_multiply_r(const double *a_data, size_t a_rows, size_t a_cols,
                       const double *b_data, size_t b_rows, size_t b_cols,
                       double **c_data);

inline void matrix_multiply(const matrix_t *a_mat, const matrix_t *b_mat,
                            matrix_t *c_mat) {
  matrix_multiply_r(a_mat->data, a_mat->rows, a_mat->cols, b_mat->data,
                    b_mat->rows, b_mat->cols, &c_mat->data);
  c_mat->rows = a_mat->rows, c_mat->cols = b_mat->cols;
}

void matrix_multiply_fast_r(const double *a_data, size_t a_rows, size_t a_cols,
                            const double *b_data, size_t b_rows, size_t b_cols,
                            double **c_data);

inline void matrix_multiply_fast(const matrix_t *a_mat, const matrix_t *b_mat,
                                 matrix_t *c_mat) {
  matrix_multiply_fast_r(a_mat->data, a_mat->rows, a_mat->cols, b_mat->data,
                         b_mat->rows, b_mat->cols, &c_mat->data);
  c_mat->rows = a_mat->rows, c_mat->cols = b_mat->cols;
}

#endif  // FP_C_MATRIX_MULTIPLY_H
