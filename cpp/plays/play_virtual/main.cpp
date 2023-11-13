#include <iostream>

struct A {
    ~A() = default;
    void test() {};
    int first = 0;
    int second = 0;
};

struct B {
    virtual ~B() {
        std::cout << "~B" << std::endl;
    };
    virtual void test() {cout << "b_test" << endl;};
    int first = 0;
    int second = 0;
};

struct C: public B {
    ~C() {
        std::cout << "~C" << std::endl;
    };
    virtual void test() {cout << "c_test" << endl;};
};

struct D: public C {
    ~D() {
        std::cout << "~D" << std::endl;
    };
    virtual void test() {};
};

using namespace std;


template<typename T>
void inspect(const T& obj, const std::string& tag) {
    cout << "===start with tag=" << tag << "===" << endl;
    cout << tag << " size=" << sizeof(obj) << endl;
    cout << tag << " offset_first=" << offsetof(T, first) << endl;
    cout << "===finish with tag=" << tag << "===" << endl << endl;
}

int main() {
    auto a = A();
    auto b = B();
    auto c = C();
    const B& c_b = C();
    const C& d_b = D();
    std::cout << "run inspect" << std::endl;
    inspect(a, "tag_a");
    inspect(b, "tag_b");
    inspect(c, "tag_c");
    inspect(c_b, "tag_c_b");
    inspect(d_b, "tag_d_b");

}
