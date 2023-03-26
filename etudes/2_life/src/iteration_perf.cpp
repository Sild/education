#include <benchmark/benchmark.h>

#include "life.h"

auto prepare_arena() {
  std::set<life::Position> life_positions;
  life_positions.insert(life::Position{10, 8});
  life_positions.insert(life::Position{10, 9});
  life_positions.insert(life::Position{10, 10});
  life_positions.insert(life::Position{10, 11});
  life_positions.insert(life::Position{10, 12});
  return init_arena(500, life_positions);
}

static auto copy_fill_arena = prepare_arena();
static void copy_fill(benchmark::State& state) {
  for (auto _ : state) {
    life::iteration(copy_fill_arena);
  }
}
BENCHMARK(copy_fill);

static auto copy_fill2_arena = prepare_arena();
static void copy_fill2(benchmark::State& state) {
  for (auto _ : state) {
      life::iteration(copy_fill2_arena);
  }
}
BENCHMARK(copy_fill2);

BENCHMARK_MAIN();
