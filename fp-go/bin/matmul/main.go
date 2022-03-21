package main

import (
	"fmt"
	"fp-go/matrix"
)

func main() {
	A := matrix.NewDenseRaw(3, 3, []float64{
		1, 2, 3,
		4, 5, 6,
		7, 8, 9,
	})

	B := matrix.NewDenseRaw(3, 3, []float64{
		2, 0, 0,
		0, 2, 0,
		0, 0, 2,
	})

	fmt.Printf("C = %+v", matrix.Multiply(A, B))
}
