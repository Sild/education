#include <string>
#include <optional>
#include <unordered_map>
#include <map>
#include <vector>
#include <set>
#include "db.h"

// TTable definitions

TTable::TTable(const std::string& aTableName)
: TableName(aTableName)
{}

std::optional<std::string> TTable::Insert(key_t aId, std::string&& aName) {
    if (KeyToPos.find(aId) != KeyToPos.end()) {
        return "Insert fail. Duplicate key: " + std::to_string(aId) + " in table: " + TableName;
    }
    Data.emplace_back(TRow{aId, std::move(aName)});
    KeyToPos.emplace(std::make_pair(aId, Data.size() - 1));
    Keys.insert(aId);
    return std::nullopt;
}

void TTable::Truncate() {
    Data.clear();
    KeyToPos.clear();
    Keys.clear();
}

std::optional<TTable::TRow> TTable::GetRow(key_t aId) {
    if (auto it = KeyToPos.find(aId); it != KeyToPos.end()) {
        return Data[it->second];
    }
    return std::nullopt;
}


//TDatabase definition
void TDatabase::Insert(const std::string& aTableName, size_t aKey, std::string aValue) {
    auto storageLock = std::unique_lock(TableStorageMtx);
    auto sTable = TablesStorage[aTableName];
    auto res = sTable.Insert(aKey, std::move(aValue));
    if (res) {
        throw std::runtime_error(res.value());
    }
}

void TDatabase::Truncate(const std::string& aTableName) {
    auto storageLock = std::unique_lock(TableStorageMtx);
    auto table = GetTableByName(aTableName);
    if (!table) {
        throw std::runtime_error("Truncate fail: table '" + aTableName + "' does not exists");
    }
    table->Truncate();
}

std::vector<TResult> TDatabase::Intersection(const std::string& aLhsTableName, const std::string& aRhsTableName, key_t aKey) {
    return LogicImpl(std::set_intersection, aLhsTableName, aRhsTableName, aKey);
}

std::vector<TResult> TDatabase::SymmetricDifference(const std::string& aLhsTableName, const std::string& aRhsTableName, key_t aKey) {
    return LogicImpl(std::set_symmetric_difference, aLhsTableName, aRhsTableName, aKey);
}

TTable* TDatabase::GetTableByName(const std::string& aTableName) {
    if (auto it = TablesStorage.find(aTableName); it != TablesStorage.end()) {
        return &it->second;
    }
    return nullptr;
}