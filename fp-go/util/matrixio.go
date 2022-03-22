package util

import (
	"fp-go/matrix"
	"log"
	"os"
)

func ImportMat(name string) *matrix.Dense {
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

func ExportMat(name string, M matrix.Matrix) {
	f, err1 := os.OpenFile(name + ".mat", os.O_CREATE | os.O_TRUNC | os.O_WRONLY, 0666)
	if err1 != nil {
		log.Panic(err1)
	}
	err2 := matrix.Export(f, M)
	if err2 != nil {
		log.Panic(err2)
	}
}
