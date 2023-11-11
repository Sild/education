#include <iostream>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <semaphore>

int i = 0;

std::binary_semaphore sem0{1}, sem1{0};

void func0() {
    int iter = 0;
    while (iter++ < 50) {
        sem0.acquire();
        std::cout << "func0: " << i++ << std::endl;
        sem1.release();

    }
}

void func1() {
    int iter = 0;
    while (iter++ < 50) {
        sem1.acquire();
        std::cout << "func1: " << i-- << std::endl;
        sem0.release();
    }
}

int main() {
    auto th1 = std::thread([](){func0();});
    auto th2 = std::thread([](){func1();});

    th1.join();
    th2.join();
    
    std::cout << "hello" << std::endl;

}
