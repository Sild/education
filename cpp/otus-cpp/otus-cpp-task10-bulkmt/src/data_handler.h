#pragma once

#include <memory>
#include <sstream>
#include <iostream>
#include "data_storage.h"
#include <queue>
#include <mutex>
#include <vector>
#include <thread>
#include <condition_variable>
#include <atomic>


class TInputReader;

struct TDataHandlerStats {
    std::string HandlerId;
    std::size_t BlocksCount = 0;
    std::size_t CommandsCount = 0;
};

class IDataHandler: private std::enable_shared_from_this<IDataHandler> {
public:
    IDataHandler(TInputReader& reader, size_t threadCount, std::string&& handlerId);

    void Update(TDataStorage dataStorage) {
        std::unique_lock lg(DataQueueMtx);
        DataQueue.emplace(std::move(dataStorage));
        lg.unlock();
        DataQueueCondVar.notify_one();
    }

    void Join() {
        JobDone = true;
        DataQueueCondVar.notify_all();
        for (auto& th: ThreadPool) {
            th.join();
        }
    }

    const auto& GetStats() {
        return HandlerStats;
    }

    void Loop(size_t threadNum) {
        size_t blocksCount = 0;
        size_t commandsCount = 0;
        bool queueEmpty = false;
        while (!JobDone || !queueEmpty) {
            std::unique_lock lg(DataQueueMtx);
            DataQueueCondVar.wait(lg, [this](){return !DataQueue.empty() || JobDone;});
            if (DataQueue.empty()) {
                queueEmpty = true;
                continue;
            }
            auto data = std::move(DataQueue.front());
            DataQueue.pop();
            lg.unlock();
            ++blocksCount;
            commandsCount += data.GetData().size();
            Action(data);
        }

        std::lock_guard lg(DataQueueMtx);
        HandlerStats.emplace_back(TDataHandlerStats{HandlerId + "-" + std::to_string(threadNum), blocksCount, commandsCount});
    }

protected:
    virtual void Action(const TDataStorage& data) = 0;
    virtual ~IDataHandler() = default;

protected:
    TInputReader& Reader;
    std::atomic_bool JobDone = false;
    std::queue<TDataStorage> DataQueue;
    std::mutex DataQueueMtx;
    std::condition_variable DataQueueCondVar;
    std::vector<std::thread> ThreadPool;
    std::mutex LogSyncMtx;
    std::vector<TDataHandlerStats> HandlerStats;
    std::string HandlerId;
};

class TConsoleHandler: public IDataHandler {
public:
    TConsoleHandler(TInputReader& reader, size_t threadCount): IDataHandler(reader, threadCount, "console") {};

protected:
    void Action(const TDataStorage& data) override;
};

class TFileHandler: public IDataHandler {
public:
    TFileHandler(TInputReader& reader, size_t threadCount): IDataHandler(reader, threadCount, "file") {};
protected:
    void Action(const TDataStorage& data) override;
};
