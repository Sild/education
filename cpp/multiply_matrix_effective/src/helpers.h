#pragma once
#include <vector>
#include <iostream>
const static int MATRIX_SIZE = 150;

using matrix = std::vector<std::vector<uint64_t>>;

matrix genMatrix(bool resizeOnly)
{
    matrix data;
    data.resize(MATRIX_SIZE);
    int counter = 0;
    for (auto &line : data)
    {
        line.resize(MATRIX_SIZE);
        if (resizeOnly)
            continue;
        for (auto &v : line)
        {
            v = counter++;
        }
    }
    return data;
}

void printMatrix(const matrix &data)
{
    for (auto &line : data)
    {
        for (auto val : line)
        {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void reverseMatrix(matrix &data) {
    for (int i = 0; i < data.size(); ++i) {
        for (int j = 0; j < i; ++j) {
            std::swap(data[i][j], data[j][i]);
        }
    }
}