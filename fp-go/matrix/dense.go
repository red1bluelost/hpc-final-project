package matrix

import (
	"log"
)

type Dense struct {
	rows, cols int
	data       []float64
}

func NewDenseRaw(r, c int, data []float64) *Dense {
	if r*c != len(data) {
		log.Panicf("Dimensions do no match.")
	}
	return &Dense{
		rows: r,
		cols: c,
		data: data,
	}
}

func NewDenseEmpty(r, c int) *Dense {
	return &Dense{
		rows: r,
		cols: c,
		data: make([]float64, r*c),
	}
}

func NewDenseRandom(r, c int, rg func() float64) *Dense {
	d := NewDenseEmpty(r, c)
	for i := range d.data {
		d.data[i] = rg()
	}
	return d
}

func (d *Dense) Raw() []float64 {
	return d.data
}

func (d *Dense) Dim() (r, c int) {
	return d.rows, d.cols
}

func (d *Dense) At(r, c int) float64 {
	return d.data[r*d.cols+c]
}

func (d *Dense) Set(r, c int, v float64) {
	d.data[r*d.cols+c] = v
}
