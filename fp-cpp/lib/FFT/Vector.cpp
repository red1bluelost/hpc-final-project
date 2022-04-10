#include "FFT/Vector.h"

#include <iomanip>
#include <iterator>

using namespace fft;

std::ostream &operator<<(std::ostream &OS, const Vector &V) {
  OS << V.size() << '\n' << std::setprecision(8);
  for (const auto &C : V) {
    double R = C.real(), I = C.imag();
    OS << '(' << R << (I < 0 ? "" : "+") << I << "i)\n";
  }
  return OS;
}

std::istream &operator>>(std::istream &IS, Vector &V) {
  V.clear();
  fft::IndexT Size;
  IS >> Size;
  V.reserve(Size);

  return IS;
}