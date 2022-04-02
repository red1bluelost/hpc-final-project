#include "Matrix/Matrix.h"
#include "Util/Math.h"

#include <algorithm>
#include <iterator>
#include <functional>
#include <iomanip>

using namespace matrix;

std::ostream &operator<<(std::ostream &OS, const Dense &M) {
  auto[Rows, Cols] = M.dim();
  OS << Rows << ' ' << Cols << '\n';
  for (IndexT R = 0; R < Rows; ++R) {
    for (IndexT C = 0; C < Cols; ++C)
      OS << std::setprecision(8) << M.at(R, C) << ' ';
    OS << '\n';
  }
  return OS;
}

std::istream &operator>>(std::istream &IS, Dense &M) {
  matrix::IndexT Rows, Cols;
  IS >> Rows >> Cols;
  std::vector<double> V;
  V.reserve(Rows * Cols);
  std::copy_n(std::istream_iterator<double>(IS), Rows * Cols, std::back_inserter(V));
  M = Dense(Rows, Cols, std::move(V));
  return IS;
}

bool Dense::operator==(const Dense &M) const noexcept {
  return Rows == M.Rows
      && Cols == M.Cols
      && std::invoke([&] {
        for (std::size_t I = 0, E = Data.size(); I < E; ++I)
          if (!util::fequal(Data[I], M.Data[I]))
            return false;
        return true;
      });
}
