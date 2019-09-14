#pragma once
#include <boost/crc.hpp>
#include <config.h>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <boost/thread/thread.hpp>
#include <boost/uuid/detail/md5.hpp>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <exception>
#include <math.h>
#include <fstream>
#include <cstring>
#include <memory>

auto MAX_THREAD_COUNT = std::max(std::thread::hardware_concurrency(), 1u);

using namespace boost::filesystem;
class TDataChecker {
public:
    explicit TDataChecker(const TConfig& conf)
    : Conf(conf)
    , FinishAdd(false)
    {}

    void Prepare() {
        for (auto i = 0; i < MAX_THREAD_COUNT; ++i) {
            WorkerPool.create_thread(boost::bind(&TDataChecker::Worker, this));
        }
    }
    void AddPath(const path& p) {
        std::lock_guard lock(PathQueueMtx);
        PathQueue.push(p);
        PathQueueAddCondVar.notify_one();
    }

    void PrintDuplicates() {
        for (auto& [k, v]: SameFilesPath) {
            for (auto& file: v) {
                std::cout << file << std::endl;
            }
            std::cout << std::endl;
        }
    }

    void WaitFinish() {
        FinishAdd = true;
        PathQueueAddCondVar.notify_all();
        WorkerPool.join_all();
    }

    void Worker() {
        while (true) {
            std::unique_lock lock(PathQueueMtx);
            PathQueueAddCondVar.wait(lock, [this]()->bool{ return !this->PathQueue.empty() || FinishAdd;});
            if (PathQueue.empty()) {
                break;
            }
            auto p = PathQueue.front();
            PathQueue.pop();
            lock.unlock();
            HandlePath(p);
        }
    }

private:
    void HandlePath(const path& p) {
        std::vector<std::string> filesToCompare;
        auto newFilePath = canonical(p).string();
        {
            std::lock_guard l(HandledPathMtx);
            filesToCompare.reserve(HandledPaths.size());
            filesToCompare.insert(filesToCompare.begin(), HandledPaths.begin(), HandledPaths.end());  // нужно проверить со всеми этими файлами
            HandledPaths.insert(newFilePath);
        }
        for (const auto& toCheckFilePath: filesToCompare) {
            if (file_size(p) != file_size(toCheckFilePath)) {
                std::lock_guard lg(LogMtx);
                continue; // не проверяем на схожесть файлы разных размеров
            }

            if (AreSame(newFilePath, toCheckFilePath)) {
                std::lock_guard lg1(SameFilePathMtx);
                std::lock_guard lg2(PathToHashMtx);
                SameFilesPath[PathToHash[newFilePath]].insert(newFilePath);
                SameFilesPath[PathToHash[newFilePath]].insert(toCheckFilePath); // если это первое совпадение, добавить нужно оба (возможна оптимизация)
                break; // если были еще совпадения, то они уже обработаны ранее и уже попали в список одинаковых
            }
        }
    }

    bool AreSame(const std::string& lhs, const std::string& rhs) {
        const auto& lhsSize = file_size(lhs);
        const auto& rhsSize = file_size(rhs);
        if (lhsSize != rhsSize) {
            throw std::runtime_error("IsSame must be called only for files with same sizes");
        }
        // try to extract hash from store
        THashState lhsHash;
        THashState rhsHash;
        {
            std::lock_guard lg(PathToHashMtx);
            lhsHash = PathToHash[lhs];
            rhsHash = PathToHash[rhs];
        }

        size_t i = 0;
        // проверяем то, что уже было посчитано
        for (; i < std::min(lhsHash.Value.size(), rhsHash.Value.size()); ++i) {
            if (lhsHash.Value[i] != rhsHash.Value[i]) {
                return false;
            }
        }

        ifstream lhsStream(lhs, std::ios::in|std::ios::binary|std::ios::ate);
        ifstream rhsStream(rhs, std::ios::in|std::ios::binary|std::ios::ate);

        lhsStream.seekg(i * Conf.CheckBlockSize, std::ios_base::beg);
        rhsStream.seekg(i * Conf.CheckBlockSize, std::ios_base::beg);

        bool mustStoreLhs = false;
        bool mustStoreRhs = false;
        bool areSame = true;

        auto buffer = std::make_unique<char[]>(Conf.CheckBlockSize);
        for (; i < std::ceil(static_cast<double>(lhsSize) / Conf.CheckBlockSize); ++i) {
            if (lhsHash.Value.size() <= i) {
                std::memset(buffer.get(), 0, Conf.CheckBlockSize);
                lhsStream.read(buffer.get(), Conf.CheckBlockSize);
                lhsHash.Value.emplace_back(CalcBlockHash(buffer.get(), Conf.CheckBlockSize));
                mustStoreLhs = true;
            }
            if (rhsHash.Value.size() <= i) {
                std::memset(buffer.get(), 0, Conf.CheckBlockSize);
                rhsStream.read(buffer.get(), Conf.CheckBlockSize);
                rhsHash.Value.emplace_back(CalcBlockHash(buffer.get(), Conf.CheckBlockSize));
                mustStoreRhs = true;
            }
            if (lhsHash.Value[i] != rhsHash.Value[i]) {
                areSame = false;
                break;
            }
        }

        if (mustStoreLhs || mustStoreRhs) {
            std::lock_guard lg(PathToHashMtx);
            if (mustStoreLhs) {
                PathToHash[lhs] = lhsHash;
            }
            if (mustStoreRhs) {
                PathToHash[rhs] = rhsHash;
            }
        }
        return areSame;
    }

    std::string CalcBlockHash(char* buffer, size_t size) {
        std::string res;
        switch(Conf.CheckMethod) {
            case TConfig::ECheckMethod::CRC32: {
                boost::crc_32_type result;
                result.process_bytes(buffer, size);
                res = std::to_string(result.checksum()) + "__";
                return res;
            }
            case TConfig::ECheckMethod::MD5: {
                boost::uuids::detail::md5 hash;
                boost::uuids::detail::md5::digest_type digest;

                hash.process_bytes(buffer, size);
                hash.get_digest(digest);
                for (int i = 0; i < 4; i++) {
                    res += std::to_string(digest[i]) + "_";
                }
                res += "__";
                return res;
            }
        }
    }

    struct THashState {
        std::vector<std::string> Value;
        bool operator<(const THashState& rhs) const {
            return Value < rhs.Value;
        }
    };

    const TConfig& Conf;
    std::atomic_bool FinishAdd;
    std::queue<path> PathQueue;
    std::mutex PathQueueMtx;
    std::condition_variable PathQueueAddCondVar;
    boost::thread_group WorkerPool;
    std::mutex LogMtx;

    std::set<std::string> HandledPaths;
    std::mutex HandledPathMtx;
    std::map<THashState, std::unordered_set<std::string>> SameFilesPath;
    std::mutex SameFilePathMtx;
    std::unordered_map<std::string, THashState> PathToHash;
    std::mutex PathToHashMtx;
};
