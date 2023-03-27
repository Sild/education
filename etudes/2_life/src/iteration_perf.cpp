#include <benchmark/benchmark.h>

#include "life.h"

auto prepare_arena() {
  std::unordered_set<life::Position> life_positions;
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

static auto inplace_arena = prepare_arena();
static void inplace(benchmark::State& state) {
  for (auto _ : state) {
      life::iteration_inplace(inplace_arena);
  }
}
BENCHMARK(inplace);

static auto inplace_static_arena = prepare_arena();
static void inplace_static(benchmark::State& state) {
  for (auto _ : state) {
      life::iteration_inplace_static(inplace_arena);
  }
}
BENCHMARK(inplace_static);

static auto inplace_fast_break = prepare_arena();
static void fast_break(benchmark::State& state) {
  for (auto _ : state) {
      life::iteration_fast_break(inplace_arena);
  }
}
BENCHMARK(fast_break);

BENCHMARK_MAIN();
