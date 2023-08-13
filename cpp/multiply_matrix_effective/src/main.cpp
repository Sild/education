#include <iostream>
#include <vector>
#include "helpers.h"
#include "naive.hpp"
#include "naive_reverse.hpp"


int main()
{
    const auto left = genMatrix(/*resizeOnly=*/false);
    auto right = genMatrix(/*resizeOnly=*/false);
    auto result = genMatrix(/*resizeOnly=*/true);
    naive(left, right, result);

    // printMatrix(left);
    printMatrix(right);
    reverseMatrix(right);
    printMatrix(right);
    printMatrix(result);
    return 0;
}