#pragma once

#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include "config.h"
#include <iostream>
#include <data_checker.h>

using namespace boost::filesystem;

namespace boost::filesystem {
    template<typename T>
    void no_push(T& itr){}

    template<>
    void no_push<recursive_directory_iterator>(recursive_directory_iterator& itr) {
        itr.no_push();
    }
}

class FsWalker {
public:
    FsWalker(const TConfig& conf)
    : Conf(conf)
    , PathsToInclude(conf.IncludeToSearchDirs.cbegin(), conf.IncludeToSearchDirs.cend())
    , PathsToExclude(conf.ExcludeToSearchDirs.cbegin(), conf.ExcludeToSearchDirs.cend())
    , DataChecker(conf)
    {
        for(auto m: Conf.FilesMask) {
            // TODO  надо бы поинсертить .* вместо * для маски
            FilesMask.emplace(m, boost::regex::icase);
        }
    }

    void Run() {
        DataChecker.Prepare();
        for(auto& dir: PathsToInclude) {
            if (Conf.Recursive) {
                FindAll<recursive_directory_iterator>(dir);
            } else {
                FindAll<directory_iterator>(dir);
            }
        }
        DataChecker.WaitFinish();
    }

    void PrintDuplicates() {
        DataChecker.PrintDuplicates();
    }
private:
    template<typename TDirItr>
    void FindAll(const std::string& dirPath) {
        auto p = path(dirPath);
        if (IsFileSatisfyConditions(p)) {
            DataChecker.AddPath(p);
            return;
        }

        TDirItr dirItr(dirPath);
        TDirItr end;
        while (dirItr != end) {
            auto entry = *dirItr;
            if (IsFileSatisfyConditions(entry)) {
                DataChecker.AddPath(entry);
            } else if (is_directory(entry) && IsPathExcluded(entry)) {
                no_push(dirItr);
            }
            ++dirItr;
        }
    }

    bool IsFileSatisfyConditions(const path& p) {
        return !is_directory(p)
            && IsPathMatchMask(p)
            && IsPathMatchSize(p)
            && !IsPathExcluded(p);
    }

    bool IsPathMatchMask(const path& p) {
        if (FilesMask.empty()) {
            return true;
        }
        static boost::smatch unusedResult;
        for (const auto& fmRegexp: FilesMask) {
            if (boost::regex_match(p.filename().string(), unusedResult, fmRegexp)) {
                return true;
            }
        }
        return false;
    }

    bool IsPathMatchSize(const path& p) {
        return file_size(p) >= Conf.MinCheckFileSizeBytes;
    }

    bool IsPathExcluded(const path& p) {
        for (const auto& pRow: {canonical(p).string(), p.relative_path().string()}) {
            for (auto& ex: PathsToExclude) {
                if (pRow.find(ex) != std::string::npos) {
                    return true;
                }
            }
        }
        return false;
    }

    const TConfig& Conf;
    std::set<std::string> PathsToInclude;
    std::set<std::string> PathsToExclude;
    std::set<boost::regex> FilesMask;
    TDataChecker DataChecker;
};