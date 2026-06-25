#ifndef MATHLIB_H
#define MATHLIB_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    bool has_solution;
    uint64_t x;
    uint64_t y;
} diophantine_solution_t;
uint64_t gcd(uint64_t a, uint64_t b);
uint64_t smallest_prime_divisor(uint64_t n);
uint64_t euler_totient(uint64_t n);
diophantine_solution_t solve_linear_diophantine(uint64_t a, uint64_t b, uint64_t c);
void extended_gcd(uint64_t a, uint64_t b, int64_t *x, int64_t *y);

#endif // MATHLIB_H