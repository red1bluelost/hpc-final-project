#include "FFT/FFT.h"
#include "FFT/Vector.h"

#include <iostream>

int main() {
  fft::Vector V(8);
  for (std::size_t I = 0; I < 8; ++I) {
    if (I < 4)
      V[I] = 1;
    else
      V[I] = 0;
  }
  std::cout << V;
  fft::fftDitR2(V);
  std::cout << V;

  return 0;
}