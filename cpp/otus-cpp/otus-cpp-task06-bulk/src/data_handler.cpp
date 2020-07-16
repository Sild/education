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
}

IDataHandler::IDataHandler(TInputReader& reader): Reader(reader) {
        Reader.Register(*this);
}

void TConsoleHandler::Update(TDataStorage& dataStorage) {
    std::cout << "bulk: " << std::to_string(dataStorage.GetData()) << "\n";
}

void TFileHandler::Update(TDataStorage& dataStorage) {
    std::ofstream out("bulk" + std::to_string(dataStorage.GetInitTime()) + ".log");
    out << std::to_string(dataStorage.GetData()) << "\n";
}