#include <unistd.h>
#include "input_reader.h"
#include "data_handler.h"

void TInputReader::Start() {
    int openCounter = 0;
    size_t commandCounter = 0;
    for(std::string line; std::getline(std::cin, line);) {
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
}

void TInputReader::Dump() {
    if(DataStorage.Empty()) {
        return;
    }
    for(auto it: DataHandlers) {
        it.get().Update(DataStorage);
    }
    DataStorage.Clear();
}


