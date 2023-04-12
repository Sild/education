#include <iostream>
#include <assert.h>

template<typename Base, int M>
class Pow {
    static 
};

template<typename... T>
class Reverser {

};

int main() {
    using R = Reverser<1, 2, 3, 4>;
    using T = std::tuple<4,3,2,1>;
    assert(std::is_same_v<R, T>);
    return 0;
}