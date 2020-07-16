#pragma once
#include <map>
#include <memory>
#include <exception>

namespace NMatrix {


template<typename T, T DefaultValue, size_t Dimension>
class TMatrix {

using TStorageKey = std::array<T, Dimension>;

public:

    size_t size() const {
        return Storage.size();
    };

    auto operator[](size_t index) {
        return TMatrixIndexer(*this, index);
    }

    class TMatrixIndexer {
    public:
        TMatrixIndexer(TMatrix& matrix, size_t index): Matrix(matrix), Deep(1)  {
            Key[Deep -1] = index;
        }

        TMatrixIndexer& operator[](size_t index) {
            if(++Deep > Dimension) {
                throw std::runtime_error("Try to index out of dimension");
            }
            Key[Deep - 1] = index;
            return *this;
        }

        TMatrixIndexer& operator=(T value) {
            if(Deep != Dimension) {
                throw std::runtime_error("Try to assign value for middle-state container");
            }
            if(value != DefaultValue) {
                Matrix.Storage[Key] = value;
            } else {
                auto it = Matrix.Storage.find(Key);
                if(it != Matrix.Storage.end()) {
                    Matrix.Storage.erase(it);
                }
            }
            
            return *this;
        }

        operator T() {
            auto it = Matrix.Storage.find(Key);
            if(it == Matrix.Storage.end()) {
                return DefaultValue;
            }
            return it->second;
        }

    private:
        TMatrix& Matrix;
        TStorageKey Key;
        size_t Deep = 1;

    };

    template<typename TItType>
    class TMatrixIterator {
    public:
        TMatrixIterator(const TItType& iterator): Current(iterator) {}

        auto operator*() {
            return std::tuple_cat(unpack(Current->first), std::make_tuple(Current->second));
        }

        TMatrixIterator& operator++() {
            Current++;
            return *this;
        }
        bool operator!=(TMatrixIterator other) {
            return Current != other.Current;
        }

    private:
        template <std::size_t... Is>
        auto unpack_impl(const TStorageKey& arr, std::index_sequence<Is...>) -> decltype(std::make_tuple(arr[Is]...)) {
            return std::make_tuple( arr[Is]...);
        }

        auto unpack(const TStorageKey &arr) -> decltype(unpack_impl(arr, std::make_index_sequence<Dimension>{})) {
            return unpack_impl(arr, std::make_index_sequence<Dimension>{});
        }

        TItType Current;
    };

    auto begin() {
        return TMatrixIterator<typename std::map<TStorageKey, T>::iterator>(Storage.begin());
    }
    auto end() {
        return TMatrixIterator<typename std::map<TStorageKey, T>::iterator>(Storage.end());
    }



private:
    std::map<TStorageKey, T> Storage;
};


}