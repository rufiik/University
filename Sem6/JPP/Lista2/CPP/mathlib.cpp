#include "mathlib.hpp"
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
