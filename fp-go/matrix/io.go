package matrix

import (
	"bufio"
	"fmt"
	"io/ioutil"
	"os"
	"strings"
)

func Export(name string, m Matrix) error {
	sb := strings.Builder{}
	rows, cols := m.Dim()
	if _, err := fmt.Fprintf(&sb, "%d %d\n", rows, cols); err != nil {
		return err
	}
	for r := 0; r < rows; r++ {
		for c := 0; c < cols; c++ {
			_, err := fmt.Fprintf(&sb, "%f ", m.At(r, c))
			if err != nil {
				return err
			}
		}
		_, err := fmt.Fprint(&sb, "\n")
		if err != nil {
			return err
		}
	}
	ns := name + ".mat"
	return ioutil.WriteFile(ns, []byte(sb.String()), 0666)
}

func ImportDense(name string) (*Dense, error) {
	f, err1 := os.Open(name + ".mat")
	if err1 != nil {
		return nil, err1
	}
	_, err2 := f.Seek(0,0)
	if err2 != nil {
		return nil, err2
	}
	fr := bufio.NewScanner(f)
	var rows, cols int
	if !fr.Scan() {
		return nil, fmt.Errorf("missing row and/or column size")
	}
	if _, e := fmt.Sscanf(fr.Text(), "%d %d", &rows, &cols); e != nil {
		return nil, e
	}
	data := make([]float64, rows*cols)
	for r := 0; r < rows; r++ {
		if !fr.Scan() {
			return nil, fmt.Errorf("missing row %d", r)
		}
		ls := bufio.NewScanner(strings.NewReader(fr.Text()))
		ls.Split(bufio.ScanWords)
		for c := 0; c < cols; c++ {
			if !ls.Scan() {
				return nil, fmt.Errorf("missing data at (%d,%d)", r, c)
			}
			_, e := fmt.Sscanf(ls.Text(), "%f", &data[r*cols+c])
			if e != nil {
				return nil, e
			}
		}
	}
	return NewDenseRaw(rows, cols, data), nil
}
