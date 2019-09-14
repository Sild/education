//
// echo_server.cpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/write.hpp>
#include <cstdio>
#include <bulk/async.h>
#include <atomic>
#include <iostream>

using boost::asio::ip::tcp;
using boost::asio::awaitable;
using boost::asio::co_spawn;
using boost::asio::detached;
using boost::asio::use_awaitable;
namespace this_coro = boost::asio::this_coro;

auto ReadPortNum(int argc, char* argv[]) {
    return argc >=2 ? static_cast<uint16_t>(std::stoi(argv[1])) : 4242;
}
size_t ReadBlockSize(int argc, char* argv[]) {
    return argc >=3 ? static_cast<size_t>(std::stoi(argv[2])) : 5;
}
static void* BULK_HANDLER = nullptr;
static unsigned short LISTENING_PORT = 0;
static std::atomic_size_t CONNECTION_ID(0);

awaitable<void> handle(tcp::socket socket, size_t id) {
    try {
        char data[1024];
        for (;;) {
            std::size_t n = co_await socket.async_read_some(boost::asio::buffer(data), use_awaitable);
            if (n > 0) {
                async::receive(BULK_HANDLER, data, n, id);
            }
        
        }
    } catch (std::exception& e) {
    }
}

awaitable<void> listener() {
    auto executor = co_await this_coro::executor;
    tcp::acceptor acceptor(executor, {tcp::v4(), LISTENING_PORT});
    std::cout << "listening on: " << LISTENING_PORT << std::endl;
    for (;;) {
        tcp::socket socket = co_await acceptor.async_accept(use_awaitable);
        co_spawn(executor,
            [socket = std::move(socket)]() mutable {
                return handle(std::move(socket), CONNECTION_ID++);
            },
            detached
        );
    }
}

int main(int argc, char** argv) {
    BULK_HANDLER = async::connect(ReadBlockSize(argc, argv));
    LISTENING_PORT = ReadPortNum(argc, argv);
    try {
        boost::asio::io_context io_context(1);

        boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);
        signals.async_wait([&](auto, auto){ io_context.stop(); });

        co_spawn(io_context, listener, detached);

        io_context.run();
        async::disconnect(BULK_HANDLER);
    } catch (std::exception& e) {
        std::printf("Exception: %s\n", e.what());
    }
}