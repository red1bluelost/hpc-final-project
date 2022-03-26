package matrix

import "testing"

func TestEqual(t *testing.T) {
	type args struct {
		A Matrix
		B Matrix
	}
	tests := []struct {
		name string
		args args
		want bool
	}{
		{
			name: "3x3 == 3x3",
			args: args{
				A: NewDenseRaw(3, 3, []float64{1, 2, 3, 4, 5, 6, 7, 8, 9}),
				B: NewDenseRaw(3, 3, []float64{1, 2, 3, 4, 5, 6, 7, 8, 9}),
			},
			want: true,
		},
		{
			name: "3x3 != 3x3",
			args: args{
				A: NewDenseRaw(3, 3, []float64{1, 2, 3, 4, 5, 6, 7, 8, 9}),
				B: NewDenseRaw(3, 3, []float64{1, 2, 7, 4, 5, 6, 3, 8, 9}),
			},
			want: false,
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if got := Equal(tt.args.A, tt.args.B); got != tt.want {
				t.Errorf("Equal() = %v, want %v", got, tt.want)
			}
		})
	}
}
