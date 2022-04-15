#ifndef FP_C_UTIL_MATH_H
#define FP_C_UTIL_MATH_H

#include <stddef.h>

inline int log2i(int i) {
  int r = 0;
  while (1 << r < i) r++;
  return r;
}

inline size_t divruz(size_t n, size_t d) { return (n + d - 1) / d; }

inline size_t minz(size_t a, size_t b) { return a < b ? a : b; }

#endif  // FP_C_UTIL_MATH_H
