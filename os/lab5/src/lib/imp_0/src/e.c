#include <stdio.h>
#include <math.h>
#include "../e.h"

float E(int x) {
    float result = powf(1 + 1.0 / x, x);
    printf("1 реализация x = %d, result = %f\n", x, result);
    return result;
}