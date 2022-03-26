package fft

import (
	umath "fp-go/util/math"
	"testing"
)

func TestFffDitR2(t *testing.T) {
	tests := []struct {
		name string
		arg  []complex128
		want []complex128
	}{
		{
			name: "1",
			arg:  []complex128{1, 2, 3, 4},
			want: []complex128{10, -2 + 2i, -2, -2 - 2i},
		},
		{
			name: "2",
			arg:  []complex128{1, 2, 3, 4, 5, 6, 7, 8},
			want: []complex128{
				36, -4 + 9.6569i, -4 + 4i, -4 + 1.6569i,
				-4, -4 - 1.6569i, -4 - 4i, -4 - 9.6569i,
			},
		},
		{
			name: "3",
			arg:  []complex128{1, 1, 1, 1, 0, 0, 0, 0},
			want: []complex128{
				4, 1 - 2.4142i, 0, 1 - 0.4142i,
				0, 1 + 0.4142i, 0, 1 + 2.4142i,
			},
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			got := FffDitR2(tt.arg)
			for i, v := range got {
				if !umath.CEqual(v, tt.want[i], 0.0001) {
					t.Errorf("FffDitR2() = %v, want %v", got, tt.want)
				}
			}
		})
	}
}
