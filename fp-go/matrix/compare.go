package matrix

import (
	"math"
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
	const TOLERANCE = 0.00001
	for i, v := range A.data {
		if d := math.Abs(v - B.data[i]); d > TOLERANCE {
			return false
		}
	}
	return true
}
