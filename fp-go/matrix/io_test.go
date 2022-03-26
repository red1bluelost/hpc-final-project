package matrix

import (
	"bytes"
	"io"
	"reflect"
	"strings"
	"testing"
)

var testString = `3 3
1.000000 2.000000 3.000000 
4.000000 5.000000 6.000000 
7.000000 8.000000 9.000000 
`

func TestImportDense(t *testing.T) {
	type args struct {
		r io.Reader
	}
	tests := []struct {
		name    string
		args    args
		want    *Dense
		wantErr bool
	}{
		{
			name: "3x3",
			args: args{
				strings.NewReader(testString),
			},
			want: NewDenseRaw(3, 3,
				[]float64{1, 2, 3, 4, 5, 6, 7, 8, 9}),
			wantErr: false,
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			got, err := ImportDense(tt.args.r)
			if (err != nil) != tt.wantErr {
				t.Errorf("ImportDense() error = %v, wantErr %v", err, tt.wantErr)
				return
			}
			if !reflect.DeepEqual(got, tt.want) {
				t.Errorf("ImportDense() got = %v, want %v", got, tt.want)
			}
		})
	}
}

func TestExport(t *testing.T) {
	type args struct {
		m Matrix
	}
	tests := []struct {
		name    string
		args    args
		wantWr  string
		wantErr bool
	}{
		{
			name: "3x3",
			args: args{
				NewDenseRaw(3, 3,
					[]float64{1, 2, 3, 4, 5, 6, 7, 8, 9}),
			},
			wantWr:  testString,
			wantErr: false,
		}}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			wr := &bytes.Buffer{}
			err := Export(wr, tt.args.m)
			if (err != nil) != tt.wantErr {
				t.Errorf("Export() error = %v, wantErr %v", err, tt.wantErr)
				return
			}
			if gotWr := wr.String(); gotWr != tt.wantWr {
				t.Errorf("Export() gotWr = %v, want %v", gotWr, tt.wantWr)
			}
		})
	}
}
