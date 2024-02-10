#pragma once

// https://www.boost.org/doc/libs/master/libs/beast/example/http/server/small/http_server_small.cpp

//
// Copyright (c) 2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

//------------------------------------------------------------------------------
//
// Example: HTTP server, small
//
//------------------------------------------------------------------------------

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <memory>
#include <string>

namespace beast = boost::beast;    // from <boost/beast.hpp>
namespace http = beast::http;      // from <boost/beast/http.hpp>
namespace net = boost::asio;       // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;  // from <boost/asio/ip/tcp.hpp>

class http_connection : public std::enable_shared_from_this<http_connection> {
   public:
    http_connection(tcp::socket socket) : socket_(std::move(socket)) {}

    // Initiate the asynchronous operations associated with the connection.
    void start() {
        read_request();
        check_deadline();
    }

   private:
    tcp::socket socket_;
    beast::flat_buffer buffer_{8192};
    http::request<http::dynamic_body> request_;
    http::response<http::dynamic_body> response_;
    net::steady_timer deadline_{socket_.get_executor(), std::chrono::seconds(60)};

    // Asynchronously receive a complete request message.
    void read_request() {
        auto self = shared_from_this();

        http::async_read(socket_, buffer_, request_, [self](beast::error_code ec, std::size_t bytes_transferred) {
            boost::ignore_unused(bytes_transferred);
            if (!ec) self->process_request();
        });
    }

    // Determine what needs to be done with the request message.
    void process_request() {
        response_.version(request_.version());
        response_.keep_alive(true);

        switch (request_.method()) {
            case http::verb::get:
                response_.result(http::status::ok);
                response_.set(http::field::server, "Beast");
                create_response();
                break;

            default:
                // We return responses indicating an error if
                // we do not recognize the request method.
                response_.result(http::status::bad_request);
                response_.set(http::field::content_type, "text/plain");
                beast::ostream(response_.body())
                    << "Invalid request-method '" << std::string(request_.method_string()) << "'";
                break;
        }

        write_response();
    }

    // Construct a response message based on the program state.
    void create_response() {
        // std::cout << request_.target() << std::endl;
        response_.set(http::field::content_type, "text/html");
        beast::ostream(response_.body()) << "foo\n";
    }

    // Asynchronously transmit the response message.
    void write_response() {
        auto self = shared_from_this();

        response_.content_length(response_.body().size());

        http::async_write(socket_, response_, [self](beast::error_code ec, std::size_t) {
            self->socket_.shutdown(tcp::socket::shutdown_send, ec);
            self->deadline_.cancel();
        });
    }

    // Check whether we have spent enough time on this connection.
    void check_deadline() {
        auto self = shared_from_this();

        deadline_.async_wait([self](beast::error_code ec) {
            if (!ec) {
                // Close socket to cancel any outstanding operation.
                self->socket_.close(ec);
            }
        });
    }
};

// "Loop" forever accepting new connections.
void http_server(tcp::acceptor& acceptor, tcp::socket& socket) {
    acceptor.async_accept(socket, [&](beast::error_code ec) {
        if (!ec) std::make_shared<http_connection>(std::move(socket))->start();
        http_server(acceptor, socket);
    });
}