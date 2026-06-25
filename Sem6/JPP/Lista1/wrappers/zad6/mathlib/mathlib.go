package mathlib
import "C"
type DiophantineSolution struct {
	HasSolution int
	X           uint64
	Y           uint64
}

func GCD(a, b uint64) uint64 {
	if a == 0 {
		return b
	}
	if b == 0 {
		return a
	}

	for b != 0 {
		a, b = b, a%b
	}
	return a
}

func SmallestPrimeDivisor(n uint64) uint64 {
	if n <= 1 {
		return 0
	}
	if n%2 == 0 {
		return 2
	}

	for i := uint64(3); i*i <= n; i += 2 {
		if n%i == 0 {
			return i
		}
	}
	return n
}

func EulerTotient(n uint64) uint64 {
	if n == 0 {
		return 0
	}

	result := n

	if n%2 == 0 {
		for n%2 == 0 {
			n /= 2
		}
		result -= result / 2
	}

	for i := uint64(3); i*i <= n; i += 2 {
		if n%i == 0 {
			for n%i == 0 {
				n /= i
			}
			result -= result / i
		}
	}

	if n > 1 {
		result -= result / n
	}

	return result
}

func extendedGCD(a, b uint64) (int64, int64) {
	if b == 0 {
		return 1, 0
	}

	x1, y1 := extendedGCD(b, a%b)
	x := y1
	y := x1 - int64(a/b)*y1

	return x, y
}

func SolveLinearDiophantine(a, b, c uint64, hasSolution *int, x *uint64, y *uint64) {
    g := GCD(a, b)
    if g == 0 || c%g != 0 {
        *hasSolution = 0
        return
    }

    if b == 0 {
        if c%a == 0 {
            *hasSolution = 1
            *x = uint64(c / a)
            *y = 0
        } else {
            *hasSolution = 0
        }
        return
    }

    x0, y0 := extendedGCD(a, b)

    factor := int64(c / g)
    x0 *= factor
    y0 *= factor

    y0 = -y0

    stepX := int64(b / g)
    stepY := int64(a / g)

    if stepX == 0 || stepY == 0 {
        *hasSolution = 0
        return
    }

    var tMinX, tMinY int64

    if stepX > 0 {
        tMinX = (-x0 + stepX - 1) / stepX
    } else {
        tMinX = (-x0) / stepX
    }

    if stepY > 0 {
        tMinY = (-y0 + stepY - 1) / stepY
    } else {
        tMinY = (-y0) / stepY
    }

    t := tMinX
    if tMinY > t {
        t = tMinY
    }

    x0 += t * stepX
    y0 += t * stepY

    if x0 >= 0 && y0 >= 0 {
        *hasSolution = 1
        *x = uint64(x0)
        *y = uint64(y0)
    } else {
        *hasSolution = 0
    }
}