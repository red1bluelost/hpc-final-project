#include "Matrix/Multiply.h"

#include <cassert>

using namespace matrix;

Dense matrix::multiply(const Dense &A, const Dense &B) {
  auto[ARows, ACols] = A.dim();
  auto[BRows, BCols] = B.dim();
  assert(ACols == BRows && "mismatch of dimensions");

  Dense CMat(ARows, BCols);
  for (IndexT R = 0; R < ARows; ++R) {
    for (IndexT C = 0; C < BCols; ++C) {
      double Sum = 0.0;
      for (IndexT K = 0; K < ACols; K++)
        Sum += A.at(R, K) * B.at(K, C);
      CMat.at(R, C) = Sum;
    }
  }
  return CMat;
}