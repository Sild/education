#pragma once
#include <iostream>
#include <vector>

#include "eiden.hpp"

// const static int MATRIX_SIZE = 48;
const static int MATRIX_SIZE = 64;
using DataType = float;

using matrix = std::vector<std::vector<DataType>>;

matrix genMatrix(bool resizeOnly) {
    matrix data;
    data.resize(MATRIX_SIZE);
    int counter = 0;
    for (auto &line : data) {
        line.resize(MATRIX_SIZE);
        if (resizeOnly) continue;
        for (auto &v : line) {
            v = counter++;
        }
    }
    return data;
}

template <int TSize>
auto genEigenMatrix() {
    auto normal = genMatrix(false);
    Eigen::Matrix<DataType, TSize, TSize> eigen(MATRIX_SIZE, MATRIX_SIZE);
    int i = 0;
    for (auto &l : normal) {
        int j = 0;
        for (auto &e : l) {
            eigen(i, j++) = e;
        }
        ++i;
    }
    return eigen;
}

void printMatrix(const matrix &data) {
    for (auto &line : data) {
        for (auto val : line) {
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