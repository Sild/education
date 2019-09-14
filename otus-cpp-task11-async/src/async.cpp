#include "async.h"
#include "input_reader.h"
#include "data_handler.h"
#include <map>

namespace async {
using reader_key_t = size_t;
static reader_key_t ReaderKey = 0;
static std::map<reader_key_t, TInputReader> Storage;

handle_t connect(std::size_t bulk) {
    TInputReader reader(bulk, ReaderKey);
    reader.Register(std::make_unique<TConsoleHandler>(1));
    reader.Register(std::make_unique<TFileHandler>(2));
    Storage.insert(std::make_pair(ReaderKey, std::move(reader)));
    return reinterpret_cast<handle_t>(ReaderKey++);
}

void receive(handle_t handle, const char *data, std::size_t size) {
    auto key = reinterpret_cast<reader_key_t>(handle);
    if (auto it = Storage.find(key); it != Storage.end()) {
        it->second.Read(data, size);
    }
}

void disconnect(handle_t handle) {
    Storage.erase(reinterpret_cast<reader_key_t>(handle));
}

}
