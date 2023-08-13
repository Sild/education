#pragma once
#include "helpers.h"
#include <functional>
#include <algorithm>

void unroll_nested(const matrix& left, const matrix& right, matrix& result)
{
    for (int i = 0; i < MATRIX_SIZE; ++i)
    {
        auto& resLine = result[i];
        for (int j = 0; j < MATRIX_SIZE; ++j)
        {
            const auto& leftLine = left[i];
            const auto& rightLine = right[j];
            auto& res = resLine[j];
            // std::transform(leftLine.begin(), leftLine.end(), rightLine.begin(), resLine.begin(), std::multiplies<uint64_t>());
            for (int k = 0; k < MATRIX_SIZE; k += 5)
            {
                res += leftLine[k] * rightLine[k];
                res += leftLine[k+1] * rightLine[k+1];
                res += leftLine[k+2] * rightLine[k+2];
                res += leftLine[k+3] * rightLine[k+3];
                res += leftLine[k+4] * rightLine[k+4];
            }
        }
    }
}