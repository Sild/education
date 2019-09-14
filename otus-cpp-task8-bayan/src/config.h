#pragma once

#include <string>
#include <vector>

using storage_many_t = std::vector<std::string>;

struct TConfig {
    enum class ECheckMethod {
        CRC32 = 0,
        MD5,
    };
    storage_many_t IncludeToSearchDirs;
    storage_many_t ExcludeToSearchDirs;
    storage_many_t FilesMask;
    size_t MinCheckFileSizeBytes = 0;
    bool Recursive;
    size_t CheckBlockSize = 0;
    ECheckMethod CheckMethod = ECheckMethod::CRC32;

    bool IsValid() {
        if (IncludeToSearchDirs.empty() || 0 == CheckBlockSize) {
            return false;
        }
        return true;
    }

};

std::ostream& operator<<(std::ostream& os, const storage_many_t& vec) {
    for(const auto& e: vec) {
        os << "'" << e << "', ";
    }
    return os;
}

std::istream& operator>>(std::istream& in, TConfig::ECheckMethod& checkMethod) {
    std::string token;
    in >> token;
    if (token == "crc32")
        checkMethod = TConfig::ECheckMethod::CRC32;
    else if (token == "md5")
        checkMethod = TConfig::ECheckMethod::MD5;
    else
        throw std::runtime_error("Invalid TConfig::ECheckMethod deserialization");
    return in;
}

std::ostream& operator<<(std::ostream& os, TConfig::ECheckMethod checkMethod) {
    switch (checkMethod) {
        case TConfig::ECheckMethod::CRC32:
            os << "crc32";
            break;
        case TConfig::ECheckMethod::MD5:
            os << "md5";
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const TConfig& config) {
    os << "Parsed config:\n"
       << "IncludeToSearchDirs:\n" << config.IncludeToSearchDirs << "\n"
       << "ExcludeToSearchDirs:\n" << config.ExcludeToSearchDirs << "\n"
       << "FilesMask:\n" << config.FilesMask << "\n"
       << "MinCheckFileSizeBytes: " << config.MinCheckFileSizeBytes << "\n"
       << "Recursive: " << config.Recursive << "\n"
       << "CheckBlockSize: " << config.CheckBlockSize << "\n"
       << "CheckMethod: " << config.CheckMethod;
    return os;
}


