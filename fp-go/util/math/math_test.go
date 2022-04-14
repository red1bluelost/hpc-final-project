package math

import (
	"fmt"
	"testing"
)

func TestILog2(t *testing.T) {
	for i := 0; i < 30; i++ {
		e := 1 << i
		t.Run(fmt.Sprintf("log2(%d)=%d", e, i), func(t *testing.T) {
			if got := ILog2(e); got != i {
				t.Errorf("ILog2() = %v, want %v", e, i)
			}
		})
	}
}

func TestDivRU(t *testing.T) {
	type args struct {
		n int
		d int
	}
	tests := []struct {
		name string
		args args
		want int
	}{
		{
			name: "10/6 = 2",
			args: args{
				n: 10,
				d: 6,
			},
			want: 2,
		},
		{
			name: "1233/81 = 16",
			args: args{
				n: 1233,
				d: 81,
			},
			want: 16,
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if got := DivRU(tt.args.n, tt.args.d); got != tt.want {
				t.Errorf("DivRU() = %v, want %v", got, tt.want)
			}
		})
	}
}

func TestMinI(t *testing.T) {
	type args struct {
		vals []int
	}
	tests := []struct {
		name string
		args args
		want int
	}{
		{
			name: "min(3,5,10,2)=2",
			args: args{
				vals: []int{3, 5, 10, 2},
			},
			want: 2,
		},
		{
			name: "min(2,0,100,5)=0",
			args: args{
				vals: []int{2, 0, 100, 5},
			},
			want: 0,
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if got := MinI(tt.args.vals...); got != tt.want {
				t.Errorf("MinI() = %v, want %v", got, tt.want)
			}
		})
	}
}
