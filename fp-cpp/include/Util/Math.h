#ifndef FP_CPP_UTIL_MATH_H
#define FP_CPP_UTIL_MATH_H

#include <cmath>
#include <concepts>

namespace util {

template <std::floating_point FloatT>
[[nodiscard]] inline constexpr bool fequal(FloatT R, FloatT L) noexcept {
  return std::abs(R - L) <= 0.0001;
}

template <std::integral IntT>
[[nodiscard]] inline constexpr IntT log2(IntT I) noexcept {
  IntT R = 0;
  while (1 << R < I)
    R++;
  return R;
}

} // namespace util

#endif // FP_CPP_UTIL_MATH_H
