#include "Matrix/Matrix.h"

#include <algorithm>
#include <iterator>
#include <iomanip>

using namespace matrix;

std::ostream &operator<<(std::ostream &OS, const Dense &M) {
  auto[Rows, Cols] = M.dim();
  OS << Rows << ' ' << Cols << '\n';
  for (std::size_t R = 0; R < Rows; ++R) {
    for (std::size_t C = 0; C < Cols; ++C) {
      OS << std::setprecision(8) << M.at(R, C) << ' ';
    }
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