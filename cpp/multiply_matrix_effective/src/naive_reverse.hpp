#pragma once
#include "helpers.h"

// __attribute__((optimize("-ffast-math")))
void naive_reverse(const matrix& left, const matrix& right, matrix& result)
{
    for (int i = 0; i < MATRIX_SIZE; ++i)
    {
        for (int j = 0; j < MATRIX_SIZE; ++j)
        {
            // #pragma clang loop vectorize(assume_safety)
            // #pragma clang loop unroll_count(2)
            for (int k = 0; k < MATRIX_SIZE; ++k)
            {
                result[i][j] += left[i][k] * right[j][k];
            }
        }
    }
}