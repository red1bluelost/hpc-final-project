package main

import (
	"flag"
	"fmt"
	"fp-go/fft"
	"fp-go/util"
	umath "fp-go/util/math"
	"os"
	"time"
)

var verify = flag.Bool("verify", false,
	"check parallel multiplication against a regular implementation")

func main() {
	flag.Parse()
	start := time.Now()
	X := util.ImportVec("X")
	endImport := time.Now()

	X = fft.FffDitR2(X)
	endMultiply := time.Now()

	util.ExportVec("Xf", X)
	endExport := time.Now()

	durImport := endImport.Sub(start)
	durMultiply := endMultiply.Sub(endImport)
	durExport := endExport.Sub(endMultiply)

	fmt.Printf("-- Timing --\n")
	fmt.Printf("Import: %s\n", durImport)
	fmt.Printf("Multiply: %s\n", durMultiply)
	fmt.Printf("Export: %s\n", durExport)

	if *verify {
		XC := util.ImportVec("X")
		XC = fft.FffDitR2(XC)
		for i, v := range XC {
			if !umath.CEqual(v, X[i], 0x0001) {
				fmt.Fprintf(os.Stderr, "Multiplication failed with unequal matrices\n")
				os.Exit(1)
			}
		}
	}
}
