#pragma once
#include "helpers.h"

void naive(const matrix& left, const matrix& right, matrix& result)
{
    for (int i = 0; i < MATRIX_SIZE; ++i)
    {
        for (int j = 0; j < MATRIX_SIZE; ++j)
        {
            for (int k = 0; k < MATRIX_SIZE; ++k)
            {
                result[i][j] += left[i][k] * right[k][j];
            }
        }
    }
}