#include <iostream>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <semaphore>

int i = 0;

std::atomic_bool spinlock0{true}, spinlock1{false};

void func0() {
    int iter = 0;
    while (iter++ < 50) {
        while (!spinlock0);
        spinlock0 = false;
        std::cout << "func0: " << i++ << std::endl;
        spinlock1 = true;
    }
}

void func1() {
    int iter = 0;
    while (iter++ < 50) {
        while (!spinlock1);
        spinlock1 = false;
        std::cout << "func1: " << i-- << std::endl;
        spinlock0 = true;
    }
}

int main() {
    auto th1 = std::thread([](){func0();});
    auto th2 = std::thread([](){func1();});

    th1.join();
    th2.join();
    
    std::cout << "hello" << std::endl;

}
