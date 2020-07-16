#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include "data_storage.h"

class IDataHandler;

class TInputReader {
public:
    TInputReader(size_t defaultBulkSize, size_t tagNum): DefaulBulkSize(defaultBulkSize), TagNum(tagNum) {}

    TInputReader(TInputReader&& rhs):
        DataHandlers(std::move(rhs.DataHandlers)),
        DefaulBulkSize(rhs.DefaulBulkSize),
        TagNum(rhs.TagNum),
        DataStorageCommon(std::move(rhs.DataStorageCommon)),
        DataStoragePerId(std::move(rhs.DataStoragePerId)),
        ReadRawCount(static_cast<size_t>(rhs.ReadRawCount)),
        CommandsCount(static_cast<size_t>(rhs.CommandsCount)),
        BlocksCount(static_cast<size_t>(rhs.BlocksCount)),
        OpenCounterPerId(std::move(rhs.OpenCounterPerId)),
        CommandCounter(static_cast<size_t>(rhs.CommandCounter)),
        Buffer(std::move(rhs.Buffer))
    {}

    TInputReader(const TInputReader& rhs) = delete;

    void Read(const char* buffer, size_t size, size_t connId);

    void Register(std::unique_ptr<IDataHandler>&& dataHandler) {
        DataHandlers.emplace_back(std::move(dataHandler));
    }

    ~TInputReader();

private:
    void Dump(TDataStorage& aStorage);
    std::vector<std::unique_ptr<IDataHandler>> DataHandlers;
    const size_t DefaulBulkSize = 0;
    const size_t TagNum = 0;
    TDataStorage DataStorageCommon;
    std::map<size_t, TDataStorage> DataStoragePerId;
    std::map<size_t, std::atomic_int> OpenCounterPerId;
    std::mutex TotalMtx;;

    std::atomic_size_t ReadRawCount = 0;
    std::atomic_size_t CommandsCount = 0;
    std::atomic_size_t BlocksCount = 0;
    std::atomic_size_t CommandCounter = 0;
    std::string Buffer;
    static std::mutex LogMtx;
};


