package main

import (
	"fmt"
	"fp-go/util"
	"math/rand"
	"os"
	"time"
)

func usage() {
	fmt.Fprintf(os.Stderr, "genvec <length> <name>\n")
}

func main() {
	args := os.Args
	if len(args) != 3 {
		fmt.Fprintf(os.Stderr, "missing arguments\n")
		usage()
		os.Exit(1)
	}

	var l int
	if _, err := fmt.Sscanf(args[1], "%d", &l); err != nil {
		fmt.Fprintf(os.Stderr, "failed to read length: %e", err)
		usage()
		os.Exit(1)
	}
	rand.Seed(time.Now().UnixNano())
	r := func() float64 { return rand.NormFloat64() * 50 }
	name := args[2]
	v := make([]complex128, l)
	for i := range v {
		v[i] = complex(r(), r())
	}
	util.ExportVec(name, v)
}
