#include <vector>
#include <iostream>
#include <vector>
#include <unordered_set>
#include <functional>

namespace life {

using matrix = std::vector<std::vector<bool>>;

struct Position {
    int row = 0;
    int column = 0;
    auto operator<=>(const Position&) const = default;
};
}

namespace std {
  template<>
  struct hash<life::Position>
  {
    std::size_t operator()(const life::Position& k) const
    {
      return ((std::hash<int>()(k.column)
               ^ (std::hash<int>()(k.row) << 1)) >> 1);
    }
  };
}

namespace life {


void print_arena(const matrix& m) {
    for (int i = 0; i < m.size(); ++i) {
        std::cout << "=";
    }
    std::cout << std::endl;
    for (const auto& row: m) {
        for (const auto& v: row) {
            std::cout <<  (v ? 1 : 0) << "  ";
        }
        std::cout << std::endl;
    }
    for (int i = 0; i < m.size(); ++i) {
        std::cout << "=";
    }
    std::cout << std::endl;
}

matrix init_arena(size_t arena_size, const std::unordered_set<Position>& life_positions) {
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
            }
        }
    }
    arena = new_arena;
}

void iteration_inplace(matrix& arena) {
    const int arena_size = int(arena.size());

    std::vector<Position> to_change;

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
                to_change.emplace_back(Position{i, j});
            } else if (!cur && life_cnt == 3) {
                to_change.emplace_back(Position{i, j});
            }
        }
    }
    for (const auto& pos: to_change) {
        arena[pos.row][pos.column] = ~arena[pos.row][pos.column];
    }
}

void iteration_inplace_static(matrix& arena) {
    const int arena_size = int(arena.size());

    static std::vector<Position> to_change;
    to_change.clear();

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
                to_change.emplace_back(Position{i, j});
            } else if (!cur && life_cnt == 3) {
                to_change.emplace_back(Position{i, j});
            }
        }
    }
    for (const auto& pos: to_change) {
        arena[pos.row][pos.column] = ~arena[pos.row][pos.column];
    }
}

void iteration_fast_break(matrix& arena) {
    const int arena_size = int(arena.size());

    static std::vector<Position> to_change;
    to_change.clear();

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
                    if (life_cnt == 3) {
                        break;
                    }
                }
                if (life_cnt == 3) {
                    break;
                }
            }
            if (cur && (life_cnt < 2 || life_cnt > 3)) {
                to_change.emplace_back(Position{i, j});
            } else if (!cur && life_cnt == 3) {
                to_change.emplace_back(Position{i, j});
            }
        }
    }
    for (const auto& pos: to_change) {
        arena[pos.row][pos.column] = ~arena[pos.row][pos.column];
    }
}

}

