#include "async.h"
#include "input_reader.h"
#include "data_handler.h"
#include <map>

namespace async {
using reader_key_t = size_t;
static std::atomic_size_t ReaderKey = 0;
static std::map<reader_key_t, TInputReader> Storage;
static std::mutex StorageMtx;

handle_t connect(std::size_t bulk) {
    TInputReader reader(bulk, ReaderKey);
    reader.Register(std::make_unique<TConsoleHandler>(1));
    reader.Register(std::make_unique<TFileHandler>(2));
    auto lock = std::lock_guard(StorageMtx);
    Storage.insert(std::make_pair(ReaderKey.load(), std::move(reader)));

    return reinterpret_cast<handle_t>(ReaderKey++);
}

void receive(handle_t handle, const char * const data, std::size_t size, size_t connection_id) {
    auto key = reinterpret_cast<reader_key_t>(handle);
    auto lock = std::unique_lock(StorageMtx);
    if (auto it = Storage.find(key); it != Storage.end()) {
        auto& reader = it->second;
        lock.unlock();
        reader.Read(data, size, connection_id);
    } else {
        std::cerr << "bulk handler not found" << std::endl;
    }
}

void disconnect(handle_t handle) {
    auto lock = std::lock_guard(StorageMtx);
    Storage.erase(reinterpret_cast<reader_key_t>(handle));
}

}
