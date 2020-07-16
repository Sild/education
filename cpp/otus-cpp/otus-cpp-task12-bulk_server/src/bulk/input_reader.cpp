#include <unistd.h>
#include "input_reader.h"
#include "data_handler.h"

void TInputReader::Read(const char* buffer, size_t size, size_t connId) {
    auto lock = std::lock_guard(TotalMtx);
    Buffer += std::string(buffer, size);
    std::string line;
    auto pos = Buffer.find('\n');
    while (pos != std::string::npos) {
        line = Buffer.substr(0, pos);
        Buffer = Buffer.substr(pos + 1, Buffer.size() - pos);
        pos = Buffer.find('\n');
        ++ReadRawCount;
        if(line == "{") {
            OpenCounterPerId[connId]++;
            continue;
        } else if (line == "}") {
            auto& sOpenCounter = OpenCounterPerId[connId]; 
            --sOpenCounter;
            if(sOpenCounter == 0) {
                Dump(DataStoragePerId[connId]);
                sOpenCounter = 0;
            } else if (sOpenCounter < 0) {
                sOpenCounter = 0;
            }
            continue;
        } else {
            ++CommandsCount;
        }
        if (OpenCounterPerId[connId] == 0) {
            DataStorageCommon.Add(std::move(line));
            if(++CommandCounter == DefaulBulkSize) {
                    Dump(DataStorageCommon);
                    CommandCounter = 0;
                }
        } else {
            DataStoragePerId[connId].Add(std::move(line));
        }
    }
}

void TInputReader::Dump(TDataStorage& aStorage) {
    ++BlocksCount;
    if(aStorage.Empty()) {
        return;
    }
    for(auto& it: DataHandlers) {
        it.get()->Update(aStorage);
    }

    aStorage.Clear();
}
std::mutex TInputReader::LogMtx;
TInputReader::~TInputReader() {
    if (DataHandlers.empty()) {
        return;
    }
    for (auto& [id, counter]: OpenCounterPerId) {
        if (counter == 0) {
            Dump(DataStoragePerId[id]);
        }
    }

    for(auto& it: DataHandlers) {
        it.get()->Join();
    }

    auto logMtx = std::lock_guard(LogMtx);
    std::cout << std::endl << "TagNum: " << TagNum << "; main: " << ReadRawCount << " raws, " << CommandsCount << " commands, " << BlocksCount << " blocks" << std::endl;
    for(auto& it: DataHandlers) {
        auto& handlerStats = it.get()->GetStats();
        for (auto& s: handlerStats) {
            std::cout << s.HandlerId << ": " << s.CommandsCount << " commands, " << s.BlocksCount << " blocks," << std::endl;
        }
    }
}


