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

func MultiplyBlocked(A, B Matrix) *Dense {
	ar, ac := A.Dim()
	br, bc := B.Dim()
	if br != ac {
		log.Panicf("mismatch of dimensions (%d x %d) and (%d x %d)", ar, ac, br, bc)
	}
	cdata := make([]float64, ar*bc)
	const blockSize = 8
	for ro := 0; ro < ar; ro += blockSize {
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
	return NewDenseRaw(ar, bc, cdata)
}

func MultiplyParallel(A, B Matrix) *Dense {
	ar, ac := A.Dim()
	br, bc := B.Dim()
	if br != ac {
		log.Panicf("mismatch of dimensions (%d x %d) and (%d x %d)", ar, ac, br, bc)
	}
	cdata := make([]float64, ar*bc)

	maxProc := runtime.GOMAXPROCS(0)
	workWidth := umath.DivRU(ar, maxProc)

	var wg sync.WaitGroup
	wg.Add(maxProc)
	for pnum := 0; pnum < maxProc; pnum++ {
		go func(start, width int) {
			for r := start; r < umath.MinI(start+width, ar); r++ {
				for c := 0; c < bc; c++ {
					sum := 0.0
					for k := 0; k < ac; k++ {
						sum += A.At(r, k) * B.At(k, c)
					}
					cdata[r*bc+c] = sum
				}
			}
			wg.Done()
		}(pnum*workWidth, (pnum+1)*workWidth)
	}
	wg.Wait()

	return NewDenseRaw(ar, bc, cdata)
}

func MultiplyFast(A, B Matrix) *Dense {
	ar, ac := A.Dim()
	br, bc := B.Dim()
	if br != ac {
		log.Panicf("mismatch of dimensions (%d x %d) and (%d x %d)", ar, ac, br, bc)
	}
	cdata := make([]float64, ar*bc)

	const blockSize = 8
	maxProc := runtime.GOMAXPROCS(0)
	workWidth := umath.DivRU(umath.DivRU(ar, maxProc), blockSize) * blockSize

	var wg sync.WaitGroup
	for pnum := 0; pnum < maxProc && pnum*workWidth < ar; pnum++ {
		wg.Add(1)
		go func(start, width int) {
			for ro := start; ro < umath.MinI(start+width, ar); ro += blockSize {
				for co := 0; co < bc; co += blockSize {
					for ko := 0; ko < ac; ko += blockSize {
						for r := ro; r < umath.MinI(ro+blockSize, ar); {
							if r+4 <= umath.MinI(ro+blockSize, ar) {
								for c := co; c < umath.MinI(co+blockSize, bc); c++ {
									var sum [4]float64
									if ko != 0 {
										for i := 0; i < 4; i++ {
											sum[i] = cdata[(r+i)*bc+c]
										}
									}
									for k := ko; k < umath.MinI(ko+blockSize, ac); k++ {
										for i := 0; i < 4; i++ {
											sum[i] += A.At(r+i, k) * B.At(k, c)
										}

									}
									for i := 0; i < 4; i++ {
										cdata[(r+i)*bc+c] = sum[i]
									}
								}
								r += 4
							} else {
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
								r++
							}
						}
					}
				}
			}
			wg.Done()
		}(pnum*workWidth, workWidth)
	}
	wg.Wait()

	return NewDenseRaw(ar, bc, cdata)
}
