package fft

import (
	"bytes"
	"io"
	"reflect"
	"strings"
	"testing"
)

var testString = `8
(1.000000+1.000000i)
(2.000000+2.000000i)
(3.000000+3.000000i)
(4.000000+4.000000i)
(5.000000+5.000000i)
(6.000000+6.000000i)
(7.000000+7.000000i)
(8.000000+8.000000i)
`

func TestExport(t *testing.T) {
	tests := []struct {
		name    string
		arg     []complex128
		wantWr  string
		wantErr bool
	}{
		{
			name:    "v8",
			arg:     []complex128{1 + 1i, 2 + 2i, 3 + 3i, 4 + 4i, 5 + 5i, 6 + 6i, 7 + 7i, 8 + 8i},
			wantWr:  testString,
			wantErr: false,
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			wr := &bytes.Buffer{}
			err := Export(wr, tt.arg)
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

func TestImport(t *testing.T) {
	tests := []struct {
		name    string
		arg     io.Reader
		want    []complex128
		wantErr bool
	}{
		{
			name:    "v8",
			arg:     strings.NewReader(testString),
			want:    []complex128{1 + 1i, 2 + 2i, 3 + 3i, 4 + 4i, 5 + 5i, 6 + 6i, 7 + 7i, 8 + 8i},
			wantErr: false,
		}}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			got, err := Import(tt.arg)
			if (err != nil) != tt.wantErr {
				t.Errorf("Import() error = %v, wantErr %v", err, tt.wantErr)
				return
			}
			if !reflect.DeepEqual(got, tt.want) {
				t.Errorf("Import() got = %v, want %v", got, tt.want)
			}
		})
	}
}
