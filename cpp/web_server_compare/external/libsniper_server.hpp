#pragma once

#include <thread>

#include "libsniper/sniper/event/Loop.h"
#include "libsniper/sniper/event/Timer.h"
#include "libsniper/sniper/http/Server.h"
#include "libsniper/sniper/http/server/Connection.h"
#include "libsniper/sniper/http/server/Request.h"
#include "libsniper/sniper/http/server/Response.h"
#include "libsniper/sniper/log/log.h"
#include "libsniper/sniper/std/check.h"
#include "libsniper/sniper/std/vector.h"
#include "libsniper/sniper/threads/Stop.h"

namespace sniper {

void stop_signal(const event::loop_ptr& loop) {
    log_warn("Stop signal. Exiting");
    threads::Stop::get().stop();
    loop->break_loop(ev::ALL);
}

static void sigsegv_handler(int sig) {
    log_err("Error: signal {} ({})", strsignal(sig), sig);
    log_err("{}", stacktrace_to_string(boost::stacktrace::stacktrace()));
    exit(1);
}

class Server {
    using Callback =
        function<void(const intrusive_ptr<sniper::http::server::Connection>&, const intrusive_ptr< sniper::http::server::Request>&, const intrusive_ptr< sniper::http::server::Response>&)>;

   public:
    Server(Callback&& cb) {
        for (int i = 0; i < 20; ++i) {
            _workers.emplace_back(&Server::worker, this, cb);
        }
    }

    ~Server() {
        for (auto& w: _workers) {
            if (w.joinable()) w.join();
        }
    }

   private:
    void worker(Callback&& cb) noexcept {
        try {
            auto loop = event::make_loop();
            check(loop, "[Server] cannot init event loop");

            auto config = http::server::make_config();
            // config->normalize = true;
            // config->add_server_header = true;
            // config->add_date_header = true;

            http::Server server(loop, config);
            check(server.bind("0.0.0.0", 8080));

            server.set_cb(std::move(cb));

            event::TimerRepeat timer_stop(loop, 1s, [&loop] {
                if (threads::Stop::get().is_stopped()) loop->break_loop(ev::ALL);
            });

            loop->run();
        } catch (std::exception& e) {
            log_err(e.what());
        } catch (...) {
            log_err("[Server] non std::exception occurred");
        }

        threads::Stop::get().stop();
    }

    std::vector<std::thread> _workers;
};

}  // namespace sniper