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
        return tuple_get<Idx - 1>(t.nest);
    }
    if constexpr(Idx == 0) {
        return t.cur;
    }
}

template<typename T, typename... K>
struct my_tuple {
    my_tuple() = default;

    my_tuple<T, K...>(const T& arg, K... other):
      cur(arg)
    , nest(other...)
    {};

    T cur;
    my_tuple<K...> nest;
};

template<typename T>
struct my_tuple<T> {
    my_tuple() = default;

    my_tuple<T>(const T& val): cur(val) {}

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

    auto t2 = my_tuple<int, float, std::string>(12, 22, "test2_string");
    std::cout << tuple_get<0>(t2) << std::endl;
    std::cout << tuple_get<1>(t2) << std::endl;
    std::cout << tuple_get<2>(t2) << std::endl;
}

