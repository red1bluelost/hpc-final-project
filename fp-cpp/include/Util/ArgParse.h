#ifndef FP_CPP_UTIL_ARGPARSE_H
#define FP_CPP_UTIL_ARGPARSE_H

#include <algorithm>
#include <string_view>

inline constexpr bool argBool(const int Argc, const char *Argv[], std::string_view Flag) noexcept {
  return Argv + Argc !=
      std::find_if(Argv, Argv + Argc, [Flag](std::string_view SV) { return SV == Flag; });
}

#endif //FP_CPP_UTIL_ARGPARSE_H
