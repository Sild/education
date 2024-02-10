#include <httplib.h>

#include <atomic>
#include <chrono>
#include <functional>
#include <iostream>
#include <thread>

#include "data.hpp"
#include "libsniper_server.hpp"
#include "servers.hpp"
#include "worker.hpp"
#include "chrono"

static void stop_signal(ev::dynamic_loop* const loop) {
    fmt::println("Stop signal. Exiting");
    // threads::Stop::get().stop();
    loop->break_loop(ev::ALL);
}

auto print_with_cur_ts(std::string_view data) {
    auto ts = std::chrono::high_resolution_clock::now();
    fmt::println("[{}]: {}", ts, data);
}

class Sig final {
   public:
    explicit Sig(ev::dynamic_loop* loop, int signum, std::function<void(ev::dynamic_loop* const loop)>&& cb = {})
        : _loop(std::move(loop)), _cb(std::move(cb)) {
        _w.set(*_loop);
        _w.set<Sig, &Sig::cb_sig>(this);
        _w.start(signum);
    }
    ~Sig() noexcept { _w.stop(); }

   private:
    void cb_sig(ev::sig& w, [[maybe_unused]] int revents) noexcept {
        if (_cb) _cb(_loop);
    }

    ev::dynamic_loop* _loop;
    ev::sig _w;
    // ev::child
    std::function<void(ev::dynamic_loop* const loop)> _cb;
};

class AsyncWatcher final {
   public:
    explicit AsyncWatcher(ev::dynamic_loop* loop, std::function<void(ev::dynamic_loop* const loop)>&& cb = {})
        : _loop(std::move(loop)), _cb(std::move(cb)) {
        _w.set(*_loop);
        _w.set<AsyncWatcher, &AsyncWatcher::cb_async>(this);
        _w.start();
    }
    void notify() {
        _w.send();
    }
    ~AsyncWatcher() noexcept { _w.stop(); }

   private:
    void cb_async(ev::async& w, [[maybe_unused]] int revents) noexcept {
        print_with_cur_ts("async callback called!");
        // _cb(_loop);
    }

    ev::dynamic_loop* _loop;
    ev::async _w;
    std::function<void(ev::dynamic_loop* const loop)> _cb;
};

struct WorkerLoop {
    WorkerLoop()
        :_loop(ev::dynamic_loop(ev::AUTO)) {
        _async = std::make_shared<AsyncWatcher>(&_loop, [](auto*x) {print_with_cur_ts("worker callback");});
        _thread = std::thread([this]() {
            
            Sig sigint(&_loop, SIGINT, [&](ev::dynamic_loop* const loop) {
                fmt::println("sigint call\n");
                stop_signal(loop);
            });
            Sig sigterm(&_loop, SIGTERM, [&](ev::dynamic_loop* const loop) {
                fmt::println("sigterm call\n");
                // stop_signal(loop);
            });

            _loop.run();
        });

        _thread_2 = std::thread([this]() {
            auto lock = std::unique_lock(this->mtx);
            print_with_cur_ts("wait start");
            this->cond_var.wait(lock, [this](){return this->th2_flag;});
            print_with_cur_ts("wait done");
        });
    }
    void notify() {
        th2_flag = true;
        print_with_cur_ts("notify call");
        _async->notify();
    }

    void notify_2() {
        print_with_cur_ts("notify_2 start");
        cond_var.notify_one();
        print_with_cur_ts("notify_2 done");
    }
    ~WorkerLoop() {
        fmt::println("~WorkerLoop\n");
        _thread.join();
        _thread_2.join();
    }

   private:
    std::thread _thread;
    std::thread _thread_2;
    std::condition_variable cond_var;
    std::mutex mtx;
    bool th2_flag = false;
    std::shared_ptr<AsyncWatcher> _async;
    ev::dynamic_loop _loop;
};



int main() {
    signal(SIGPIPE, SIG_IGN);
    signal(SIGSEGV, sigsegv_handler);
    signal(SIGABRT, sigsegv_handler);

    auto sQueue = webtest::TaskQueue();
    // // auto sServerTh = webtest::runHttpLib(0, 8080, sQueue);
    // // auto sServerTh = webtest::runAsioSimple(0, 8080, sQueue);
    auto sServerTh = webtest::runLibsniper(0, 8080, sQueue);
    // auto sServerTh = webtest::runLibsniperWithQueue(0, 8080, sQueue);
    // // auto sWorker1 = webtest::Worker(0, 2, sQueue);
    // auto sWorker2 = webtest::Worker(1, 4, sQueue);
    sServerTh.join();


    // print_with_cur_ts("ready");
    // auto worker = WorkerLoop();

    // std::this_thread::sleep_for(std::chrono::milliseconds(4000));
    // worker.notify();
    // std::this_thread::sleep_for(std::chrono::milliseconds(200));
    // worker.notify();
    // print_with_cur_ts("all notifications done");
    // std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    // print_with_cur_ts("rest1");
    // print_with_cur_ts("before_notify2");
    // worker.notify_2();
    // print_with_cur_ts("after_notify2");



}
