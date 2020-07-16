#include <string>
#include <vector>
#include <iostream>
#include <istream>
#include <ostream>

template<typename T>
struct S {
    T &get();

    const T &get() const;

    explicit S(const T &v) : val(v) {};

    S<T> operator=(const T &v) {
        val = v;
        return *this;
    }


private:
    T val;
};

template<typename T>
T &S<T>::get() {
    return val;
};

template<typename T>
const T &S<T>::get() const {
    return val;
};

template<typename T>
void read_val(T &v) {
    std::cout << "enter value: ";
    std::cin >> v;
}

int main() {
    S<int> a(5);
    S<char> b('b');
    S<double> c(0.0);
    S<std::string> d("string_val");
    S<std::vector<int>> e({1, 2, 3, 4, 5});

    e = std::vector<int>({2, 3});

    std::cout << a.get()
              << std::endl
              << b.get()
              << std::endl
              << c.get()
              << std::endl
              << d.get()
              << std::endl;
    for (const auto v: e.get()) {
        std::cout << v << ",";
    }
    std::cout << std::endl;

    read_val(a.get());
    read_val(b.get());
    read_val(c.get());
    read_val(d.get());

    std::cout << a.get()
              << std::endl
              << b.get()
              << std::endl
              << c.get()
              << std::endl
              << d.get()
              << std::endl;
    for (const auto v: e.get()) {
        std::cout << v << ",";
    }
    std::cout << std::endl;
}