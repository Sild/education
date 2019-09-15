#include "matrix.h"
#include <cassert>
#include <iostream>

int main() {
    NMatrix::TMatrix<int, -1, 2> matrix;
    assert(matrix.size() == 0);

    auto a = matrix[0][0];
    assert(a == -1);
    assert(matrix.size() == 0);

    matrix[13][13] = 3;
    assert(matrix.size() == 1);
    matrix[13][13] = -1;
    assert(matrix.size() == 0);


    matrix[100][100] = 314;
    assert(matrix[100][100] == 314);
    assert(matrix.size() == 1);

    for(auto c: matrix) {
        int x;
        int y;
        int v;
        std::tie(x, y, v) = c;
        assert(x == 100);
        assert(y == 100);
        assert(v == 314);
    }
    matrix[5][5] = 3;
    assert(matrix[5][5] == 3);
    matrix[5][5] = 7;
    assert(matrix[5][5] == 7);

    assert(matrix.size() == 2);

    try {
        matrix[5] = 22;
        assert(false);
    } catch (const std::runtime_error& e) {
    }

    try {
        matrix[5][5][5];
        assert(false);
    } catch (const std::runtime_error& e) {
    }

}