package main

import (
	"fmt"
	"fp-go/matrix"
	"log"
	"os"
)

func importMat(name string) *matrix.Dense {
	f, err1 := os.OpenFile(name + ".mat", os.O_RDONLY, 0666)
	if err1 != nil {
		log.Panic(err1)
	}
	A, err2 := matrix.ImportDense(f)
	if err2 != nil {
		log.Panic(err2)
	}
	return A
}

func exportMat(name string, M matrix.Matrix) {
	f, err1 := os.OpenFile(name + ".mat", os.O_CREATE | os.O_TRUNC | os.O_WRONLY, 0666)
	if err1 != nil {
		log.Panic(err1)
	}
	err2 := matrix.Export(f, M)
	if err2 != nil {
		log.Panic(err2)
	}
}

func main() {
	A := importMat("A")
	B := importMat("B")

	C := matrix.Multiply(A, B)

	fmt.Printf("C = %+v", C)

	exportMat("C", C)

}
