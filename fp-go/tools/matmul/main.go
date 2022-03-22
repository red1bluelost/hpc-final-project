package main

import (
	"fmt"
	"fp-go/matrix"
	"fp-go/util"
)



func main() {
	A := util.ImportMat("A")
	B := util.ImportMat("B")

	C := matrix.Multiply(A, B)

	fmt.Printf("C = %+v", C)

	util.ExportMat("C", C)
}
