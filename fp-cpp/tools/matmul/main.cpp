#include "Matrix/Matrix.h"
#include "Matrix/Multiply.h"

#include <fstream>
#include <iostream>

int main(const int Argc, const char *Argv[]) {
  std::ifstream AFile("A.mat"), BFile("B.mat");
  matrix::Dense A, B;
  AFile >> A;
  BFile >> B;

  matrix::Dense C = matrix::multiply(A, B);

  std::ofstream CFile("C.mat");
  CFile << C;

  if (C != matrix::multiply(A, B)) {
    std::cerr << "multiplication failed\n";
    return 1;
  }
  return 0;
}
