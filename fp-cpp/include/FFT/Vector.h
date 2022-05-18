#ifndef FP_CPP_FFT_VECTOR_H
#define FP_CPP_FFT_VECTOR_H

#include <fmt/format.h>

#include <complex>
#include <cstddef>
#include <vector>

namespace fft {

using IndexT = std::size_t;
using CmplxT = std::complex<double>;

class Vector : public std::vector<CmplxT> {
  using std::vector<CmplxT>::vector;
};

} // namespace fft

std::ostream &operator<<(std::ostream &OS, const fft::Vector &V);
std::istream &operator>>(std::istream &IS, fft::Vector &V);

template <> struct fmt::formatter<fft::Vector> {
  constexpr auto parse(format_parse_context &Ctx) -> decltype(Ctx.begin()) {
    return Ctx.begin();
  }

  template <typename FormatContext>
  auto format(const fft::Vector &V, FormatContext &Ctx) -> decltype(Ctx.out()) {
    auto &&O = Ctx.out();
    format_to(O, "{}\n", V.size());
    for (const auto &C : V) {
      double R = C.real(), I = C.imag();
      format_to(O, "({:.6f}{:+.6f}i)\n", R, I);
    }
    return format_to(O, "");
  }
};

#endif // FP_CPP_FFT_VECTOR_H
