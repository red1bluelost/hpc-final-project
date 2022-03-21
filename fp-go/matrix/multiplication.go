package matrix

import "log"

func Multiply(A, C Matrix) *Dense {
	ar, ac := A.Dim()
	br, bc := C.Dim()
	if br != ac {
		log.Panicf("mismatch of dimensions (%d x %d) and (%d x %d)", ar, ac, br, bc)
	}
	cdata := make([]float64, ar*bc)
	for r := 0; r < ar; r++ {
		for c := 0; c < bc; c++ {
			sum := 0.0
			for k := 0; k < ac; k++ {
				sum += A.At(r, k) * C.At(k, c)
			}
			cdata[r*bc+c] = sum
		}
	}
	return NewDenseRaw(ar, bc, cdata)
}
