package matrix

import (
	"reflect"
	"testing"
)

func TestDense_At(t *testing.T) {
	type fields struct {
		rows int
		cols int
		data []float64
	}
	base := fields{
		rows: 3,
		cols: 3,
		data: []float64{1, 2, 3, 4, 5, 6, 7, 8, 9},
	}
	type args struct {
		r int
		c int
	}
	tests := []struct {
		name   string
		fields fields
		args   args
		want   float64
	}{
		{
			name:   "At (0,0)",
			fields: base,
			args:   args{0, 0},
			want:   1,
		},
		{
			name:   "At (1,0)",
			fields: base,
			args:   args{1, 0},
			want:   4,
		},
		{
			name:   "At (2,1)",
			fields: base,
			args:   args{2, 1},
			want:   8,
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			d := &Dense{
				rows: tt.fields.rows,
				cols: tt.fields.cols,
				data: tt.fields.data,
			}
			if got := d.At(tt.args.r, tt.args.c); got != tt.want {
				t.Errorf("At() = %v, want %v", got, tt.want)
			}
		})
	}
}

func TestDense_Dim(t *testing.T) {
	type fields struct {
		rows int
		cols int
		data []float64
	}
	tests := []struct {
		name   string
		fields fields
		wantR  int
		wantC  int
	}{
		{
			name: "10x12",
			fields: fields{
				rows: 10,
				cols: 12,
				data: make([]float64, 10*12),
			},
			wantR: 10,
			wantC: 12,
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			d := &Dense{
				rows: tt.fields.rows,
				cols: tt.fields.cols,
				data: tt.fields.data,
			}
			gotR, gotC := d.Dim()
			if gotR != tt.wantR {
				t.Errorf("Dim() gotR = %v, want %v", gotR, tt.wantR)
			}
			if gotC != tt.wantC {
				t.Errorf("Dim() gotC = %v, want %v", gotC, tt.wantC)
			}
		})
	}
}

func TestDense_Set(t *testing.T) {
	type fields struct {
		rows int
		cols int
		data []float64
	}
	base := fields{
		rows: 3,
		cols: 3,
		data: []float64{1, 2, 3, 4, 5, 6, 7, 8, 9},
	}
	type args struct {
		r int
		c int
		v float64
	}
	tests := []struct {
		name   string
		fields fields
		args   args
	}{
		{
			name:   "M.at(0,0) = 10",
			fields: base,
			args:   args{0, 0, 10},
		},
		{
			name:   "M.at(1,2) = 12",
			fields: base,
			args:   args{1, 2, 12},
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			d := &Dense{
				rows: tt.fields.rows,
				cols: tt.fields.cols,
				data: tt.fields.data,
			}
			d.Set(tt.args.r, tt.args.c, tt.args.v)
			if d.At(tt.args.r, tt.args.c) != tt.args.v {
				t.Errorf("Did not properly set at (%d,%d)",
					tt.args.r, tt.args.c)
			}
		})
	}
}

func TestNewDenseEmpty(t *testing.T) {
	type args struct {
		r int
		c int
	}
	tests := []struct {
		name string
		args args
		want *Dense
	}{
		{
			name: "3x3",
			args: args{3, 3},
			want: &Dense{
				rows: 3,
				cols: 3,
				data: make([]float64, 3*3),
			},
		},
		{
			name: "10x12",
			args: args{10, 12},
			want: &Dense{
				rows: 10,
				cols: 12,
				data: make([]float64, 10*12),
			},
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if got := NewDenseEmpty(tt.args.r, tt.args.c); !reflect.DeepEqual(got, tt.want) {
				t.Errorf("NewDenseEmpty() = %v, want %v", got, tt.want)
			}
		})
	}
}

func TestNewDenseRandom(t *testing.T) {
	type args struct {
		r  int
		c  int
		rg func() float64
	}
	tests := []struct {
		name string
		args args
		want *Dense
	}{
		{
			name: "3x3",
			args: args{
				r:  3,
				c:  3,
				rg: func() float64 { return 1.0 },
			},
			want: &Dense{
				rows: 3,
				cols: 3,
				data: []float64{1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
			},
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if got := NewDenseRandom(tt.args.r, tt.args.c, tt.args.rg); !reflect.DeepEqual(got, tt.want) {
				t.Errorf("NewDenseRandom() = %v, want %v", got, tt.want)
			}
		})
	}
}

func TestNewDenseRaw(t *testing.T) {
	type args struct {
		r    int
		c    int
		data []float64
	}
	tests := []struct {
		name string
		args args
		want *Dense
	}{
		{
			name: "3x3",
			args: args{
				r:    3,
				c:    3,
				data: []float64{1, 2, 3, 4, 5, 6, 7, 8, 9},
			},
			want: &Dense{
				rows: 3,
				cols: 3,
				data: []float64{1, 2, 3, 4, 5, 6, 7, 8, 9},
			},
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if got := NewDenseRaw(tt.args.r, tt.args.c, tt.args.data); !reflect.DeepEqual(got, tt.want) {
				t.Errorf("NewDenseRaw() = %v, want %v", got, tt.want)
			}
		})
	}
}
