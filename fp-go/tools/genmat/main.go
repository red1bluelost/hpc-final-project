package main

import (
	"fmt"
	"fp-go/matrix"
	"fp-go/util"
	"math/rand"
	"os"
	"time"
)

func usage() {
	fmt.Fprintf(os.Stderr, "genmat <rows> <columns> <name>\n")
}

func main() {
	args := os.Args
	if len(args) != 4 {
		fmt.Fprintf(os.Stderr, "missing arguments\n")
		usage()
		os.Exit(1)
	}

	var rows, cols int
	if _, err := fmt.Sscanf(args[1], "%d", &rows); err != nil {
		fmt.Fprintf(os.Stderr, "failed to read rows: %e", err)
		usage()
		os.Exit(1)
	}
	if _, err := fmt.Sscanf(args[2], "%d", &cols); err != nil {
		fmt.Fprintf(os.Stderr, "failed to read columns: %e", err)
		usage()
		os.Exit(1)
	}
	rand.Seed(time.Now().UnixNano())
	name := args[3]
	M := matrix.NewDenseRandom(rows, cols,
		func() float64 { return rand.NormFloat64() * 50 })
	util.ExportMat(name, M)
}
