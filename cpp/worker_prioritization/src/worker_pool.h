#include <format>
#include <memory>

#include "worker.h"

class WorkerPool {
   public:
    WorkerPool(size_t aPoolSize) {
        m_WorkerPool.reserve(aPoolSize);
        // m_WorkerTaskCounter.reserve(aPoolSize);
        for (size_t i = 0; i < aPoolSize; ++i) {
            m_WorkerPool.emplace_back();
            m_WorkerTaskCounter.emplace_back(std::make_unique<std::atomic_size_t>(size_t(0)));
        }
    }

    void stop() {
        for (auto &worker : m_WorkerPool) worker.stop();
    }

    void assign1(Task &&task) {
        auto pos = m_Counter++ % m_WorkerPool.size();
        m_WorkerPool[pos].assign(std::move(task));
    }

    void assign2(Task &&task) {
        auto pos = getLazyWorkerPos();
        m_WorkerTaskCounter[pos]->operator++();
        Task wrapTask = [this, task = std::move(task), pos]() {
            task();
            m_WorkerTaskCounter[pos]->operator--();
        };
        m_WorkerPool[pos].assign(std::move(task));
    }

   private:
    struct WorkerTaskInfo {
        size_t m_WorkerNum = 0;
        size_t m_TaskCnt = 0;
    };
    std::vector<Worker> m_WorkerPool;
    std::vector<std::unique_ptr<std::atomic_size_t>> m_WorkerTaskCounter;
    size_t m_Counter = 0;

    void printWorkerTaskCount() {
        for (size_t i = 0; i < m_WorkerPool.size(); i++) {
            printf("worker=%zd, task_cnt=%zu\n", i, m_WorkerPool[i].curTaskCount());
        }
    }

    size_t getLazyWorkerPos() { return m_Counter++ % m_WorkerPool.size(); }
};