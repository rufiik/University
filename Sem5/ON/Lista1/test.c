#include <stdio.h>
#include <float.h>
#include <math.h>

int main() {
    // Float32 i Float64
    printf("Float32 (FLT_MAX): %.15e\n", FLT_MAX);
    printf("Float32 (FLT_EPSILON): %.15e\n", FLT_EPSILON);
    printf("Float64 (DBL_MAX): %.15e\n", DBL_MAX);
    printf("Float64 (DBL_EPSILON): %.15e\n", DBL_EPSILON);

    return 0;
}