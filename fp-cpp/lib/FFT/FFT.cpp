#include "FFT/FFT.h"
#include "Util/Math.h"

#include <algorithm>
#include <numbers>

using namespace fft;

static void bitrev(Vector &X) {
  int N = static_cast<int>(X.size());
  for (int I = 0, R = 0; I <= N - 2; ++I) {
    if (I < R)
      std::swap(X[I], X[R]);
    int K;
    for (K = N / 2; K <= R; K /= 2)
      R -= K;
    R += K;
  }
}

void fft::fftDitR2(Vector &X) {
  int N = static_cast<int>(X.size());
  int NU = util::log2(N);
  bitrev(X);
  for (int M = 1; M <= NU; ++M) {
    int L = 1 << M;
    int L2 = L / 2;
    for (int IR = 0; IR < L2; ++IR) {
      double D = -2 * std::numbers::pi_v<double> * static_cast<double>(IR) /
                 static_cast<double>(L);
      CmplxT W = std::polar(1.0, D);
      for (int IT = IR; IT < N; IT += L) {
        int IB = IT + L2;
        CmplxT T = X[IB] * W;
        X[IB] = X[IT] - T;
        X[IT] = X[IT] + T;
      }
    }
  }
}
