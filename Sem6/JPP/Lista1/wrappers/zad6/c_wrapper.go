package main

/*
#cgo LDFLAGS: -L../../C -lmathC
#include <stdint.h>
#include <stdlib.h>

// Deklaracje funkcji z libmathC
extern uint64_t gcd(uint64_t a, uint64_t b);
extern uint64_t smallest_prime_divisor(uint64_t n);
extern uint64_t euler_totient(uint64_t n);

typedef struct {
    int has_solution;
    uint64_t x;
    uint64_t y;
} diophantine_solution_t;

extern diophantine_solution_t solve_linear_diophantine(uint64_t a, uint64_t b, uint64_t c);
*/
import "C"


type DiophantineSolution struct {
    HasSolution bool
    X, Y        uint64
}

func GcdC(a, b uint64) uint64 {
    return uint64(C.gcd(C.uint64_t(a), C.uint64_t(b)))
}

func SmallestPrimeDivisorC(n uint64) uint64 {
    return uint64(C.smallest_prime_divisor(C.uint64_t(n)))
}

func EulerTotientC(n uint64) uint64 {
    return uint64(C.euler_totient(C.uint64_t(n)))
}

func SolveLinearDiophantineC(a, b, c uint64) DiophantineSolution {
    sol := C.solve_linear_diophantine(C.uint64_t(a), C.uint64_t(b), C.uint64_t(c))
    return DiophantineSolution{
        HasSolution: sol.has_solution != 0,
        X:           uint64(sol.x),
        Y:           uint64(sol.y),
    }
}