#include "mathlib.h"
#include <stdint.h>
#include <stdbool.h>

#include <stdio.h>

uint64_t gcd(uint64_t a, uint64_t b)
{
    if (a == 0)
    {
        return b;
    }
    if (b == 0)
    {
        return a;
    }
    while (b != 0)
    {
        uint64_t temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

uint64_t smallest_prime_divisor(uint64_t n)
{
    if (n <= 1)
    {
        return 0;
    }
    if (n % 2 == 0)
    {
        return 2;
    }
    for (uint64_t i = 3; i * i <= n; i += 2)
    {
        if (n % i == 0)
        {
            return i;
        }
    }
    return n;
}

uint64_t euler_totient(uint64_t n)
{
    if (n == 0)
    {
        return 0;
    }
    uint64_t result = n;

    if (n % 2 == 0)
    {
        while (n % 2 == 0)
        {
            n /= 2;
        }
        result -= result / 2;
    }

    for (uint64_t i = 3; i * i <= n; i += 2)
    {
        if (n % i == 0)
        {
            while (n % i == 0)
            {
                n /= i;
            }
            result -= result / i;
        }
    }

    if (n > 1)
    {
        result -= result / n;
    }

    return result;
}

diophantine_solution_t solve_linear_diophantine(uint64_t a, uint64_t b, uint64_t c)
{
    diophantine_solution_t solution = {false, 0, 0};

    uint64_t g = gcd(a, b);
    if (g == 0 || c % g != 0)
    {
        return solution;
    }

    if (b == 0)
    {
        if (c % a == 0)
        {
            solution.has_solution = true;
            solution.x = c / a;
            solution.y = 0; 
        }
        return solution;
    }

    int64_t x0, y0;
    extended_gcd(a, b, &x0, &y0);

    int64_t factor = c / g;
    x0 *= factor;
    y0 *= factor;

    y0 = -y0;

    int64_t step_x = b / g;
    int64_t step_y = a / g;

    if (step_x == 0 || step_y == 0)
    {
        return solution;
    }

    int64_t t_min_x, t_min_y;

    if (step_x > 0)
        t_min_x = (-x0 + step_x - 1) / step_x;
    else
        t_min_x = (-x0) / step_x;

    if (step_y > 0)
        t_min_y = (-y0 + step_y - 1) / step_y;
    else
        t_min_y = (-y0) / step_y;

    int64_t t = (t_min_x > t_min_y) ? t_min_x : t_min_y;

    x0 += t * step_x;
    y0 += t * step_y;

    if (x0 >= 0 && y0 >= 0)
    {
        solution.has_solution = true;
        solution.x = x0;
        solution.y = y0;
    }

    return solution;
}

void extended_gcd(uint64_t a, uint64_t b, int64_t *x, int64_t *y)
{
    if (b == 0)
    {
        *x = 1;
        *y = 0;
        return;
    }

    int64_t x1, y1;
    extended_gcd(b, a % b, &x1, &y1);

    *x = y1;
    *y = x1 - (a / b) * y1;
}
