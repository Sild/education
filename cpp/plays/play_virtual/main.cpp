#include <iostream>

struct A {
    ~A() = default;
    int first = 0;
    int second = 0;
};

struct B {
    virtual ~B() = default;
    int first = 0;
    int second = 0;
};

using namespace std;


template<typename T>
void inspect(const T& obj, const std::string& tag) {
    cout << tag << " size=" << sizeof(obj) << endl;
    cout << tag << " offset_first=" << offsetof(T, first) << endl;


}

int main() {
    auto a = A();
    auto b = B();
    std::cout << "run inspect" << std::endl;
    inspect(a, "tag_a");
    inspect(b, "tag_b");

}
