#pragma once

#include <atomic>
#include <mutex>
#include <queue>
#include "libsniper_server.hpp"

namespace webtest {

struct Task {
    size_t m_id;
    std::string m_tag;
    std::string m_result;
    std::atomic_bool m_ready;

    // const sniper::intrusive_ptr<sniper::http::server::Connection> conn;
    // const sniper::intrusive_ptr< sniper::http::server::Request> req;
    // const sniper::intrusive_ptr< sniper::http::server::Response> rsp;

    sniper::intrusive_ptr<sniper::http::server::Connection> conn;
    sniper::http::server::Request* req;
    sniper::http::server::Response* rsp;
};

template <typename T, size_t S>
class TSQueue {
   public:
    TSQueue(){};
    bool push(T item) {
        auto lock = std::unique_lock(m_mtx);
        if (m_data.size() == S) return false;
        m_data.emplace(std::move(item));
        return true;
    }
    T pop() {
        auto lock = std::unique_lock(m_mtx);
        if (m_data.empty()) return nullptr;
        auto res = std::move(m_data.front());
        m_data.pop();
        return res;
    }

    bool empty() {
        auto lock = std::unique_lock(m_mtx);
        return m_data.empty();
    }

   private:
    std::queue<T> m_data;
    std::mutex m_mtx;
};

using TaskQueue = TSQueue<Task*, 4096>;

}  // namespace webtest