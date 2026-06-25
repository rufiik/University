package main

import (
    "fmt"
    "mathl/mathlib"
)

func main() {
    a := uint64(30)
    b := uint64(18)
    c := uint64(6)
    n := uint64(77)

    // ADA
    fmt.Println("=== ADA ===")
    fmt.Printf("GCD(%d, %d): %d\n", a, b, GcdAda(a, b))
    fmt.Printf("Smallest prime divisor of %d: %d\n", n, SmallestPrimeDivisorAda(n))
    fmt.Printf("Euler's totient of %d: %d\n", n, EulerTotientAda(n))
    solAda := SolveLinearDiophantineAda(a, b, c)
    fmt.Printf("Diophantine: has_solution=%t x=%d y=%d\n", solAda.HasSolution, solAda.X, solAda.Y)
    fmt.Println()
    // GO
    fmt.Println("=== GO ===")
    gcd := mathlib.GCD(a, b)
    fmt.Printf("GCD(%d, %d): %d\n", a, b, gcd)
    spd := mathlib.SmallestPrimeDivisor(n)
    fmt.Printf("Smallest prime divisor of %d: %d\n", n, spd)
    euler := mathlib.EulerTotient(n)
    fmt.Printf("Euler's totient of %d: %d\n", n, euler)
    var hasSolution int
    var x, y uint64
    mathlib.SolveLinearDiophantine(a, b, c, &hasSolution, &x, &y)
    fmt.Printf("Diophantine: has_solution=%t x=%d y=%d\n", hasSolution != 0, x, y)
    fmt.Println()
    // C 
    fmt.Println("=== C ===")
    fmt.Printf("GCD(%d, %d): %d\n", a, b, GcdC(a, b))
    fmt.Printf("Smallest prime divisor of %d: %d\n", n, SmallestPrimeDivisorC(n))
    fmt.Printf("Euler's totient of %d: %d\n", n, EulerTotientC(n))
    solC := SolveLinearDiophantineC(a, b, c)
    fmt.Printf("Diophantine: has_solution=%t x=%d y=%d\n", solC.HasSolution, solC.X, solC.Y)
}