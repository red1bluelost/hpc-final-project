#ifndef FP_CPP_MATRIX_MULTIPLY_H
#define FP_CPP_MATRIX_MULTIPLY_H

#include "Matrix/Matrix.h"

namespace matrix {
Dense multiply(const Dense &A, const Dense &B);
Dense multiplyBlocked(const Dense &A, const Dense &B);
Dense multiplyParallel(const Dense &A, const Dense &B);
Dense multiplyFast(const Dense &A, const Dense &B);
} // namespace matrix

#endif // FP_CPP_MATRIX_MULTIPLY_H
