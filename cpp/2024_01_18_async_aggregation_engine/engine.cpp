#include "bookkeeper_server.hpp"

#include <deque>
#include <iostream>
#include <memory>
#include <thread>

namespace bookkeeper {

void BKServer::add_source(std::unique_ptr<SourceAdaptor> adaptor) { adaptors.emplace_back(std::move(adaptor)); }

void BKServer::run() {
    for (size_t i = 0; i < 10; ++i) {
        auto th = std::thread([this]() { this->worker_loop(); });
        action_workers.emplace_back(std::move(th));
    }
    std::cerr << "workers started\n";

    server_thread = std::thread([this]() { this->run_server(); });
    std::cerr << "server started\n";

    auto callback = [this](Order&& order) {
        std::cerr << "got order: price=" << order.price << " from source=" << order.source << "\n";
        {
            auto lock = std::unique_lock(orders_mtx);
            orders[order.id] = order;  // assume different sources maintain different ids format for simplicity
        }
    };

    for (auto& adaptor : adaptors) {
        auto th = std::thread([&adaptor, callback]() { adaptor->run_fetch(callback); });
        adaptor_thread.emplace_back(std::move(th));
    }
    std::cerr << "adaptors started\n";
}

void BKServer::worker_loop() {
    while (!stopping || !actions_queue.empty()) {
        auto res = actions_queue.pop();
        if (res.has_value()) {
            res.value()();  // call action
        } else {
            // wait for data
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
    }
}

void BKServer::stop() {
    // adaptors first
    for (auto& adaptor : adaptors) {
        adaptor->stop();
    }
    for (auto& thread : adaptor_thread) {
        thread.join();
    }
    std::cerr << "adaptors stopped\n";

    // then server
    stopping = true;
    server_thread.join();
    std::cerr << "server stopped\n";

    // after server stopped, wait for workers to cleanup the queue
    for (auto& th : action_workers) {
        th.join();
    }
    std::cerr << "workers stopped\n";
}

void BKServer::run_server() {
    while (!stopping) {
        // TODO check client connection and use corresponding handler
        std::this_thread::sleep_for(std::chrono::seconds(1));
        auto action = parse_client_action();
        auto client_id = parse_client_id();
        switch (action) {
            case 1:
                publish_update(client_id);
                break;
            case 2:
                add_order(client_id, parse_client_order());
            default:
                break;
        }
    }
}

// TODO use enum instead int
int BKServer::parse_client_action() {
    // TODO parse action from client
    return 0;
}
std::string BKServer::parse_client_id() { return "test_id"; }
Order BKServer::parse_client_order() {
    // TODO parse order from client
    return Order{};
}

void BKServer::publish_update(std::string client_id) {
    auto subs_lock = std::unique_lock(subscribers_mtx);
    auto [last_sent_pos, inserted] = subscribers.try_emplace(client_id, std::prev(updates_queue.end()));
    auto lock = std::unique_lock(updates_queue_mtx);
    update_cv.wait(lock, [this, last_sent_pos]() { return std::next(last_sent_pos->second) != updates_queue.end(); });
    last_sent_pos->second++;
    Order update = *(last_sent_pos->second);
    // todo send update to the client

    // TODO check if it's possible to cleanup updates queue (if there are active subscribers at .begin() positions)
}

void BKServer::add_order(std::string client_id, Order&& order) {
    auto action = [this, order = std::move(order)]() mutable {
        if (adaptors.empty()) {
            std::cerr << "no adaptors found\n";
        }
        std::cerr << "worker action was made for order_source=" << order.source << "\n";
        // TODO find proper adaptor for the order
        auto& adaptor = adaptors[0];
        adaptor->submit_order(std::move(order));
    };

    if (!actions_queue.push(std::move(action))) {
        std::cerr << "queue is full, fail to add an action\n";
    };
}

}  // namespace bookkeeper