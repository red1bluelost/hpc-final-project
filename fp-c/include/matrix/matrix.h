#ifndef FP_C_MATRIX_MATRIX_H
#define FP_C_MATRIX_MATRIX_H

#include <malloc.h>
#include <stddef.h>
#include <stdio.h>

typedef struct {
  size_t rows, cols;
  double* data;
} matrix_t;

inline void matrix_destroy(matrix_t* mat) { free(mat->data); }

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

#endif  // FP_C_MATRIX_MATRIX_H
