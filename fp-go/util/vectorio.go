package util

import (
	"fp-go/fft"
	"log"
	"os"
)

func ImportVec(name string) []complex128 {
	f, err1 := os.OpenFile(name+".vec", os.O_RDONLY, 0666)
	defer f.Close()
	if err1 != nil {
		log.Panic(err1)
	}
	A, err2 := fft.Import(f)
	if err2 != nil {
		log.Panic(err2)
	}
	return A
}

func ExportVec(name string, V []complex128) {
	f, err1 := os.OpenFile(name+".vec", os.O_CREATE|os.O_TRUNC|os.O_WRONLY, 0666)
	defer f.Close()
	if err1 != nil {
		log.Panic(err1)
	}
	err2 := fft.Export(f, V)
	if err2 != nil {
		log.Panic(err2)
	}
}
