#ifndef CPP_TASKS_STRING_H
#define CPP_TASKS_STRING_H

#include <iostream>
#include <cstring>
#include <memory>
#include <string>
namespace sild {
    class String {
    public:
        String(): arr_size(1) {
            data = new char[1];
            data[0] = '\0';
        };

        String(const char *buffer): arr_size(std::strlen(buffer) + 1) {
            data = new char[arr_size];
            strncpy(data, buffer,arr_size);
        };

        String(const String &str): arr_size(str.arr_size) {
            data = new char[arr_size];
            strncpy(data, str.data, arr_size);
        }

        String(String&& str):arr_size(str.arr_size), data(str.data) {
            str.data = nullptr;
            str.arr_size = 0;
        }

        String &operator=(const String &str);

        String &operator=(String&& str);

        virtual ~String();

        bool operator!=(const String &target) const;

        bool operator==(const String &target) const;

        char &operator[](const size_t index) const;

        size_t length() const;

        std::unique_ptr<char[]> c_str() const;

        friend std::ostream& operator<<(std::ostream& os, const String& str) {
            os << str.data;
            return os;
        }

    private:
        char *data;
        size_t arr_size;
        void swap(String &target) noexcept;
    };
}
#endif //CPP_TASKS_STRING_H
