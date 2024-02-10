#pragma once

#include <iostream>
#include <thread>

namespace webtest {
void pinThread(std::thread& aThread, size_t aCpuNum) {
    // cpu_set_t cpuset;
    // CPU_ZERO(&cpuset);
    // CPU_SET(aCpuNum, &cpuset);
    // if (const auto rc = pthread_setaffinity_np(aThread.native_handle(), sizeof(cpu_set_t), &cpuset); rc != 0) {
    //     std::cerr << "thread pin fail to cpu=" << aCpuNum << ", rc=" << rc << std::endl;
    // }
}
}  // namespace webtest