#include <unistd.h>
#include "input_reader.h"
#include "data_handler.h"

void TInputReader::Read(const char* buffer, size_t size) {
    Buffer += std::string(buffer, size);

    std::string line;
    auto pos = Buffer.find('\n');
    while (pos != std::string::npos) {
        line = Buffer.substr(0, pos);
        Buffer = Buffer.substr(pos + 1, Buffer.size() - pos);
        pos = Buffer.find('\n');
        ++ReadRawCount;
        if(line == "{") {
            if(OpenCounter++ == 0) {
                Dump();
            }
            continue;
        } else if (line == "}") {
            --OpenCounter;
            if(CommandCounter == 0) {
                Dump();
                CommandCounter = 0;
            } else if (OpenCounter < 0) {
                OpenCounter = 0;
            }
            continue;
        } else {
            ++CommandsCount;
        }
        DataStorage.Add(std::move(line));
        if(OpenCounter == 0) {
            if(++CommandCounter == DefaulBulkSize) {
                Dump();
                CommandCounter = 0;
            }
        }
//        usleep(500000);
    }

}

void TInputReader::Dump() {
    ++BlocksCount;
    if(DataStorage.Empty()) {
        return;
    }
    for(auto& it: DataHandlers) {
        it.get()->Update(DataStorage);
    }
    DataStorage.Clear();
}

TInputReader::~TInputReader() {
    if (DataHandlers.empty()) {
        return;
    }
    if (OpenCounter == 0) {
        Dump();
    }

    for(auto& it: DataHandlers) {
        it.get()->Join();
    }

    std::cout << std::endl << "TagNum: " << TagNum << "; main: " << ReadRawCount << " raws, " << CommandsCount << " commands, " << BlocksCount << " blocks" << std::endl;
    for(auto& it: DataHandlers) {
        auto& handlerStats = it.get()->GetStats();
        for (auto& s: handlerStats) {
            std::cout << s.HandlerId << ": " << s.CommandsCount << " commands, " << s.BlocksCount << " blocks," << std::endl;
        }
    }
}


