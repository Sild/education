// https://flibusta.club/b/636815/read

#include <unistd.h>

#include "life.h"

const int ARENA_SIZE = 20;
const size_t ITERATIONS = 50;

using namespace life;

int main() {
    std::unordered_set<Position> life_positions;
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