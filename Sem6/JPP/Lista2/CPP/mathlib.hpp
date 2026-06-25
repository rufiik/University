#ifndef MATHLIB_HPP
#define MATHLIB_HPP

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    bool has_solution;
    uint64_t x;
    uint64_t y;
} diophantine_solution_t;
uint64_t gcd(uint64_t a, uint64_t b);
diophantine_solution_t solve_linear_diophantine(uint64_t a, uint64_t b, uint64_t c);
void extended_gcd(uint64_t a, uint64_t b, int64_t *x, int64_t *y);

#endif // MATHLIB_HPP