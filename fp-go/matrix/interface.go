package matrix

type Matrix interface {
	Dim() (r, c int)
	At(r, c int) float64
	Set(r, c int, v float64)
}
