package matrix

import (
	umath "fp-go/util/math"
	"log"
	"runtime"
	"sync"
)

func Multiply(A, B Matrix) *Dense {
	ar, ac := A.Dim()
	br, bc := B.Dim()
	if br != ac {
		log.Panicf("mismatch of dimensions (%d x %d) and (%d x %d)", ar, ac, br, bc)
	}
	cdata := make([]float64, ar*bc)
	for r := 0; r < ar; r++ {
		for c := 0; c < bc; c++ {
			sum := 0.0
			for k := 0; k < ac; k++ {
				sum += A.At(r, k) * B.At(k, c)
			}
			cdata[r*bc+c] = sum
		}
	}
	return NewDenseRaw(ar, bc, cdata)
}

func parallelbody(wg *sync.WaitGroup, start, width int, A, B Matrix, cdata []float64) {
	ar, ac := A.Dim()
	_, bc := B.Dim()
	const blockSize = 8
	for ro := start; ro < umath.MinI(start+width, ar); ro += blockSize {
		for co := 0; co < bc; co += blockSize {
			for ko := 0; ko < ac; ko += blockSize {
				for r := ro; r < umath.MinI(ro+blockSize, ar); r++ {
					for c := co; c < umath.MinI(co+blockSize, bc); c++ {
						var sum float64
						if ko == 0 {
							sum = 0.0
						} else {
							sum = cdata[r*bc+c]
						}
						for k := ko; k < umath.MinI(ko+blockSize, ac); k++ {
							sum += A.At(r, k) * B.At(k, c)
						}
						cdata[r*bc+c] = sum
					}
				}
			}
		}
	}
	wg.Done()
}

func MultiplyParallel(A, B Matrix) *Dense {
	ar, ac := A.Dim()
	br, bc := B.Dim()
	if br != ac {
		log.Panicf("mismatch of dimensions (%d x %d) and (%d x %d)", ar, ac, br, bc)
	}
	cdata := make([]float64, ar*bc)

	maxProc := runtime.GOMAXPROCS(0)
	width := umath.DivRU(ar, maxProc)

	var wg sync.WaitGroup
	wg.Add(maxProc)
	for pnum := 0; pnum < maxProc; pnum++ {
		go parallelbody(&wg, pnum*width, (pnum+1)*width, A, B, cdata)
	}
	wg.Wait()

	return NewDenseRaw(ar, bc, cdata)
}
