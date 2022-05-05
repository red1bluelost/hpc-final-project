#include "Matrix/Matrix.h"
#include "Matrix/Multiply.h"
#include "Util/ArgParse.h"

#include <fmt/os.h>

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

  matrix::Dense C = matrix::multiplyFast(A, B);
  auto EndMultiply = std::chrono::high_resolution_clock::now();

  fmt::output_file("C.mat").print("{}", C);
  auto EndExport = std::chrono::high_resolution_clock::now();

  std::cout << "-- Timing MatMul C++ --\n";
  for (const auto &[S, D] :
       {std::make_pair("Import", EndImport - Start),
        std::make_pair("Multiply", EndMultiply - EndImport),
        std::make_pair("Export", EndExport - EndMultiply),
        std::make_pair("Total", EndExport - Start)})
    std::cout
        << S << ": "
        << std::chrono::duration_cast<std::chrono::microseconds>(D).count()
        << " us\n";

  if (Verify && C != matrix::multiply(A, B)) {
    std::cerr << "multiplication failed\n";
    return 1;
  }
  return 0;
}
