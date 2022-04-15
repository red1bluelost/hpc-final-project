package matrix

import (
	"fmt"
	"math/rand"
	"reflect"
	"testing"
	"time"
)

type args struct {
	A Matrix
	B Matrix
}

var tests = []struct {
	name string
	args args
	want *Dense
}{
	{
		name: "3x3 * 3x3 Identity",
		args: args{
			A: NewDenseRaw(3, 3, []float64{1, 2, 3, 4, 5, 6, 7, 8, 9}),
			B: NewDenseRaw(3, 3, []float64{1, 0, 0, 0, 1, 0, 0, 0, 1}),
		},
		want: NewDenseRaw(3, 3, []float64{1, 2, 3, 4, 5, 6, 7, 8, 9}),
	},
	{
		name: "3x4 * 4x3",
		args: args{
			A: NewDenseRaw(3, 4,
				[]float64{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}),
			B: NewDenseRaw(4, 3,
				[]float64{2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24}),
		},
		want: NewDenseRaw(3, 3,
			[]float64{140, 160, 180, 316, 368, 420, 492, 576, 660}),
	},
}

func TestMultiply(t *testing.T) {
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if got := Multiply(tt.args.A, tt.args.B); !reflect.DeepEqual(got, tt.want) {
				t.Errorf("Multiply() = %v, want %v", got, tt.want)
			}
		})
	}
}

func TestMultiplyBlocked(t *testing.T) {
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if got := MultiplyBlocked(tt.args.A, tt.args.B); !reflect.DeepEqual(got, tt.want) {
				t.Errorf("MultiplyBlocked() = %v, want %v", got, tt.want)
			}
		})
	}
}

func TestMultiplyParallel(t *testing.T) {
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if got := MultiplyParallel(tt.args.A, tt.args.B); !reflect.DeepEqual(got, tt.want) {
				t.Errorf("MultiplyParallel() = %v, want %v", got, tt.want)
			}
		})
	}
}

func TestMultiplyFast(t *testing.T) {
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if got := MultiplyFast(tt.args.A, tt.args.B); !reflect.DeepEqual(got, tt.want) {
				t.Errorf("MultiplyFast() = %v, want %v", got, tt.want)
			}
		})
	}
}

func TestPropertyMultiplyFast(t *testing.T) {
	rand.Seed(time.Now().Unix())
	for i := 0; i < 100; i++ {
		t.Run(fmt.Sprintf("Test %d", i), func(t *testing.T) {
			m, k, n := rand.Intn(500), rand.Intn(500), rand.Intn(500)
			A, B := NewDenseRandom(m, k, rand.NormFloat64), NewDenseRandom(k, n, rand.NormFloat64)
			Base := MultiplyParallel(A, B)
			Fast := MultiplyFast(A, B)
			if !Equal(Base, Fast) {
				t.Errorf("Mismatch of matrices (%dx%d)*(%dx%d)", m, k, k, n)
			}
		})
	}
}
