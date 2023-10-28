#include <iostream>

struct X {
    auto operator<=>(const X&) const = default;

    auto operator<=>(int r) const {
        return this->a <=> r;
    }
    bool operator==(int r) const {
        return operator<=>(r) == 0;
    }
    int a;
};

int main() {
    X x1{1}, x42{42};

    std::cout << (x1 < x42 ? "x1 < x42" : "not x1 < x42") << std::endl;
    std::cout << (x1 > x42 ? "x1 > x42" : "not x1 > x42") << std::endl;
    std::cout << (x1 <= x42 ? "x1 <= x42" : "not x1 <= x42") << std::endl;
    std::cout << (x1 >= x42 ? "x1 >= x42" : "not x1 >= x42") << std::endl;
    std::cout << (x1 == x42 ? "x1 == x42" : "not x1 == x42") << std::endl;
    std::cout << (x1 != x42 ? "x1 != x42" : "not x1 != x42") << std::endl;

    std::cout << (x1 < 42 ? "x1 < 42" : "not x1 < 42") << std::endl;
    std::cout << (x1 > 42 ? "x1 > 42" : "not x1 > 42") << std::endl;
    std::cout << (x1 <= 42 ? "x1 <= 42" : "not x1 <= 42") << std::endl;
    std::cout << (x1 >= 42 ? "x1 >= 42" : "not x1 >= 42") << std::endl;

    // std::cout << (x1 == 42 ? "x1 == 42" : "not x1 == 42") << std::endl;
    // std::cout << (x1 != 42 ? "x1 != 42" : "not x1 != 42") << std::endl; 
}