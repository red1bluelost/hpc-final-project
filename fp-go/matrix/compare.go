package matrix

import (
	"fp-go/util/math"
)

func Equal(A Matrix, B Matrix) bool {
	ra, ca := A.Dim()
	rb, cb := B.Dim()
	if ra != rb || ca != cb {
		return false
	}
	{
		Ad, ok1 := A.(*Dense)
		Bd, ok2 := B.(*Dense)
		if ok1 && ok2 {
			return denseEqual(Ad, Bd)
		}
	}
	panic("unimplemented")
}

func denseEqual(A *Dense, B *Dense) bool {
	const TOLERANCE = 0.0001
	for i, v := range A.data {
		if !math.FEqual(v, B.data[i], TOLERANCE) {
			return false
		}
	}
	return true
}
