#include "matrix/multiply.h"

#include <assert.h>
#include <malloc.h>
#include <pthread.h>
#include <unistd.h>

#include "util/math.h"

#define BLOCKSIZE 8

extern inline void matrix_multiply(const matrix_t *a_mat, const matrix_t *b_mat,
                                   matrix_t *c_mat);
extern inline void matrix_multiply_fast(const matrix_t *a_mat,
                                        const matrix_t *b_mat, matrix_t *c_mat);

void matrix_multiply_r(const double *a_data, size_t a_rows, size_t a_cols,
                       const double *b_data, size_t b_rows, size_t b_cols,
                       double **c_data) {
  (void)b_rows, assert(a_cols == b_rows && "mismatch of dimensions");
  double *data_out = malloc(a_rows * b_cols * sizeof(*data_out));
  for (size_t R = 0; R < a_rows; ++R) {
    for (size_t C = 0; C < b_cols; ++C) {
      double Sum = 0.0;
      for (size_t K = 0; K < a_cols; ++K)
        Sum += a_data[R * a_cols + K] * b_data[K * b_cols + C];
      data_out[R * b_cols + C] = Sum;
    }
  }
  *c_data = data_out;
}

typedef struct {
  size_t start, width;
  size_t a_rows, a_cols, b_cols;
  const double *a_data;
  const double *b_data;
  double *c_data;
  pthread_t thread;
} mm_thrd_fast_arg_t;

static void *mm_thrd_fast(void *arg) {
  mm_thrd_fast_arg_t *t = (mm_thrd_fast_arg_t *)arg;
  for (size_t RO = t->start; RO < minz(t->start + t->width, t->a_rows);
       RO += BLOCKSIZE) {
    for (size_t CO = 0; CO < t->b_cols; CO += BLOCKSIZE) {
      for (size_t KO = 0; KO < t->a_cols; KO += BLOCKSIZE) {
        for (size_t R = RO; R < minz(RO + BLOCKSIZE, t->a_rows);) {
          if (R + 4 <= minz(RO + BLOCKSIZE, t->a_rows)) {
            for (size_t C = CO; C < minz(CO + BLOCKSIZE, t->b_cols); ++C) {
              double sum[4];
              for (size_t i = 0; i < 4; ++i)
                sum[i] = KO == 0 ? 0.0 : t->c_data[(R + i) * t->b_cols + C];
              for (size_t K = KO; K < minz(KO + BLOCKSIZE, t->a_cols); ++K)
                for (size_t i = 0; i < 4; ++i)
                  sum[i] += t->a_data[(R + i) * t->a_cols + K] *
                            t->b_data[K * t->b_cols + C];
              for (size_t i = 0; i < 4; ++i)
                t->c_data[(R + i) * t->b_cols + C] = sum[i];
            }
            R += 4;
          } else {
            for (size_t C = CO; C < minz(CO + BLOCKSIZE, t->b_cols); ++C) {
              double sum = KO == 0 ? 0.0 : t->c_data[R * t->b_cols + C];
              for (size_t K = KO; K < minz(KO + BLOCKSIZE, t->a_cols); ++K)
                sum +=
                    t->a_data[R * t->a_cols + K] * t->b_data[K * t->b_cols + C];
              t->c_data[R * t->b_cols + C] = sum;
            }
            ++R;
          }
        }
      }
    }
  }
  return NULL;
}

void matrix_multiply_fast_r(const double *a_data, size_t a_rows, size_t a_cols,
                            const double *b_data, size_t b_rows, size_t b_cols,
                            double **c_data) {
  (void)b_rows, assert(a_cols == b_rows && "mismatch of dimensions");

  size_t max_proc = sysconf(_SC_NPROCESSORS_ONLN);
  size_t work_width = divruz(divruz(a_rows, max_proc), BLOCKSIZE) * BLOCKSIZE;

  double *data_out = malloc(a_rows * b_cols * sizeof(*data_out));
  mm_thrd_fast_arg_t *thrds = malloc(max_proc * sizeof(*thrds));

  for (size_t tid = 0; tid < max_proc && tid * work_width < a_rows; ++tid) {
    mm_thrd_fast_arg_t thrd = {.start = tid * work_width,
                               .width = work_width,
                               .a_rows = a_rows,
                               .a_cols = a_cols,
                               .b_cols = b_cols,
                               .a_data = a_data,
                               .b_data = b_data,
                               .c_data = data_out};
    thrds[tid] = thrd;
    pthread_create(&thrds[tid].thread, NULL, mm_thrd_fast, &thrds[tid]);
  }
  for (size_t tid = 0; tid < max_proc && tid * work_width < a_rows; ++tid)
    pthread_join(thrds[tid].thread, NULL);

  *c_data = data_out;
  free(thrds);
}
