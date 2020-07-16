#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include "data_storage.h"

class IDataHandler;

class TInputReader {
public:
    TInputReader(size_t defaultBulkSize, size_t tagNum): DefaulBulkSize(defaultBulkSize), TagNum(tagNum) {}

    void Read(const char* buffer, size_t size);

    void Register(std::unique_ptr<IDataHandler>&& dataHandler) {
        DataHandlers.emplace_back(std::move(dataHandler));
    }
    TInputReader(const TInputReader&) = default;
    TInputReader(TInputReader&&) = default;

    ~TInputReader();

private:
    void Dump();
    std::vector<std::unique_ptr<IDataHandler>> DataHandlers;
    size_t DefaulBulkSize = 0;
    TDataStorage DataStorage;
    size_t ReadRawCount = 0;
    size_t CommandsCount = 0;
    size_t BlocksCount = 0;
    int OpenCounter = 0;
    size_t CommandCounter = 0;
    size_t TagNum = 0;
    std::string Buffer;
};


