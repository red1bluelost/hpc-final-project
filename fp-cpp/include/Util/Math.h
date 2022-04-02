#ifndef FP_CPP_UTIL_MATH_H
#define FP_CPP_UTIL_MATH_H

#include <concepts>
#include <cmath>

namespace util {

template<std::floating_point FloatT>
[[nodiscard]] inline constexpr bool
fequal(FloatT R, FloatT L) noexcept { return std::abs(R - L) <= 0.0001; }

} // namespace util

#endif //FP_CPP_UTIL_MATH_H
