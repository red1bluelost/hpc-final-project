package matrix

import (
	"bufio"
	"fmt"
	"io"
	"strconv"
)

func Export(wr io.Writer, m Matrix) error {
	w := bufio.NewWriter(wr) // This adds a massive speed up to output
	defer w.Flush()
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

func ImportDense(rd io.Reader) (*Dense, error) {
	fr := bufio.NewScanner(rd)
	fr.Split(bufio.ScanWords)
	var rows, cols int
	var err error
	if !fr.Scan() {
		return nil, fmt.Errorf("missing row size")
	}
	if rows, err = strconv.Atoi(fr.Text()); err != nil {
		return nil, err
	}
	if !fr.Scan() {
		return nil, fmt.Errorf("missing column size")
	}
	if cols, err = strconv.Atoi(fr.Text()); err != nil {
		return nil, err
	}
	data := make([]float64, rows*cols)
	for r := 0; r < rows; r++ {
		for c := 0; c < cols; c++ {
			if !fr.Scan() {
				return nil, fmt.Errorf("missing data at (%d,%d)", r, c)
			}
			data[r*cols+c], err = strconv.ParseFloat(fr.Text(), 64)
			if err != nil {
				return nil, err
			}
		}
	}
	return NewDenseRaw(rows, cols, data), nil
}
