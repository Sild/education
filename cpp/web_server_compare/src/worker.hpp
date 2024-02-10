#pragma once

#include <iostream>
#include <thread>

#include "data.hpp"
#include "tool.hpp"

namespace webtest {
struct Worker {
    Worker(size_t aWorkerID, size_t aCpuNum, TaskQueue& aQueue) : m_WorkerID(aWorkerID), m_Stopping(false) {
        m_Thread = std::thread([this, &aQueue]() { this->loop(aQueue); });
        pinThread(m_Thread, aCpuNum);
    }

    ~Worker() {
        m_Stopping = true;
        m_Thread.join();
    }

   private:
    void loop(TaskQueue& aQueue) {
        while (!m_Stopping) {
            if (auto task = aQueue.pop(); task != nullptr) {
                processTask(task);
            }
        }
        while (!aQueue.empty()) {
            if (auto task = aQueue.pop(); task != nullptr) {
                processTask(task);
            }
        }
    }
    void processTask(Task* aTask) {
        // std::cout << "worker[" << m_WorkerID << "] handling task: " << aTask->m_id << std::endl;
        // std::this_thread::sleep_for(std::chrono::seconds(5));
        aTask->m_result = "result for " + aTask->m_tag;
        // std::cout << "worker[" << m_WorkerID << "] handling finished: " << aTask->m_id << std::endl;

        aTask->m_ready = true;
        sniper::intrusive_ptr<sniper::http::server::Response> rsp;
        rsp->code = sniper::http::ResponseStatus::OK;
        rsp->keep_alive = true;
        rsp->add_header_nocopy("Content-Type: text/plain\r\n");
        rsp->set_data_copy(aTask->m_result);
        // sniper::log_info("handled by worker");
        // resp->add_header_nocopy("Access-Control-Allow-Origin: *\r\n");
        aTask->conn->send(aTask->rsp);
        // sniper::log_info("rsp sent by worker");
    }

   private:
    const std::size_t m_WorkerID = 0;
    std::atomic_bool m_Stopping;
    std::thread m_Thread;
};

}  // namespace webtest