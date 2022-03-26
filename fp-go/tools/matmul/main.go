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

	fmt.Printf("-- Timing --\n")
	fmt.Printf("Import: %s\n", durImport)
	fmt.Printf("Multiply: %s\n", durMultiply)
	fmt.Printf("Export: %s\n", durExport)

	if *verify && !matrix.Equal(C, matrix.Multiply(A, B)) {
		fmt.Fprintf(os.Stderr, "Multiplication failed with unequal matrices\n")
		os.Exit(1)
	}
}
