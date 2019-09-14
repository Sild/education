#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include "data_storage.h"

class IDataHandler;

class TInputReader {
public:
    TInputReader(size_t defaultBulkSize): DefaulBulkSize(defaultBulkSize) {}

    void Start();

    void Register(IDataHandler& dataHandler) {
        DataHandlers.emplace_back(dataHandler);
    }

private:
    void Dump();
    std::vector<std::reference_wrapper<IDataHandler>> DataHandlers;
    size_t DefaulBulkSize = 0;
    TDataStorage DataStorage;
    size_t ReadRawCount = 0;
    size_t CommandsCount = 0;
    size_t BlocksCount = 0;
};


