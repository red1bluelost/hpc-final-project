#ifndef FP_CPP_MATRIX_MATRIX_H
#define FP_CPP_MATRIX_MATRIX_H

#include <cstddef>
#include <utility>
#include <vector>
#include <ostream>
#include <istream>
#include <cassert>

namespace matrix {

typedef std::size_t IndexT;
typedef std::pair<IndexT, IndexT> DimenionT;

class Dense {
  IndexT Rows{}, Cols{};
  std::vector<double> Data{};

public:
  Dense() = default;
  Dense(IndexT Rows, IndexT Cols) : Rows(Rows), Cols(Cols), Data(Rows * Cols) {}
  Dense(IndexT Rows, IndexT Cols, std::vector<double> &&V) : Rows(Rows), Cols(Cols), Data(V) {
    assert(V.size() == Rows * Cols && "size did not match");
  }
  Dense(const Dense &) = default;
  ~Dense() = default;

  Dense &operator=(const Dense &) = default;
  Dense &operator=(Dense &&M) = default;

  bool operator==(const Dense &M) const noexcept;
  bool operator!=(const Dense &M) const noexcept = default;

  [[nodiscard]] DimenionT dim() const noexcept { return {Rows, Cols}; };
  [[nodiscard]] const double &at(IndexT R, IndexT C) const { return Data[R * Cols + C]; }
  [[nodiscard]] double &at(IndexT R, IndexT C) { return Data[R * Cols + C]; }
};

} // namespace matrix

std::ostream &operator<<(std::ostream &OS, const matrix::Dense &M);
std::istream &operator>>(std::istream &IS, matrix::Dense &M);

#endif //FP_CPP_MATRIX_MATRIX_H
