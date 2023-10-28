#include <iostream>
#include <string>

template<int T>
int fact() {
    return T * fact<T - 1>();
};

template<>
int fact<1>() {
    return 1;
};

template<int Idx, typename T>
auto& tuple_get(T& t) {
    if constexpr(Idx > 0) {
        return tuple_get<Idx - 1, decltype(t.nest)>(t.nest);
    }
    if constexpr(Idx == 0) {
        return t.cur;
    }
}

template<typename T, typename... K>
struct my_tuple {
    T cur;
    my_tuple<K...> nest;
};

template<typename T>
struct my_tuple<T> {
    T cur;
};

int main() {
    int res = fact<3>();
    std::cout << res << std::endl;

    auto t = my_tuple<int, float, std::string>();
    tuple_get<0>(t) = 10;
    tuple_get<1>(t) = 11;
    tuple_get<2>(t) = "test_string";
    std::cout << tuple_get<0>(t) << std::endl;
    std::cout << tuple_get<1>(t) << std::endl;
    std::cout << tuple_get<2>(t) << std::endl;
    // std::cout << tuple_get<3, decltype(t)>(t) << std::endl;
}

