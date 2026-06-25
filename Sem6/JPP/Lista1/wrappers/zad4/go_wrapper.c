#include "go_wrapper.h"

uint64_t gcd_go_wrapper(uint64_t a, uint64_t b) {
    return GCD(a, b);
}

uint64_t spd_go_wrapper(uint64_t n) {
    return SmallestPrimeDivisor(n);
}

uint64_t et_go_wrapper(uint64_t n) {
    return EulerTotient(n);
}

void solve_diophantine_go_wrapper(uint64_t a, uint64_t b, uint64_t c,
                                 int *hasSolution, uint64_t *x, uint64_t *y) {
    SolveLinearDiophantine(a, b, c, hasSolution, x, y);
}