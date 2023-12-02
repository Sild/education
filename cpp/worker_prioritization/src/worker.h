#include <deque>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>
#include <iostream>

using Task = std::function<void()>;

constexpr static size_t THREADS_COUNT = 4;
class Worker {
   public:
    Worker() {
        m_Threads.reserve(THREADS_COUNT);
        for (size_t i = 0; i < THREADS_COUNT; ++i) {
            m_Threads.emplace_back(std::thread([this, i]() { this->loop(i); }));
        }
    }
    Worker(const Worker &) = delete;
    Worker(Worker &&other) {
        m_Threads.reserve(other.m_Threads.size());
        for (auto &t : other.m_Threads) {
            m_Threads.emplace_back(std::move(t));
        }
        for (int i = 0; i < other.m_TaskPool.size(); ++i) {
            m_TaskPool.emplace_back(std::move(other.m_TaskPool.front()));
            other.m_TaskPool.pop_front();
        }
    };
    ~Worker() = default;

    void stop() {
        m_Stop = true;
        for (auto &th : m_Threads) {
            th.join();
        }
    }

    void assign(Task &&task) {
        if (m_Stop) return;
        auto lock = std::lock_guard(m_TaskMtx);
        m_TaskPool.emplace_back(std::move(task));
    }

    size_t curTaskCount() {
        auto lock = std::lock_guard(m_TaskMtx);
        return m_TaskPool.size();
    }

   private:
    std::deque<Task> m_TaskPool;
    std::vector<std::thread> m_Threads;
    std::mutex m_TaskMtx;
    std::atomic_bool m_Stop = false;

    void loop(int worker_num) {
        while (!m_Stop) {
            Task task;
            {
                auto lock = std::lock_guard(m_TaskMtx);
                if (m_TaskPool.empty()) {
                    // if (m_Stop) return;
                    continue;
                }
                task = std::move(m_TaskPool.front());
                m_TaskPool.pop_front();
            }
            // std::cout << "worker " << worker_num << " executing task" << std::endl;
            task();
        };
    };
};