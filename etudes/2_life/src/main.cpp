// https://flibusta.club/b/636815/read

#include <iostream>
#include <vector>
#include <set>
#include <unistd.h>

using matrix = std::vector<std::vector<bool>>;
const int ARENA_SIZE = 20;
const size_t ITERATIONS = 50;

struct Position {
    int row = 0;
    int column = 0;
    auto operator<=>(const Position&) const = default;
};

void print_arena(const matrix& m) {
    for (int i = 0; i < m.size(); ++i) {
        std::cout << "=";
    }
    std::cout << std::endl;
    for (const auto& row: m) {
        for (const auto& v: row) {
            std::cout <<  (v ? 1 : 0) << " ";
        }
        std::cout << std::endl;
    }
    for (int i = 0; i < m.size(); ++i) {
        std::cout << "=";
    }
    std::cout << std::endl;
}

matrix init_arena(size_t arena_size, const std::set<Position>& life_positions) {
    matrix arena;

    arena.resize(arena_size);
    for (int i = 0; i < arena_size; ++i) {
        auto& row = arena[i];
        row.resize(arena_size);
        for (int j = 0; j < arena_size; ++j) {
            row[j] = life_positions.contains(Position{i, j});
        }
    }
    return arena;
}

void iteration(matrix& arena) {
    const int arena_size = int(arena.size());
    auto new_arena = arena;

    for (int i = 0; i < arena_size; ++i) {
        for (int j = 0; j < arena_size; ++j) {
            auto cur = arena[i][j];
            size_t life_cnt = 0;
            for (int p = -1; p <=1; ++p) {
                if (i + p < 0 || i + p >= arena_size) {
                    continue;
                }
                for (int k = -1; k <= 1; ++k) {
                    if ((p == 0 && k == 0) || j + k < 0 || j + k >= arena_size) {
                        continue;
                    }
                    life_cnt += int(arena[i + p][j + k]);
                }
            }
            if (cur && (life_cnt < 2 || life_cnt > 3)) {
                new_arena[i][j] = false;
            } else if (!cur && life_cnt == 3) {
                new_arena[i][j] = true;
            } else {
                new_arena[i][j] = cur;
            }
        }
    }
    arena = new_arena;
}

int main() {
    std::set<Position> life_positions;
    life_positions.insert(Position{10, 8});
    life_positions.insert(Position{10, 9});
    life_positions.insert(Position{10, 10});
    life_positions.insert(Position{10, 11});
    life_positions.insert(Position{10, 12});
    auto arena = init_arena(ARENA_SIZE, life_positions);
    print_arena(arena);
    sleep(1);

    for (int i = 0; i < ITERATIONS; ++i) {
        iteration(arena);
        print_arena(arena);
        sleep(1);
    }
}