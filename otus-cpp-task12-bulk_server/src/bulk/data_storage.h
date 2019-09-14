#pragma once

#include <vector>
#include <chrono>


class TDataStorage {
public:
    void Add(std::string&& cmd) {
        if(Data.empty()) {
            InitTime = std::chrono::system_clock::to_time_t( std::chrono::system_clock::now() );
        }
        Data.emplace_back(std::move(cmd));
    }

    auto GetInitTime() const {
        return InitTime;
    }

    auto& GetData() const {
        return Data;
    }

    void Clear() {
        Data.clear();
    }

    auto Empty() const {
        return Data.empty();
    }

private:
    size_t InitTime = 0;
    std::vector<std::string> Data;
};
