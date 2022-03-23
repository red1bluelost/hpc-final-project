package matrix

import (
	"bufio"
	"fmt"
	"io"
	"strings"
)

func Export(wr io.Writer, m Matrix) error {
	w := bufio.NewWriter(wr) // This adds a massive speed up to output
	rows, cols := m.Dim()
	if _, err := fmt.Fprintf(w, "%d %d\n", rows, cols); err != nil {
		return err
	}
	for r := 0; r < rows; r++ {
		for c := 0; c < cols; c++ {
			_, err := fmt.Fprintf(w, "%f ", m.At(r, c))
			if err != nil {
				return err
			}
		}
		_, err := w.Write([]byte("\n"))
		if err != nil {
			return err
		}
	}
	return nil
}

func ImportDense(r io.Reader) (*Dense, error) {
	fr := bufio.NewScanner(r)
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
