package main

import (
	"fmt"
	"fp-go/matrix"
	"fp-go/util"
	"time"
)

func main() {
	start := time.Now()
	A := util.ImportMat("A")
	B := util.ImportMat("B")
	endImport := time.Now()

	C := matrix.Multiply(A, B)
	endMultiply := time.Now()

	util.ExportMat("C", C)
	endExport := time.Now()

	durImport := endImport.Sub(start)
	durMultiply := endMultiply.Sub(endImport)
	durExport := endExport.Sub(endMultiply)

	fmt.Printf("-- Timing --\n")
	fmt.Printf("Import: %s\n", durImport)
	fmt.Printf("Multiply: %s\n", durMultiply)
	fmt.Printf("Export: %s\n", durExport)

	//fmt.Printf("C = %+v", C)
}
