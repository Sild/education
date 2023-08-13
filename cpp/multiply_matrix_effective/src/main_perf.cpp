#include <iostream>
#include <vector>
#include "helpers.h"
#include <benchmark/benchmark.h>
#include "naive.hpp"
#include "naive_reverse.hpp"
#include "enroll_nested.hpp"

static void naive_bench(benchmark::State &state)
{
    const auto left = genMatrix(/*resizeOnly=*/false);
    const auto right = genMatrix(/*resizeOnly=*/false);
    auto result = genMatrix(/*resizeOnly=*/true);
    for (auto _ : state)
    {
        naive(left, right, result);
        for (auto &line : result)
            std::fill(line.begin(), line.end(), 0);
    }
}

static void naive_reverse_bench(benchmark::State &state)
{
    const auto left = genMatrix(/*resizeOnly=*/false);
    auto right = genMatrix(/*resizeOnly=*/false);
    auto result = genMatrix(/*resizeOnly=*/true);
    reverseMatrix(right);
    for (auto _ : state)
    {
        naive_reverse(left, right, result);
        for (auto &line : result)
            std::fill(line.begin(), line.end(), 0);
    }
}

static void enroll_nested_bench(benchmark::State &state)
{
    const auto left = genMatrix(/*resizeOnly=*/false);
    auto right = genMatrix(/*resizeOnly=*/false);
    auto result = genMatrix(/*resizeOnly=*/true);
    reverseMatrix(right);
    for (auto _ : state)
    {
        enroll_nested(left, right, result);
        for (auto &line : result)
            std::fill(line.begin(), line.end(), 0);
    }
}

BENCHMARK(naive_bench);
BENCHMARK(naive_reverse_bench);
BENCHMARK(enroll_nested_bench);
BENCHMARK_MAIN();
