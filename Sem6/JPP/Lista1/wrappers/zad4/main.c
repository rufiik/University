#include <stdio.h>
#include <stdint.h>
#include "ada_wrapper.h"
#include "go_wrapper.h"
#include "mathlib.h"

int main() {
    uint64_t a = 30, b = 18, c = 6, n = 77;

    printf("=== ADA ===\n");
    printf("GCD(%lu, %lu): %lu\n", a, b, gcd_ada_wrapper(a, b));
    printf("Smallest prime divisor of %lu: %lu\n", n, spd_ada_wrapper(n));
    printf("Euler's totient of %lu: %lu\n", n, et_ada_wrapper(n));

    DiophantineSolution d = solve_diophantine_ada_wrapper(a, b, c);
    printf("Diophantine: has_solution=%d x=%lu y=%lu\n", d.has_solution, d.x, d.y);
    printf("\n=== GO ===\n");
    printf("GCD(%lu, %lu): %lu\n", a, b, gcd_go_wrapper(a, b));
    printf("Smallest prime divisor of %lu: %lu\n", n, spd_go_wrapper(n));
    printf("Euler's totient of %lu: %lu\n", n, et_go_wrapper(n));

    int hasSol;
    uint64_t gx, gy;
    solve_diophantine_go_wrapper(a, b, c, &hasSol, &gx, &gy);
    printf("Diophantine: has_solution=%d x=%lu y=%lu\n", hasSol, gx, gy);
    printf("\n=== C ===\n");
    printf("GCD(%lu, %lu): %lu\n", a, b, gcd(a, b));
    printf("Smallest prime divisor of %lu: %lu\n", n, smallest_prime_divisor(n));
    printf("Euler's totient of %lu: %lu\n", n, euler_totient(n));

    diophantine_solution_t cd = solve_linear_diophantine(a, b, c);
    printf("Diophantine: has_solution=%d x=%lu y=%lu\n", cd.has_solution, cd.x, cd.y);

    return 0;
}