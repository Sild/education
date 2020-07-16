#include "string.hpp"

namespace sild {
    String &String::operator=(const String &str) {
        String tmp(str);
        swap(tmp);
        return *this;
    };

    String &String::operator=(String &&str) {
        swap(str);
        return *this;
    }

    bool String::operator!=(const String &target) const {
        return !(*this == target);
    }

    bool String::operator==(const String &target) const {
        return strcmp(data, target.data) == 0;
    }

    char &String::operator[](const size_t index) const {
        if (index >= (this->arr_size - 1)) {
            throw std::length_error("index is too big");
        }
        return *(data + index);
    };

    size_t String::length() const {
        return arr_size - 1;
    }

    std::unique_ptr<char[]> String::c_str() const {
        std::unique_ptr<char[]> ptr(new char[arr_size]);
        strncpy(ptr.get(), data, arr_size);
        return ptr;
    }

    String::~String() {
        delete[] data;
    };

    void String::swap(String &target) noexcept {
        std::swap(data, target.data);
        std::swap(arr_size, target.arr_size);
    }


};

