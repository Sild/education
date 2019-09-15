#include <iostream>
#include <libs.h>
#include <functional>

auto read_pool() {
    using namespace std;
    using TIp = vector<uint32_t>;
    vector<TIp> ip_pool;
    for(string line; std::getline(cin, line);) {
        vector<string> v = split(line, '\t');
        auto ip = split(v.at(0), '.');
        ip_pool.emplace_back(TIp({stoui(ip.at(0)), stoui(ip.at(1)), stoui(ip.at(2)), stoui(ip.at(3))}));
    }
    return ip_pool;
}

template<typename T>
void print_ip(const T& ip) {
    for(auto ip_part = ip.cbegin(); ip_part != ip.cend(); ++ip_part) {
        if (ip_part != ip.cbegin()) {
            std::cout << ".";
        }
        std::cout << *ip_part;
    }
    std::cout << "\n";
}

// очень странное использование шаблонов
template<typename T, typename UnaryPredicate>
void print_ips_lambda(const T& ips, UnaryPredicate filter_in) {
    for(const auto& ip: ips) {
        if(filter_in(ip)) {
            print_ip(ip);
        }
    }
}

int main() {
    try {
        auto ip_pool = read_pool();
        sort(ip_pool);

        print_ips_lambda(ip_pool, filter_accept_all);

        print_ips_lambda(ip_pool, filter_seq<decltype(ip_pool)::value_type, 1>);

        print_ips_lambda(ip_pool, filter_seq<decltype(ip_pool)::value_type, 46, 70>);

        print_ips_lambda(ip_pool, filter_accept_check_any);
    }
    catch(const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
