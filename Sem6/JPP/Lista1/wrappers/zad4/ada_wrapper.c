#include "ada_wrapper.h"

// funkcje z Ady (EXPORT)
extern uint64_t gcd(uint64_t a, uint64_t b);
extern uint64_t smallest_prime_divisor(uint64_t n);
extern uint64_t euler_totient(uint64_t n);

// STRUKTURA z Ady
typedef struct {
    int has_solution;  
    uint64_t x;
    uint64_t y;
} ada_diophantine;

extern ada_diophantine solve_linear_diophantine(uint64_t a, uint64_t b, uint64_t c);

extern void extended_gcd(uint64_t a, uint64_t b, int64_t *x, int64_t *y);

// ================= WRAPPERY =================

uint64_t gcd_ada_wrapper(uint64_t a, uint64_t b) {
    return gcd(a, b);
}

uint64_t spd_ada_wrapper(uint64_t n) {
    return smallest_prime_divisor(n);
}

uint64_t et_ada_wrapper(uint64_t n) {
    return euler_totient(n);
}

DiophantineSolution solve_diophantine_ada_wrapper(uint64_t a, uint64_t b, uint64_t c) {
    ada_diophantine res = solve_linear_diophantine(a, b, c);

    DiophantineSolution out;
    out.has_solution = res.has_solution;
    out.x = res.x;
    out.y = res.y;

    return out;
}

void extended_gcd_ada_wrapper(uint64_t a, uint64_t b, int64_t *x, int64_t *y) {
    extended_gcd(a, b, x, y);
}