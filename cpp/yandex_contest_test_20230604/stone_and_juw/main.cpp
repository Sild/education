#include <iostream>
#include <string>
#include <unordered_set>

int main() {
    std::string juw;
    std::string stone;
    long b = 0;
    std::cin >> juw;
    std::cin >> stone;
    std::unordered_set<char> juw_m;
    for (int i = 0; i < juw.size(); ++ i) juw_m.insert(juw[i]);

    int res = 0;
    for (int i = 0; i < stone.size(); ++i) {
        if (juw_m.find(stone[i]) != juw_m.end()) res++;
    }
    std::cout << res << std::endl;
}