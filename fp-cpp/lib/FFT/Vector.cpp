#include "FFT/Vector.h"

#include <iomanip>
#include <iterator>

using namespace fft;

std::ostream &operator<<(std::ostream &OS, const Vector &V) {
  OS << V.size() << '\n' << std::setprecision(8);
  for (const auto &C : V) {
    double R = C.real(), I = C.imag();
    OS << '(' << std::noshowpos << R << std::showpos << I << "i)\n";
  }
  return OS << std::noshowpos;
}

std::istream &operator>>(std::istream &IS, Vector &V) {
  V.clear();
  IndexT Size;
  IS >> Size;
  V.reserve(Size);
  for (IndexT I = 0; I < Size; ++I) {
    char IgnC;
    double Real, Imag;
    IS >> IgnC >> Real >> Imag >> IgnC >> IgnC;
    V.emplace_back(Real, Imag);
  }
  return IS;
}