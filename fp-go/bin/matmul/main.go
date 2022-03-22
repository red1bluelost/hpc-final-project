package main

import (
	"fmt"
	"fp-go/matrix"
	"log"
)

func main() {
	A, err1 := matrix.ImportDense("A")
	if err1 != nil {
		log.Panic(err1)
	}

	B, err2 := matrix.ImportDense("B")
	if err2 != nil {
		log.Panic(err2)
	}

	C := matrix.Multiply(A, B)

	fmt.Printf("C = %+v", C)

	err := matrix.Export("C", C)
	if err != nil {
		panic("failed write")
	}
}
