#pragma once
#include <string>
#include <optional>
#include <unordered_map>
#include <map>
#include <vector>
#include <set>
#include <mutex>

using key_t = int;

class TTable {
public:
    struct TRow {
        key_t Id;
        std::string Name;
    };

    TTable(const std::string& aTableName);
    std::optional<std::string> Insert(key_t aId, std::string&& aName);
    void Truncate();

    std::optional<TRow> GetRow(key_t aId);

    auto& GetKeys() {
        return Keys;
    }

private:
    std::vector<TRow> Data;
    std::unordered_map<key_t, size_t> KeyToPos;
    std::set<key_t> Keys;
    const std::string TableName;
};

struct TResult {
    TTable::TRow Row;
    std::string OwnTable;
};

class TDatabase {
public:
    void Insert(const std::string& aTableName, size_t aKey, std::string aValue);

    void Truncate(const std::string& aTableName);

    std::vector<TResult> Intersection(const std::string& aLhsTableName, const std::string& aRhsTableName, key_t aKey);

    std::vector<TResult> SymmetricDifference(const std::string& aLhsTableName, const std::string& aRhsTableName, key_t aKey);

private:
    TTable* GetTableByName(const std::string& aTableName);

    template<typename F>
    std::vector<TResult> LogicImpl(F f, const std::string& aLhsTableName, const std::string& aRhsTableName, key_t aKey) {
        auto storageLock = std::unique_lock(TableStorageMtx);
        auto lhs = GetTableByName(aLhsTableName);
        auto rhs = GetTableByName(aRhsTableName);

        if (!lhs || !rhs) {
            throw std::runtime_error("Intersection fail: one of table '" + aLhsTableName + "' or '" + aRhsTableName + "' does not exists");
        }
        auto& lhsIds = lhs->GetKeys();
        auto& rhsIds = rhs->GetKeys();
        std::vector<key_t> intersection;
        intersection.reserve(rhsIds.size() + rhsIds.size());
        f(lhsIds.begin(), lhsIds.end(), rhsIds.begin(), rhsIds.end(), std::back_inserter(intersection));
        std::vector<TResult> result; 
        for (auto& key: intersection) {
            auto row = lhs->GetRow(key);
            if (row) {
                result.emplace_back(TResult{std::move(*row), aLhsTableName});
            } else {
                result.emplace_back(TResult{rhs->GetRow(key).value(), aRhsTableName});
            }
        }
        return result;
    }
    std::unordered_map<std::string, TTable> TablesStorage;
    std::mutex TableStorageMtx;
};