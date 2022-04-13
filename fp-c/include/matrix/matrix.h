#ifndef FP_C_MATRIX_MATRIX_H
#define FP_C_MATRIX_MATRIX_H

#include <malloc.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct {
  size_t rows, cols;
  double* data;
} matrix_t;

inline void matrix_destroy(matrix_t* mat) {
  free(mat->data);
  mat->data = NULL;
}

int matrix_read_r(FILE* restrict stream, double** restrict data,
                  size_t* restrict rows, size_t* restrict cols);

inline int matrix_read(FILE* restrict stream, matrix_t* restrict mat) {
  return matrix_read_r(stream, &mat->data, &mat->rows, &mat->cols);
}

int matrix_write_r(FILE* restrict stream, const double* restrict data,
                   size_t rows, size_t cols);

inline int matrix_write(FILE* restrict stream, const matrix_t* restrict mat) {
  return matrix_write_r(stream, mat->data, mat->rows, mat->cols);
}

bool matrix_equal_r(const double* a_data, size_t a_rows, size_t a_cols,
                    const double* b_data, size_t b_rows, size_t b_cols);

inline bool matrix_equal(const matrix_t* mat_a, const matrix_t* mat_b) {
  return matrix_equal_r(mat_a->data, mat_a->rows, mat_a->cols, mat_b->data,
                        mat_b->rows, mat_b->cols);
}

#endif  // FP_C_MATRIX_MATRIX_H
