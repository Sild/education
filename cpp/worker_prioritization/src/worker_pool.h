#include "worker.h"
#include <format>

class WorkerPool {
   public:
    WorkerPool(size_t aPoolSize) {
        m_WorkerPool.reserve(aPoolSize);
        for (size_t i = 0; i < aPoolSize; ++i) {
            m_WorkerPool.emplace_back();
        }
    }

    void stop() {
        for (auto &worker : m_WorkerPool) worker.stop();
    }

    void assign1(Task &&task) {
        auto pos = m_Counter++ % m_WorkerPool.size();
        // switch (m_Counter % 100) {
        //     case 0:
        //     case 1:
        //     case 2:
        //     case 3:
        //         printWorkerTaskCount();
        // }
        // printWorkerTaskCount();
        m_WorkerPool[pos].assign(std::move(task));
    }

   private:
    std::vector<Worker> m_WorkerPool;
    size_t m_Counter = 0;

    void printWorkerTaskCount() {
        for (size_t i = 0; i < m_WorkerPool.size(); i++) {
            printf("worker=%zd, task_cnt=%zu\n", i, m_WorkerPool[i].curTaskCount());
        }
    }
};