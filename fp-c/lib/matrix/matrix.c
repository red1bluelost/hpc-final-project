#include "matrix/matrix.h"

#include <malloc.h>
#include <math.h>

extern inline void matrix_destroy(matrix_t* mat);
extern inline int matrix_read(FILE* restrict stream, matrix_t* restrict mat);
extern inline int matrix_write(FILE* restrict stream,
                               const matrix_t* restrict mat);
extern inline bool matrix_equal(const matrix_t* mat_a, const matrix_t* mat_b);

int matrix_read_r(FILE* restrict stream, double** restrict data,
                  size_t* restrict rows, size_t* restrict cols) {
  int ret;
  double* data_in = NULL;
  if ((ret = fscanf(stream, "%zu %zu\n", rows, cols)) != 2) goto error;

  size_t size = (*rows) * (*cols);
  data_in = malloc(size * sizeof(*data_in));
  for (size_t i = 0; i < size; ++i) {
    if ((ret = fscanf(stream, "%lf ", &data_in[i])) != 1) goto error;
  }
  *data = data_in;
  return 0;
error:
  if (!data_in) free(data_in);
  return ret;
}

int matrix_write_r(FILE* restrict stream, const double* restrict data,
                   size_t rows, size_t cols) {
  int ret;
  if ((ret = fprintf(stream, "%zu %zu\n", rows, cols)) < 4) return ret;
  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; ++j) {
      size_t idx = i * cols + j;
      double d = data[idx];
      if ((ret = fprintf(stream, "%lf ", d) < 2)) return ret;
    }
    if ((ret = fprintf(stream, "\n") < 1)) return ret;
  }
  return 0;
}

bool matrix_equal_r(const double* a_data, size_t a_rows, size_t a_cols,
                    const double* b_data, size_t b_rows, size_t b_cols) {
  if (a_rows != b_rows || a_cols != b_cols) return false;
  for (size_t i = 0; i < a_rows * a_cols; ++i)
    if (fabs(a_data[i] - b_data[i]) > 0.0001) return false;
  return true;
}
