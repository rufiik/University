package main

/*
#cgo LDFLAGS: -L../../ADA -lmathADA
#include <stdint.h>
#include <stdlib.h>

// Deklaracje funkcji z libmathADA
extern uint64_t gcd(uint64_t a, uint64_t b);
extern uint64_t smallest_prime_divisor(uint64_t n);
extern uint64_t euler_totient(uint64_t n);

typedef struct {
    int has_solution;
    uint64_t x;
    uint64_t y;
} ada_diophantine_t;

extern ada_diophantine_t solve_linear_diophantine(uint64_t a, uint64_t b, uint64_t c);
*/
import "C"


func GcdAda(a, b uint64) uint64 {
    return uint64(C.gcd(C.uint64_t(a), C.uint64_t(b)))
}

func SmallestPrimeDivisorAda(n uint64) uint64 {
    return uint64(C.smallest_prime_divisor(C.uint64_t(n)))
}

func EulerTotientAda(n uint64) uint64 {
    return uint64(C.euler_totient(C.uint64_t(n)))
}

func SolveLinearDiophantineAda(a, b, c uint64) DiophantineSolution {
    sol := C.solve_linear_diophantine(C.uint64_t(a), C.uint64_t(b), C.uint64_t(c))
    return DiophantineSolution{
        HasSolution: sol.has_solution != 0,
        X:           uint64(sol.x),
        Y:           uint64(sol.y),
    }
}