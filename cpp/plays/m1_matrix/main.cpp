#include <benchmark/benchmark.h>

void reference_16x16xK(float *A, float *B, float *C, uint64_t K) {
  for (uint32_t m = 0; m < 16; ++m) {
    for (uint32_t n = 0; n < 16; ++n) {
      C[n * 16 + m] = 0;
      for (uint32_t k = 0; k < K; ++k) {
        C[n * 16 + m] += A[k * 16 + m] * B[k * 16 + n];
      }
    }
  }
}

static float a[2] = {1, 2};
static float b[2] = {3, 4};
static float c[4] = {};

static void reference(benchmark::State& state) {
  for (auto _ : state) {
    reference_16x16xK(a, b, c, 2);
  }
}
BENCHMARK(reference);

BENCHMARK_MAIN();
