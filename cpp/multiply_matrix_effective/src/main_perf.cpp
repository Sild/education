#include <benchmark/benchmark.h>

#include <iostream>
#include <vector>

#include "helpers.h"
#include "naive.hpp"
#include "naive_reverse.hpp"
#include "unroll_nested.hpp"
#include "register256_float.hpp"
#include "register512_float.hpp"
// #include <fmt/format.h>

static void registers256_float_bench(benchmark::State &state) {
    const auto left = genMatrix(/*resizeOnly=*/false);
    auto right = genMatrix(/*resizeOnly=*/false);
    auto result = genMatrix(/*resizeOnly=*/true);

    size_t count = 0;
    reverseMatrix(right);
    for (auto _ : state) {
        registers256_float(left, right, result);
        count += result.size();
        for (auto &line : result) {
            std::fill(line.begin(), line.end(), 0);
        }
    }
}

static void registers512_float_bench(benchmark::State &state) {
    const auto left = genMatrix(/*resizeOnly=*/false);
    auto right = genMatrix(/*resizeOnly=*/false);
    auto result = genMatrix(/*resizeOnly=*/true);

    size_t count = 0;
    reverseMatrix(right);
    for (auto _ : state) {
        registers512_float(left, right, result);
        count += result.size();
        for (auto &line : result) {
            std::fill(line.begin(), line.end(), 0);
        }
    }
}

static void naive_bench(benchmark::State &state) {
    const auto left = genMatrix(/*resizeOnly=*/false);
    const auto right = genMatrix(/*resizeOnly=*/false);
    auto result = genMatrix(/*resizeOnly=*/true);
    size_t count = 0;
    for (auto _ : state) {
        naive(left, right, result);
        count += result.size();
        for (auto &line : result) {
            std::fill(line.begin(), line.end(), 0);
        }
    }
}

static void naive_reverse_bench(benchmark::State &state) {
    const auto left = genMatrix(/*resizeOnly=*/false);
    auto right = genMatrix(/*resizeOnly=*/false);
    auto result = genMatrix(/*resizeOnly=*/true);
    size_t count = 0;
    reverseMatrix(right);
    for (auto _ : state) {
        naive_reverse(left, right, result);
        count += result.size();
        for (auto &line : result) {
            std::fill(line.begin(), line.end(), 0);
        }
    }
}

static void unroll_nested_bench(benchmark::State &state) {
    const auto left = genMatrix(/*resizeOnly=*/false);
    auto right = genMatrix(/*resizeOnly=*/false);
    auto result = genMatrix(/*resizeOnly=*/true);
    size_t count = 0;
    reverseMatrix(right);
    for (auto _ : state) {
        // auto result = genMatrix(/*resizeOnly=*/true);
        unroll_nested(left, right, result);
        count += result.size();
        for (auto &line : result) {
            std::fill(line.begin(), line.end(), 0);
        }
    }
}

//  -march=native speed up twice!
static void eiden_static_size_bench(benchmark::State &state) {
    const auto left = genEigenMatrix<MATRIX_SIZE>();
    auto right = genEigenMatrix<MATRIX_SIZE>();

    size_t counter = 0;
    for (auto _ : state) {
        auto res = std::move(left * right);
        counter += size_t(res.any());
    }
}

//  -march=native speed up twice!
static void eiden_dynamic_size_bench(benchmark::State &state) {
    const auto left = genEigenMatrix<Eigen::Dynamic>();
    auto right = genEigenMatrix<Eigen::Dynamic>();
    auto res = std::move(left * right);
    // std::cout << "\nres:\n" << res << std::endl;
    // std::cout << right;
    size_t counter = 0;
    for (auto _ : state) {
        auto res = std::move(left * right);
        counter += size_t(res.any());
    }
    // std::cout << counter << std::endl;
}

BENCHMARK(registers256_float_bench);
BENCHMARK(registers512_float_bench);
BENCHMARK(naive_bench);
BENCHMARK(naive_reverse_bench);
BENCHMARK(unroll_nested_bench);
BENCHMARK(eiden_static_size_bench);
BENCHMARK(eiden_dynamic_size_bench);
BENCHMARK_MAIN();
