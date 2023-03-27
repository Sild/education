#include <vector>
#include <iostream>
#include <vector>
#include <unordered_set>
#include <cmath>

namespace life2 {

using matrix = std::vector<bool>;

struct Position {
    int row = 0;
    int column = 0;
    auto operator<=>(const Position&) const = default;
};

void print_arena(const matrix& m) {
    auto side_size = int(sqrt(m.size()));
    for (int i = 0; i < side_size; ++i) {
        std::cout << "=";
    }
    for (size_t i = 0; i < m.size(); ++i) {
        if (i % side_size == 0) {
            std::cout << std::endl;
        }
        std::cout <<  (m[i] ? 1 : 0) << "  ";
    }
    for (int i = 0; i < m.size(); ++i) {
        std::cout << "=";
    }
    std::cout << std::endl;
}

matrix init_arena(size_t side_size, const std::unordered_set<Position>& life_positions) {
    matrix arena;
    const auto arena_size = side_size * side_size;
    arena.resize(arena_size);
    std::unordered_set<size_t> life_pos_internal;
    for (const auto& pos: life_positions) {
        life_pos_internal.insert(pos.row * side_size + pos.column);
    }
    for (int i = 0; i < arena_size; ++i) {
        arena[i] = life_pos_internal.contains(i);
    }
    return arena;
}

void iteration(matrix& arena) {
    const int arena_size = int(arena.size());

    std::vector<Position> to_change;

    // for (int i = 0; i < arena_size; ++i) {
        // auto cur = arena[i];
        // size_t life_cnt = 0;


    //     for (int j = 0; j < arena_size; ++j) {
    //         auto cur = arena[i][j];
    //         size_t life_cnt = 0;
    //         for (int p = -1; p <=1; ++p) {
    //             if (i + p < 0 || i + p >= arena_size) {
    //                 continue;
    //             }
    //             for (int k = -1; k <= 1; ++k) {
    //                 if ((p == 0 && k == 0) || j + k < 0 || j + k >= arena_size) {
    //                     continue;
    //                 }
    //                 life_cnt += int(arena[i + p][j + k]);
    //             }
    //         }
    //         if (cur && (life_cnt < 2 || life_cnt > 3)) {
    //             to_change.emplace_back(Position{i, j});
    //         } else if (!cur && life_cnt == 3) {
    //             to_change.emplace_back(Position{i, j});
    //         }
    //     }
    // }
    // for (const auto& pos: to_change) {
    //     arena[pos.row][pos.column] = ~arena[pos.row][pos.column];
    // }
}

}
