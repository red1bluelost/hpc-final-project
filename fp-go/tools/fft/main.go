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

	fft.FffDitR2(X)
	endFFT := time.Now()

	util.ExportVec("Xf", X)
	endExport := time.Now()

	durImport := endImport.Sub(start)
	durFFT := endFFT.Sub(endImport)
	durExport := endExport.Sub(endFFT)
	durTotal := endExport.Sub(start)

	fmt.Printf("-- Timing FFT Go --\n")
	fmt.Printf("Import: %d us\n", durImport.Microseconds())
	fmt.Printf("FFT: %d us\n", durFFT.Microseconds())
	fmt.Printf("Export: %d us\n", durExport.Microseconds())
	fmt.Printf("Total: %d us\n", durTotal.Microseconds())

	if *verify {
		XC := util.ImportVec("X")
		fft.FffDitR2(XC)
		for i, v := range XC {
			if !umath.CEqual(v, X[i], 0x0001) {
				fmt.Fprintf(os.Stderr, "fft failed verification with unequal vectors\n")
				os.Exit(1)
			}
		}
	}
}
