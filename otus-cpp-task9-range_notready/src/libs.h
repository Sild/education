#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <limits>
#include <typeinfo>
#include <iostream>

auto stoui(const std::string& str) {
    auto tmp = std::stoul(str);
    if (tmp > std::numeric_limits<uint32_t>::max()) {
        throw std::bad_cast();
    }
    return static_cast<uint32_t>(tmp);
}

auto split(const std::string &str, char d)
{
    std::vector<std::string> r;

    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while(stop != std::string::npos)
    {
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}

template<typename T>
void sort(T& ips) {
    std::sort(ips.begin(), ips.end(), std::greater<typename T::value_type>());
}

auto filter_accept_all = [](const auto&){return true;};

auto filter_accept_check_any = [](const auto& a) {
            return std::find(a.begin(), a.end(), 46) != a.end();
};

namespace {
    template<typename C, size_t pos>
    bool filter_seq_impl(const C&) {
        return true;
    }

    template<typename C, size_t pos, size_t F, size_t... Args>
    bool filter_seq_impl(const C& container) {
        return container.at(pos) == F && filter_seq_impl<C, pos + 1, Args...>(container);
    }
}

template<typename C, size_t... Args>
bool filter_seq(const C& container) {
    constexpr auto seq_len = sizeof...(Args);
    if(seq_len > container.size()) {
        throw std::runtime_error("too much sequence elements to check");
    }
    return filter_seq_impl<C, 0, Args...>(container);
}