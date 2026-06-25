package main

import "C"
import "mathl/mathlib"

// Eksportowanie funkcji GCD
//export GCD
func GCD(a, b uint64) uint64 {
    return mathlib.GCD(a, b)
}

// Eksportowanie funkcji SmallestPrimeDivisor
//export SmallestPrimeDivisor
func SmallestPrimeDivisor(n uint64) uint64 {
    return mathlib.SmallestPrimeDivisor(n)
}

// Eksportowanie funkcji EulerTotient
//export EulerTotient
func EulerTotient(n uint64) uint64 {
    return mathlib.EulerTotient(n)
}

// Eksportowanie funkcji SolveLinearDiophantine
//export SolveLinearDiophantine
func SolveLinearDiophantine(a, b, c uint64, hasSolution *int, x *uint64, y *uint64) {
    mathlib.SolveLinearDiophantine(a, b, c, hasSolution, x, y)
}

func main() {}