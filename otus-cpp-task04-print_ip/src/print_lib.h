#pragma once
#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <tuple>
#include <limits>
#include <type_traits>

template<typename T>
typename std::enable_if_t<std::is_integral_v<T>, void> print_ip(const T& ip) {
    std::cout << "integral: ";
    std::make_unsigned_t<T> repr  = ip;
    decltype(repr) mask = std::numeric_limits<decltype(repr)>::max();
    for(size_t i = sizeof(T); i > 0; --i) {
        if(i !=  sizeof(T)){
            std::cout << ".";
        }
        unsigned char b = (repr >> (8*(i - 1))) & mask;
        std::cout << +b;
    }
    std::cout << std::endl;
}

template<typename T>
typename std::enable_if_t<
    std::is_same_v<std::list<typename T::value_type>, T> || std::is_same_v<std::vector<typename T::value_type>, T>,
    void
> print_ip(const T& ip) {
    std::cout << "iterable: ";
    auto begin = ip.begin();
    for(auto it = ip.begin(); it != ip.end(); ++it) {
        if(it != begin) {
            std::cout << ".";
        }
        std::cout << *it;
    }
    std::cout << std::endl;

}

void print_ip(const std::string& ip) {
        std::cout << "string: " << ip << std::endl;
}

template<typename T, typename ...Ts>
struct are_same {
    const static bool value = true;
};

template<typename T, typename ...Ts>
constexpr inline bool are_same_v = are_same<T, Ts...>::value;

template<typename T, typename V, typename ...Ts>
struct are_same<T, V, Ts...> {
    const static bool value = std::is_same_v<T, V> && are_same_v<V, Ts...>;
};



namespace {
template<size_t index, size_t maxIndex, typename... Args>
struct print_tuple {
    void operator()(const std::tuple<Args...> &tuple) {
        if (index != 0) {
            std::cout << ".";
        }
        std::cout << std::get<index>(tuple);
        print_tuple<index + 1, maxIndex, Args...>{}(tuple);
    }
};

template<size_t maxIndex, typename... Args>
struct print_tuple<maxIndex, maxIndex, Args...> {
    void operator()(const std::tuple<Args...> &tuple) {
        std::cout << std::endl;
    }
};
}

template<typename... Args>
std::enable_if_t<are_same_v<Args...>, void> print_ip(const std::tuple<Args...>& ip) {
    std::cout << "tuple: ";
    print_tuple<0, std::tuple_size_v<std::remove_reference_t<decltype(ip)>>, Args...>{}(ip);
}