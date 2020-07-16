#include <vector>
#include <thread>
#include <iostream>

template<typename T, typename A>
class ConcurrentContainer {
private:
    A data;
    mutable std::mutex locker;
public:
    static void fill_container(ConcurrentContainer &cc, const T& start) {
        T item = start;
        for (int i = 0; i < 10; i++) {
            cc.add(item++);
        }
    }

    static void fill_container_range(ConcurrentContainer &cc, const T& start) {
        T item = start;
        std::vector<T> range;
        for (int i = 0; i < 10; i++) {
            range.push_back(item++);
        }
        cc.add_range(range);
    }

    void add(const T &elem) {
        std::lock_guard<std::mutex> lock(locker);
        data.push_back(elem);
    }

    void add_range(const A &elems) {
        std::lock_guard<std::mutex> lock(locker);
        for (auto &e: elems) {
            data.push_back(e);
        }
    }

    void show() const {
        std::lock_guard<std::mutex> lock(locker);
        for (auto &e: data) {
            std::cout << e << ", ";
        }
        std::cout << std::endl;
    }

};

int main() {
    ConcurrentContainer<int, std::vector<int>> container;

    std::thread fill_t(ConcurrentContainer<int, std::vector<int>>::fill_container, std::ref(container), 10);
    std::thread fill_range_t(ConcurrentContainer<int, std::vector<int>>::fill_container_range, std::ref(container), 100);
    fill_t.join();
    fill_range_t.join();

    container.show();
}