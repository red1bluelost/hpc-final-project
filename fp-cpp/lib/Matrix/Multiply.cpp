#include "Matrix/Multiply.h"
#include "Util/Math.h"

#include <cassert>
#include <thread>

using namespace matrix;

Dense matrix::multiply(const Dense &A, const Dense &B) {
  auto [ARows, ACols] = A.dim();
  auto [BRows, BCols] = B.dim();
  assert(ACols == BRows && "mismatch of dimensions");

  Dense CMat(ARows, BCols);
  for (IndexT R = 0; R < ARows; ++R) {
    for (IndexT C = 0; C < BCols; ++C) {
      double Sum = 0.0;
      for (IndexT K = 0; K < ACols; ++K)
        Sum += A.at(R, K) * B.at(K, C);
      CMat.at(R, C) = Sum;
    }
  }
  return CMat;
}

Dense matrix::multiplyBlocked(const Dense &A, const Dense &B) {
  auto [ARows, ACols] = A.dim();
  auto [BRows, BCols] = B.dim();
  assert(ACols == BRows && "mismatch of dimensions");

  Dense CMat(ARows, BCols);
  constexpr IndexT BlockSize = 8;
  for (IndexT RO = 0; RO < ARows; RO += BlockSize) {
    for (IndexT CO = 0; CO < BCols; CO += BlockSize) {
      for (IndexT KO = 0; KO < ACols; KO += BlockSize) {
        for (IndexT R = RO; R < std::min(RO + BlockSize, ARows); ++R) {
          for (IndexT C = CO; C < std::min(CO + BlockSize, BCols); ++C) {
            double Sum = KO == 0 ? 0.0 : CMat.at(R, C);
            for (IndexT K = KO; K < std::min(KO + BlockSize, ACols); ++K)
              Sum += A.at(R, K) * B.at(K, C);
            CMat.at(R, C) = Sum;
          }
        }
      }
    }
  }
  return CMat;
}

Dense matrix::multiplyParallel(const Dense &A, const Dense &B) {
  IndexT ARows, ACols, BRows, BCols;
  std::tie(ARows, ACols) = A.dim();
  std::tie(BRows, BCols) = B.dim();
  assert(ACols == BRows && "mismatch of dimensions");

  constexpr IndexT BlockSize = 8;
  IndexT MaxProc = std::thread::hardware_concurrency();
  IndexT WorkWidth =
      util::divRU(util::divRU(ARows, MaxProc), BlockSize) * BlockSize;

  Dense CMat(ARows, BCols);
  std::vector<std::thread> Thrds(MaxProc);

  for (IndexT TID = 0; TID < MaxProc && TID * WorkWidth < ARows; ++TID)
    Thrds[TID] = std::thread(
        [WorkWidth, ARows, ACols, BCols, &A, &B, &CMat](IndexT Start) {
          for (IndexT R = Start; R < std::min(Start + WorkWidth, ARows); ++R) {
            for (IndexT C = 0; C < BCols; ++C) {
              double Sum = 0.0;
              for (IndexT K = 0; K < ACols; ++K)
                Sum += A.at(R, K) * B.at(K, C);
              CMat.at(R, C) = Sum;
            }
          }
        },
        TID * WorkWidth);

  for (auto &Thrd : Thrds)
    Thrd.join();

  return CMat;
}

Dense matrix::multiplyFast(const Dense &A, const Dense &B) {
  IndexT ARows, ACols, BRows, BCols;
  std::tie(ARows, ACols) = A.dim();
  std::tie(BRows, BCols) = B.dim();
  assert(ACols == BRows && "mismatch of dimensions");

  constexpr IndexT BlockSize = 8;
  IndexT MaxProc = std::thread::hardware_concurrency();
  IndexT WorkWidth =
      util::divRU(util::divRU(ARows, MaxProc), BlockSize) * BlockSize;

  Dense CMat(ARows, BCols);
  std::vector<std::thread> Thrds(MaxProc);

  for (IndexT TID = 0; TID < MaxProc && TID * WorkWidth < ARows; ++TID)
    Thrds[TID] = std::thread(
        [WorkWidth, ARows, ACols, BCols, &A, &B, &CMat](IndexT Start) {
          for (IndexT RO = Start; RO < std::min(Start + WorkWidth, ARows);
               RO += BlockSize) {
            for (IndexT CO = 0; CO < BCols; CO += BlockSize) {
              for (IndexT KO = 0; KO < ACols; KO += BlockSize) {
                for (IndexT R = RO; R < std::min(RO + BlockSize, ARows); ++R) {
                  for (IndexT C = CO; C < std::min(CO + BlockSize, BCols);) {
                    if (C + 4 <= std::min(CO + BlockSize, BCols)) {
                      std::array<double, 4> Sum;
                      for (IndexT I = 0; double &S : Sum)
                        S = KO == 0 ? 0.0 : CMat.at(R, C + (I++));
                      for (IndexT K = KO; K < std::min(KO + BlockSize, ACols);
                           ++K)
                        for (IndexT I = 0; double &S : Sum)
                          S += A.at(R, K) * B.at(K, C + (I++));
                      for (IndexT I = 0; double &S : Sum)
                        CMat.at(R, C + (I++)) = S;
                      C += 4;
                    } else {
                      double Sum = KO == 0 ? 0.0 : CMat.at(R, C);
                      for (IndexT K = KO; K < std::min(KO + BlockSize, ACols);
                           ++K)
                        Sum += A.at(R, K) * B.at(K, C);
                      CMat.at(R, C) = Sum;
                      ++C;
                    }
                  }
                }
              }
            }
          }
        },
        TID * WorkWidth);

  for (IndexT TID = 0; TID < MaxProc && TID * WorkWidth < ARows; ++TID)
    Thrds[TID].join();

  return CMat;
}
