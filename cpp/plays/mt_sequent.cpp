#include <iostream>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <condition_variable>

int i = 0;

std::mutex mtx0;
std::mutex mtx1;

std::condition_variable cond_var0;
std::condition_variable cond_var1;

void func0() {
    int iter = 0;
    while (iter++ < 10) {
        auto lock = std::unique_lock(mtx0);
        cond_var0.wait(lock, []{return i == 0;});
        std::cout << "func0: " << i++ << std::endl;
        cond_var1.notify_one();

    }
}

void func1() {
    int iter = 0;
    while (iter++ < 10) {
        auto lock = std::unique_lock(mtx1);
        cond_var1.wait(lock, []{return i != 0;});
        std::cout << "func1: " << i-- << std::endl;
        cond_var0.notify_one();
    }
}

int main() {
    auto th1 = std::thread([](){func0();});
    auto th2 = std::thread([](){func1();});

    th1.join();
    th2.join();
    
    std::cout << "hello" << std::endl;

}
