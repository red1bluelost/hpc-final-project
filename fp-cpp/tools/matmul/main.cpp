#include "Matrix/Matrix.h"
#include "Matrix/Multiply.h"

#include <fstream>

int main() {
  std::ifstream AFile("A.mat"), BFile("B.mat");
  matrix::Dense A, B;
  AFile >> A;
  BFile >> B;

  matrix::Dense C = matrix::multiply(A, B);

  std::ofstream CFile("C.mat");
  CFile << C;

  return 0;
}
