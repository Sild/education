#include <unistd.h>
#include "input_reader.h"
#include "data_handler.h"

void TInputReader::Start() {
    int openCounter = 0;
    size_t commandCounter = 0;
    for(std::string line; std::getline(std::cin, line);) {
        ++ReadRawCount;
        if(line == "{") {
            if(openCounter++ == 0) {
                Dump();
            }
            continue;
        } else if (line == "}") {
            --openCounter;
            if(openCounter == 0) {
                Dump();
                commandCounter = 0;
            } else if (openCounter < 0) {
                openCounter = 0;
            }
            continue;
        } else {
            ++CommandsCount;
        }
        DataStorage.Add(std::move(line));
        if(openCounter == 0) {
            if(++commandCounter == DefaulBulkSize) {
                Dump();
                commandCounter = 0;
            }
        }
//        usleep(500000);
    }
    if (openCounter == 0) {
        Dump();
    }

    for(auto& it: DataHandlers) {
        it.get().Join();
    }

    std::cout << std::endl << "main: " << ReadRawCount << " raws, " << CommandsCount << " commands, " << BlocksCount << " blocks" << std::endl;
    for(auto& it: DataHandlers) {
        auto& handlerStats = it.get().GetStats();
        for (auto& s: handlerStats) {
            std::cout << s.HandlerId << ": " << s.CommandsCount << " commands, " << s.BlocksCount << " blocks," << std::endl;
        }
    }
}

void TInputReader::Dump() {
    ++BlocksCount;
    if(DataStorage.Empty()) {
        return;
    }
    for(auto it: DataHandlers) {
        it.get().Update(DataStorage);
    }
    DataStorage.Clear();
}


