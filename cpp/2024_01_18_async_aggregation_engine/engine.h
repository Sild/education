#include <deque>
#include <iostream>
#include <memory>
#include <queue>
#include <thread>

namespace bookkeeper {

struct Order {
    int timestamp;
    bool ask;
    double price;
    double quantity;
    std::string id;
    std::string source;
    std::string symbol;
};

template <typename T>
class ActionsQueue {
   public:
    bool push(T&& val) {
        auto lock = std::unique_lock(mtx);
        if (data.size() >= MAX_SIZE) {
            return false;
        }
        data.emplace(std::move(val));
        return true;
    }
    std::optional<T> pop() {
        auto lock = std::unique_lock(mtx);
        if (data.empty()) {
            return std::nullopt;
        }
        auto res = data.front();
        data.pop();
        return res;
    }

    bool empty() {
        auto lock = std::unique_lock(mtx);
        return data.empty();
    }

   private:
    std::queue<T> data;
    static const size_t MAX_SIZE = 1024;
    std::mutex mtx;
};

// implementation supposed to receive, parse and convert data from source into Order
class SourceAdaptor {
   public:
    virtual void run_fetch(std::function<void(Order&&)> callback) = 0;
    virtual void submit_order(Order&& order) = 0;
    virtual void stop() = 0;
    virtual ~SourceAdaptor() = default;
};

class BKServer {
   public:
    void add_source(std::unique_ptr<SourceAdaptor> adaptor);
    void run();
    void stop();
    // make public for tests purposes
    void add_order(std::string client_id, Order&& order);

   private:
    void run_server();
    int parse_client_action();
    std::string parse_client_id();
    Order parse_client_order();

    void publish_update(std::string client_id);

    void worker_loop();

    std::vector<std::unique_ptr<SourceAdaptor>> adaptors;
    std::unordered_map<std::string, Order> orders;
    std::mutex orders_mtx;
    std::atomic_bool stopping = false;
    std::vector<std::thread> adaptor_thread;
    std::thread server_thread;
    // it's possible to subscribe only for all update at once for simplicity
    std::deque<Order> updates_queue;
    std::condition_variable update_cv;
    std::mutex updates_queue_mtx;
    std::unordered_map<std::string, decltype(updates_queue)::iterator> subscribers;
    std::mutex subscribers_mtx;
    ActionsQueue<std::function<void()>> actions_queue;
    std::vector<std::thread> action_workers;
};
}  // namespace bookkeeper