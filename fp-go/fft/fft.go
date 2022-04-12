package fft

import (
	umath "fp-go/util/math"
	"math"
	"math/cmplx"
)

func bitrev(x []complex128) {
	N := len(x)
	for n, r := 0, 0; n <= N-2; n++ {
		if n < r {
			x[n], x[r] = x[r], x[n]
		}
		var k int
		for k = N / 2; k <= r; k /= 2 {
			r -= k
		}
		r += k
	}
}

func FffDitR2(x []complex128) {
	n := len(x)
	nu := umath.ILog2(n)
	bitrev(x)
	for m := 1; m <= nu; m++ {
		l := 1 << m
		l2 := l / 2
		for ir := 0; ir < l2; ir++ {
			w := cmplx.Rect(1, -2*math.Pi*float64(ir)/float64(l))
			for it := ir; it < n; it += l {
				ib := it + l2
				t := x[ib] * w
				x[ib] = x[it] - t
				x[it] = x[it] + t
			}
		}
	}
}
