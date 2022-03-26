package math

import (
	"fmt"
	"testing"
)

func TestILog2(t *testing.T) {
	for i := 0; i < 30; i++ {
		e := 1 << i
		t.Run(fmt.Sprintf("log2(%d)=%d", e, i), func(t *testing.T) {
			if got := ILog2(e); got != i {
				t.Errorf("ILog2() = %v, want %v", e, i)
			}
		})
	}
}
