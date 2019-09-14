#include <iostream>
#include "matrix.h"
#include <cassert>

void run() {
    NMatrix::TMatrix<int, 0, 2> matrix;
    for(size_t i = 0; i < 10; ++i) {
        matrix[i][i] = i;
    }
    for(size_t i = 0; i < 10; ++i) {
        matrix[i][9 - i] = (9 - i);
    }
    for(size_t i = 1; i < 9; i++) {
        for(size_t j = 1; j < 9; j++) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "size=" << matrix.size() << "\n";
    for(auto c: matrix) {
        int x;
        int y;
        int v;
        std::tie(x, y, v) = c;
        std::cout << "(" << x << ", " << y << ") = " << v << "\n";
    }
    std::cout << std::endl;

    ((matrix[0][5] = 314) = 0) = 217;
    std::cout << "canonical_eq: " << matrix[0][5] << std::endl;
}

int main() {
    run();
    return 0;
}
