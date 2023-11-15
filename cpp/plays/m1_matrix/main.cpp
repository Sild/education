#include <benchmark/benchmark.h>
#include <vector>
#include <concepts>

using namespace std;

vector<int> build_vector(int size, bool reverse) {
  vector<int> res;
  res.resize(size);
  if (reverse) {
      for (int i = 0; i < size; ++i)
        res[i] = i;
  } else {
      for (int i = size - 1; i >= 0; --i)
        res[size - i - 1] = i;
  }
  return res;
}

vector<vector<int>> build_res(int size) {
  vector<vector<int>> res;
  res.resize(size);
  for (auto& r: res) {
    r.resize(size);
  }
  return res;
}

// template <typename T>
// concept IsVec = std::is_same_v<T, std::vector<typename T::value_type>>;

template<typename T>
void reference(const T& a, const T& b, vector<T>& res) {
  for (int i = 0; i < a.size(); ++i) {
    for (int j = 0; j < b.size(); ++j) {
      res[i][j] = a[i] * b[j];
    }
  }
}

static void reference(benchmark::State& state) {
  auto a = build_vector(200, true);
  auto b = build_vector(200, false);
  auto res = build_res(200);
  for (auto _ : state) {
    reference(a, b, res);
  }
}
BENCHMARK(reference);


template<typename T>
void v1(const T& a, const T& b, vector<T>& res) {
  for (int i = 0; i < a.size(); ++i) {
    for (int j = 0; j < b.size(); ++j) {
      res[i][j] = a[i] * b[j];
    }
  }
}

static void v1(benchmark::State& state) {
  auto a = build_vector(200, true);
  auto b = build_vector(200, false);
  auto res = build_res(200);
  for (auto _ : state) {
    v1(a, b, res);
  }
}
BENCHMARK(v1);

BENCHMARK_MAIN();
