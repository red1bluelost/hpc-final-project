package fft

import (
	"bufio"
	"fmt"
	"io"
	"strconv"
)

func Export(wr io.Writer, v []complex128) error {
	w := bufio.NewWriter(wr)
	defer w.Flush()
	l := len(v)
	if _, err := fmt.Fprintf(w, "%d\n", l); err != nil {
		return err
	}
	for _, c := range v {
		if _, err := fmt.Fprintf(w, "%f\n", c); err != nil {
			return err
		}
	}
	return nil
}

func Import(rd io.Reader) ([]complex128, error) {
	fr := bufio.NewScanner(rd)
	var l int
	var err error
	if !fr.Scan() {
		return nil, fmt.Errorf("missing vector length")
	}
	if l, err = strconv.Atoi(fr.Text()); err != nil {
		return nil, err
	}
	v := make([]complex128, l)
	for i := 0; i < l; i++ {
		if !fr.Scan() {
			return nil, fmt.Errorf("missing data at (%d)", i)
		}
		v[i], err = strconv.ParseComplex(fr.Text(), 64)
		if err != nil {
			return nil, err
		}
	}
	return v, nil
}
