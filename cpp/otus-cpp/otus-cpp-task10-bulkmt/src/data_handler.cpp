#include <fstream>
#include "data_handler.h"
#include "input_reader.h"

namespace std {
    std::string to_string(const std::vector<std::string>& vec) {
        std::stringstream stream;
        for(auto it = vec.begin(); it != vec.end(); ++it) {
            if(it != vec.begin()) {
                stream << ", ";
            }
            stream << *it;
        }
        return stream.str();
    }

    std::string to_string(const std::thread::id& th_id) {
        stringstream ss;
        ss << th_id;
        return ss.str();
    }
}

IDataHandler::IDataHandler(TInputReader& reader, size_t threadCount, std::string&& handlerId)
: Reader(reader)
, JobDone(false)
, HandlerId(std::move(handlerId))
{
    Reader.Register(*this);
    for(int i = 0; i < threadCount; ++i) {
        ThreadPool.emplace_back(&IDataHandler::Loop, this, i);
    }
}


void TConsoleHandler::Action(const TDataStorage &data) {
    std::lock_guard lg(LogSyncMtx);
    std::cout << "bulk: " << std::to_string(data.GetData()) << "\n";
}

void TFileHandler::Action(const TDataStorage &data) {
    thread_local size_t fileSuffux = 0;
    std::lock_guard lg(LogSyncMtx);
    std::ofstream out("bulk" + std::to_string(data.GetInitTime()) + std::to_string(++fileSuffux) + std::to_string(std::this_thread::get_id()) + ".log", std::fstream::app);
    out << std::to_string(data.GetData()) << "\n";
}