package math

import (
	"math"
)

func ILog2(i int) int {
	r := 0
	for 1<<r < i {
		r++
	}
	return r
}

func FEqual(x, y, t float64) bool {
	return math.Abs(x-y) <= t
}

func CEqual(x, y complex128, t float64) bool {
	return real(x-y) <= t && imag(x-y) <= t
}

func DivRU(n, d int) int {
	return (n + d - 1) / d
}

func MinI(vals ...int) int {
	min := math.MaxInt
	for _, v := range vals {
		if v < min {
			min = v
		}
	}
	return min
}
