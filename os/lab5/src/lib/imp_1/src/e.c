#include <stdio.h>
#include "../e.h"

int factorial(int x) {
    int result = 1;
    for (int i = 2; i <= x; ++i) {
        result *= i;
    }
    return result;
}

float E(int x) {
    float result = 0;
    for (int n = 0; n <= x; ++n) {
        result += 1.0 / factorial(n);
    }
    printf("2 реализация x = %d, result = %f\n", x, result);
    return result;
}