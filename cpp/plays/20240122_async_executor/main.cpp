#include <atomic>
#include <functional>
#include <iostream>
#include <queue>
#include <thread>

class Executor {
   public:
    using Func = std::function<void()>;
    Executor() {
        executor = std::thread([this]() {
            while (!stopping) {
                if (counter == 0) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(5));
                    continue;
                }
                auto lock = std::unique_lock(mutex);
                auto task = std::move(tasks.front());
                tasks.pop();
                lock.unlock();
                task();
            }
            while (counter != 0) {
                auto lock = std::unique_lock(mutex);
                auto task = std::move(tasks.front());
                tasks.pop();
                lock.unlock();
                task();
            }
        });
    };
    ~Executor() {
        std::cout << "enter ~Executor\n";
        stopping = true;
        executor.join();
        std::cout << "exit ~Executor\n";
    }

    void execute(Func&& task) {
        auto task_wrapper = [this, task = std::move(task)]() {
            task();
            counter--;
        };
        counter++;
        auto lock = std::unique_lock(mutex);
        tasks.emplace(std::move(task_wrapper));
    }

   private:
    std::atomic_int counter;
    std::queue<Func> tasks;
    std::mutex mutex;
    std::thread executor;
    std::atomic_bool stopping = false;
};

int main() {
    auto executor = Executor();

    for (int i = 0; i < 10; ++i) {
        auto task = [i]() {
            std::cout << "task № " << i << " started\n";
            std::this_thread::sleep_for(std::chrono::seconds(3));
            std::cout << "task № " << i << " done\n";
        };
        executor.execute(std::move(task));
    }

    return 0;
}