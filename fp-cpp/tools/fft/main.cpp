#include "FFT/FFT.h"
#include "FFT/Vector.h"
#include "Util/ArgParse.h"
#include "Util/Math.h"

#include <cassert>
#include <chrono>
#include <fstream>
#include <iostream>

int main(const int Argc, const char *Argv[]) {
  bool Verify = argBool(Argc, Argv, "-v");

  auto Start = std::chrono::high_resolution_clock::now();

  fft::Vector V;
  {
    std::ifstream IF;
    constexpr std::streamsize BufSize = 1 << 20;
    char Buf[BufSize];
    IF.rdbuf()->pubsetbuf(Buf, BufSize);
    IF.open("X.vec");
    assert(IF && "input file couldn't open");
    IF >> V;
  }
  auto EndImport = std::chrono::high_resolution_clock::now();

  fft::fftDitR2(V);
  auto EndFFT = std::chrono::high_resolution_clock::now();

  {
    std::ofstream OF;
    constexpr std::streamsize BufSize = 1 << 20;
    char Buf[BufSize];
    OF.rdbuf()->pubsetbuf(Buf, BufSize);
    OF.open("Xf.vec");
    OF << V;
  }
  auto EndExport = std::chrono::high_resolution_clock::now();

  {
    auto DurImport = EndImport - Start, DurFFT = EndFFT - EndImport,
         DurExport = EndExport - EndFFT;
    std::cout << "-- Timing FFT C++ --\n";
    using namespace std::chrono;
    for (const auto &[S, D] :
         {std::make_pair("Import", DurImport), std::make_pair("FFT", DurFFT),
          std::make_pair("Export", DurExport)})
      std::cout << S << ": " << duration_cast<microseconds>(D).count()
                << "us\n";
  }

  if (Verify) {
    std::ifstream IF("X.vec");
    assert(IF && "repeat input file couldn't open");
    fft::Vector VC;
    IF.seekg(std::ios_base::beg) >> VC;
    fft::fftDitR2(VC);
    for (std::size_t I = 0, E = VC.size(); I < E; ++I) {
      if (!util::fequal(V[I].real(), VC[I].real()) ||
          !util::fequal(V[I].imag(), VC[I].imag())) {
        std::cerr << "fft failed verification with unequal vectors\n";
        return 1;
      }
    }
  }

  return 0;
}