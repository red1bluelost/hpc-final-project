package main

import (
	"fmt"
	"fp-go/fft"
)

func main() {
	x := []complex128{1, 2, 3, 4, 5, 6, 7, 8}
	fft.FffDitR2(x)
	fmt.Printf("fft = %v\n", x)
}
