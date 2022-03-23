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
	if len(os.Args) != 4 {
		fmt.Fprintf(os.Stderr, "missing arguments\n")
		usage()
		return
	}

	var rows, cols int
	if _, err := fmt.Sscanf(os.Args[1], "%d", &rows); err != nil {
		fmt.Fprintf(os.Stderr, "failed to read rows: %e", err)
		usage()
		return
	}
	if _, err := fmt.Sscanf(os.Args[2], "%d", &cols); err != nil {
		fmt.Fprintf(os.Stderr, "failed to read columns: %e", err)
		usage()
		return
	}
	rand.Seed(time.Now().UnixNano())
	name := os.Args[3]
	M := matrix.NewDenseRandom(rows, cols,
		func() float64 { return rand.NormFloat64() * 50 })
	util.ExportMat(name, M)
}
