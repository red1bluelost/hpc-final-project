package main

import (
	"flag"
	"fmt"
	"fp-go/matrix"
	"fp-go/util"
	"os"
	"time"
)

var verify = flag.Bool("verify", false,
	"check parallel multiplication against a regular implementation")

func main() {
	flag.Parse()
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

	fmt.Printf("-- Timing MatMul Go --\n")
	fmt.Printf("Import: %d us\n", durImport.Microseconds())
	fmt.Printf("Multiply: %d us\n", durMultiply.Microseconds())
	fmt.Printf("Export: %d us\n", durExport.Microseconds())

	if *verify && !matrix.Equal(C, matrix.Multiply(A, B)) {
		fmt.Fprintf(os.Stderr, "Multiplication failed with unequal matrices\n")
		os.Exit(1)
	}
}
