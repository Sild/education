#pragma once

#include <gperftools/malloc_extension.h>

#include <functional>

#include "asio_fast/fast.hpp"
#include "asio_simple.hpp"
#include "data.hpp"
#include "httplib.h"
#include "libsniper/sniper/event/Sig.h"
#include "libsniper/sniper/event/Timer.h"
#include "libsniper_server.hpp"
#include "tool.hpp"

static void sigsegv_handler(int sig) {
    fmt::println("sigsegv_handler {} ({})", strsignal(sig), sig);
    exit(1);
}

namespace webtest {

// all cores 300 rps???
std::thread runHttpLib(size_t aCpuNum, int aPort, TaskQueue& aQueue) {
    auto sThread = std::thread([&aQueue, aPort]() {
        // std::atomic_size_t sTaskCounter = 0;
        httplib::Server svr;
        const std::string res = "111";
        svr.Get("/", [&](const httplib::Request& req, httplib::Response& rsp) {
            auto val = req.get_param_value("key");
            // std::cout << val << std::endl;
            // auto sTaskId = sTaskCounter++;
            // auto sTask = Task{.m_id = sTaskId, .m_tag = std::move(val), .m_ready = false};

            // if (!aQueue.push(&sTask)) {
            //     rsp.status = 400;
            //     rsp.set_content("too many requests", "text/plain");
            //     return;
            // }
            // while (!sTask.m_ready.load()) {
            // }
            // rsp.set_content(sTask.m_result, "text/plain");
            rsp.set_content(val, "text/plain");
        });
        std::cerr << "start listening on " << 8080 << std::endl;
        svr.listen("0.0.0.0", aPort);
    });

    pinThread(sThread, aCpuNum);
    return sThread;
}

// 20k rps
std::thread runAsioSimple(size_t aCpuNum, int aPort, TaskQueue& aQueue) {
    auto sThread = std::thread([&aQueue, aPort]() {
        try {
            // Check command line arguments.
            auto const address = net::ip::make_address("0.0.0.0");
            unsigned short port = static_cast<unsigned short>(8080);

            net::io_context ioc{1};

            tcp::acceptor acceptor{ioc, {address, port}};
            tcp::socket socket{ioc};
            http_server(acceptor, socket);
            std::cerr << "start listening on " << 8080 << std::endl;
            ioc.run();
        } catch (std::exception const& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    });

    pinThread(sThread, aCpuNum);
    return sThread;
}

// 33k rps
std::thread runAsioFast(size_t aCpuNum, int aPort, TaskQueue& aQueue) {
    auto sThread = std::thread([&aQueue, aPort]() {
        try {
            bool spin = false;
            net::io_context ioc{1};
            tcp::acceptor acceptor{ioc, {net::ip::make_address("0.0.0.0"), 8080}};
            int num_workers = 80;

            std::list<http_worker> workers;
            for (int i = 0; i < num_workers; ++i) {
                workers.emplace_back(acceptor, ".");
                workers.back().start();
            }

            std::cerr << "start listening on " << 8080 << std::endl;
            if (spin)
                for (;;) ioc.poll();
            else
                ioc.run();
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    });

    pinThread(sThread, aCpuNum);
    return sThread;
}

// 146k rps, 13ms latency
std::thread runLibsniper(size_t aCpuNum, int aPort, TaskQueue& aQueue) {
    using namespace sniper;
    auto sThread = std::thread([&aQueue, aPort]() {
        log_info("hello_server: ({}, {} UTC)", __DATE__, __TIME__);

        auto loop = event::make_loop();
        if (!loop) {
            log_err("Main: cannot init event loop");
            return;
        }

        signal(SIGPIPE, SIG_IGN);
        // signal(SIGSEGV, sigsegv_handler);
        // signal(SIGABRT, sigsegv_handler);

        event::Sig sig_int(loop, SIGINT, stop_signal);
        event::Sig sig_term(loop, SIGTERM, stop_signal);

        log_info("hello_server starting...");

        Server server([&](const auto& conn, const auto& req, const auto& resp) {
            resp->code = sniper::http::ResponseStatus::OK;
            resp->keep_alive = true;
            std::string result = std::string(8192, 'c');
            // resp->set_data_copy("hello");
            resp->set_data_copy(result);
            // log_info("handled by worker: {}", thread_num);
            // resp->add_header_nocopy("Access-Control-Allow-Origin: *\r\n");
            conn->send(resp);
        });

        event::TimerRepeat timer_tcmalloc(loop, 10s, [] { MallocExtension::instance()->ReleaseFreeMemory(); });

        log_info("hello_server started");
        loop->run();
        log_info("hello_server stopped");
    });

    pinThread(sThread, aCpuNum);
    return sThread;
}

std::thread runLibsniperWithQueue(size_t aCpuNum, int aPort, TaskQueue& aQueue) {
    using namespace sniper;
    auto sThread = std::thread([&aQueue, aPort]() {
        log_info("hello_server: ({}, {} UTC)", __DATE__, __TIME__);

        auto loop = event::make_loop();
        if (!loop) {
            log_err("Main: cannot init event loop");
            return;
        }

        signal(SIGPIPE, SIG_IGN);
        // signal(SIGSEGV, sigsegv_handler);
        // signal(SIGABRT, sigsegv_handler);

        event::Sig sig_int(loop, SIGINT, stop_signal);
        event::Sig sig_term(loop, SIGTERM, stop_signal);

        log_info("hello_server starting...");
        size_t sTaskCounter = 0;
        Server server([&](const intrusive_ptr<sniper::http::server::Connection>& conn, const intrusive_ptr< sniper::http::server::Request>& req, const intrusive_ptr< sniper::http::server::Response>& rsp) {
            const auto& params = req->params();
            
            const auto& tag = params.empty() ? "" : params[0].second;
            // log_info("tag={}", tag);
            auto sTaskId = sTaskCounter++;
            auto sTask = Task{.m_id = sTaskId, .m_tag = std::string(tag), .m_ready = false, .conn = conn};
            // auto sTask = Task{.m_id = sTaskId, .m_tag = std::string(tag), .m_ready = false};

            if (!aQueue.push(&sTask)) {
                rsp->code  = sniper::http::ResponseStatus::GATEWAY_TIMEOUT;
                rsp->keep_alive = true;
                rsp->set_data_nocopy("too many requests");
                rsp->add_header_nocopy("Content-Type: text/plain\r\n");
                conn->send(rsp);
                return;
            }
            // while (!sTask.m_ready) {
            // }
            // log_info("task ready");

            // rsp->code = sniper::http::ResponseStatus::OK;
            // rsp->keep_alive = true;
            // rsp->add_header_nocopy("Content-Type: text/plain\r\n");
            // // rsp->set_data_copy(sTask.m_result);
            // rsp->set_data_copy("rsp");
            // // log_info("handled by worker: {}", thread_num);
            // // resp->add_header_nocopy("Access-Control-Allow-Origin: *\r\n");
            // conn->send(rsp);
        });

        event::TimerRepeat timer_tcmalloc(loop, 10s, [] { MallocExtension::instance()->ReleaseFreeMemory(); });

        log_info("hello_server started");
        loop->run();
        log_info("hello_server stopped");
    });

    pinThread(sThread, aCpuNum);
    return sThread;
}

}  // namespace webtest