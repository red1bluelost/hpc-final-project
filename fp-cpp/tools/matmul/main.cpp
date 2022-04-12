#include "Matrix/Matrix.h"
#include "Matrix/Multiply.h"
#include "Util/ArgParse.h"

#include <chrono>
#include <fstream>
#include <iostream>
#include <string_view>
#include <thread>

int main(const int Argc, const char *Argv[]) {
  std::ios_base::sync_with_stdio(false);
  bool Verify = argBool(Argc, Argv, "-v");

  auto Start = std::chrono::high_resolution_clock::now();
  matrix::Dense A, B;

  {
    auto FileThreadFunc = [](auto FName, matrix::Dense &Mat) {
      std::ifstream F;
      constexpr std::streamsize BufSize = 1 << 20;
      char Buf[BufSize];
      F.rdbuf()->pubsetbuf(Buf, BufSize);
      F.open(FName);
      assert(F && "input file couldn't open");
      F >> Mat;
    };
    std::thread AThread(FileThreadFunc, "A.mat", std::ref(A));
    std::thread BThread(FileThreadFunc, "B.mat", std::ref(B));
    AThread.join(), BThread.join();
  }
  auto EndImport = std::chrono::high_resolution_clock::now();

  matrix::Dense C = matrix::multiply(A, B);
  auto EndMultiply = std::chrono::high_resolution_clock::now();

  {
    std::ofstream CFile;
    constexpr std::streamsize BufSize = 1 << 20;
    char Buf[BufSize];
    CFile.rdbuf()->pubsetbuf(Buf, BufSize);
    CFile.open("C.mat");
    CFile << C;
  }
  auto EndExport = std::chrono::high_resolution_clock::now();

  {
    auto DurImport = EndImport - Start;
    auto DurMultiply = EndMultiply - EndImport;
    auto DurExport = EndExport - EndMultiply;
    std::cout << "-- Timing MatMul C++ --\n";
    using namespace std::chrono;
    for (const auto &[S, D] : {std::make_pair("Import", DurImport),
                               std::make_pair("Multiply", DurMultiply),
                               std::make_pair("Export", DurExport)})
      std::cout << S << ": " << duration_cast<microseconds>(D).count()
                << "us\n";
  }

  if (Verify && C != matrix::multiply(A, B)) {
    std::cerr << "multiplication failed\n";
    return 1;
  }
  return 0;
}
