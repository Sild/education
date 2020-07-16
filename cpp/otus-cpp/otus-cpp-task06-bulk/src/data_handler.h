#pragma once

#include <memory>
#include <sstream>
#include <iostream>
#include "data_storage.h"

class TInputReader;

class IDataHandler: private std::enable_shared_from_this<IDataHandler> {
public:
    IDataHandler(TInputReader& reader);
    virtual void Update(TDataStorage& dataStorage) = 0;
    virtual ~IDataHandler() = default;

protected:
    TInputReader& Reader;
};

class TConsoleHandler: public IDataHandler {
public:
    TConsoleHandler(TInputReader& reader): IDataHandler(reader) {};
    virtual void Update(TDataStorage& dataStorage) override;
};

class TFileHandler: public IDataHandler {
public:
    TFileHandler(TInputReader& reader): IDataHandler(reader) {};
    virtual void Update(TDataStorage& dataStorage) override;
};
