#ifndef GO_WRAPPER_H
#define GO_WRAPPER_H

#include <stdint.h>

// deklaracje z Go
uint64_t GCD(uint64_t a, uint64_t b);
uint64_t SmallestPrimeDivisor(uint64_t n);
uint64_t EulerTotient(uint64_t n);

void SolveLinearDiophantine(uint64_t a, uint64_t b, uint64_t c,
                           int *hasSolution, uint64_t *x, uint64_t *y);

// wrappery
uint64_t gcd_go_wrapper(uint64_t a, uint64_t b);
uint64_t spd_go_wrapper(uint64_t n);
uint64_t et_go_wrapper(uint64_t n);

void solve_diophantine_go_wrapper(uint64_t a, uint64_t b, uint64_t c,
                                 int *hasSolution, uint64_t *x, uint64_t *y);

#endif