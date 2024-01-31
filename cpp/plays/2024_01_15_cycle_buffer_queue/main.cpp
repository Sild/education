#include <iostream>
#include <limits>
#include <vector>

template <typename T, size_t S>
class Queue {
   public:
    Queue() : read_pos(0), write_pos(0), tasks(adjust_size(), nullptr) {}
    bool push(T* task) { return false; }
    T* pop() { return nullptr; }

   private:
    // TODO it doesn't guarantee compile-time calculation
    static constexpr size_t adjust_size() {
        size_t real_size = 1;
        while (real_size <= std::numeric_limits<size_t>::max() / 2 && real_size < S) {
            real_size <<= 1;
        }
        return real_size;
    };
    atomic_size_t read_pos = 0;
    atomic_size_t write_pos = 0;
    std::vector<T*> tasks;
};

int main() {
    auto queue = Queue<int, 10>();
    queue.push(new int(1));
    queue.push(new int(2));
    auto val = queue.pop();
    return 0;
}