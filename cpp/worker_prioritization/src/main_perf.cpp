#include <benchmark/benchmark.h>
#include <worker_pool.h>

#include <iostream>
#include <memory>
#include <vector>

std::vector<Task> prepareTasks() {
    size_t taskCnt = 100000;
    std::vector<Task> tasks;
    tasks.reserve(taskCnt);
    for (int i = 0; i < taskCnt; i++) {
        auto sleepDuration = std::chrono::milliseconds(rand() % 5 + 5);  // sleep for 5-10ms
        tasks.emplace_back([sleepDuration, i]() {
            volatile int x = 3;
            std::this_thread::sleep_for(sleepDuration);
        });
    }
    return tasks;
}

static void worker(benchmark::State &state) {
    auto tasks = prepareTasks();
    auto worker = Worker();

    size_t iter = 0;
    for (auto _ : state) {
        size_t pos = iter++ % tasks.size();
        worker.assign(Task(tasks[pos]));
    }
    worker.stop();
}

static void workerPoolAssign1(benchmark::State &state) {
    auto tasks = prepareTasks();
    auto pool = WorkerPool(4);

    size_t iter = 0;
    for (auto _ : state) {
        size_t pos = iter++ % tasks.size();
        pool.assign1(Task(tasks[pos]));
    }
    pool.stop();
}

BENCHMARK(worker);
BENCHMARK(workerPoolAssign1);
BENCHMARK_MAIN();
