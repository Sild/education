#include "bookkeeper_server.hpp"

namespace example {
using namespace bookkeeper;

class MockAdapter : public SourceAdaptor {
   public:
    void run_fetch(std::function<void(Order&&)> callback) override {
        size_t counter = 1;
        while (!stopping) {
            callback(Order{1, true, counter++ * 1.0, 100, "1", "mock_source", "mock_symbol"});
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
    }
    void stop() override { stopping = true; }
    void submit_order(Order&& order) override { std::cerr << "submitted order: price=" << order.price << "\n"; }

   private:
    std::atomic_bool stopping = false;
};
}  // namespace example

int main() {
    auto server = bookkeeper::BKServer();
    server.add_source(std::make_unique<example::MockAdapter>());
    server.run();
    server.add_order("test_client", bookkeeper::Order{1, true, 1.0, 100, "1", "test_source", "test_symbol"});
    std::this_thread::sleep_for(std::chrono::seconds(10));
    server.stop();
    return 0;
}