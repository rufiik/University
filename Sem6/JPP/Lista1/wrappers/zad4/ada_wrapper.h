#ifndef ADA_WRAPPER_H
#define ADA_WRAPPER_H

#include <stdint.h>

// GCD
uint64_t gcd_ada_wrapper(uint64_t a, uint64_t b);

// Smallest Prime Divisor
uint64_t spd_ada_wrapper(uint64_t n);

// Euler Totient
uint64_t et_ada_wrapper(uint64_t n);

// Diophantine
typedef struct {
    int has_solution;
    uint64_t x;
    uint64_t y;
} DiophantineSolution;

DiophantineSolution solve_diophantine_ada_wrapper(uint64_t a, uint64_t b, uint64_t c);

// Extended GCD
void extended_gcd_ada_wrapper(uint64_t a, uint64_t b, int64_t *x, int64_t *y);

#endif