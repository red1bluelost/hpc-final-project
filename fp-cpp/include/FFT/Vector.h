#ifndef FP_CPP_FFT_VECTOR_H
#define FP_CPP_FFT_VECTOR_H

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

#endif // FP_CPP_FFT_VECTOR_H
