package matrix

import (
	"bufio"
	"fmt"
	"io"
	"strings"
)

func Export(w io.Writer, m Matrix) error {
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
	{
		bytes := []byte(sb.String())
		n, err := w.Write(bytes)
		if err != nil {
			return err
		}
		if n != len(bytes) {
			return fmt.Errorf("failed to write the full matrix")
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
