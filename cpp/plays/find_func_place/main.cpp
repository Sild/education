#include <iostream>

static int global_i = 5;
static const char* global_text = "aaa_bbb";

int foo() {
    std::cout << "foo\n";
    return 1;
};

struct MyStruct {
    void test_a() {
        std::cout << "test_a\n";
    }
};

int main() {
    MyStruct* a = nullptr;
    a->test_a();
    int b = 5;
    int c = 6;
    int* d = new int(7);
    printf("a_addr: %p\n", &a); 
    printf("b_addr: %p\n", &b); 
    printf("c_addr: %p\n", &c); 
    std::cout << "c_size=" << sizeof(c) << std::endl;
    printf("d_addr: %p\n", &d); 
    printf("d_val: %p\n", d); 
    printf("func_addr:%p\n", &MyStruct::test_a); 
    printf("global_i_addr: %p\n", &global_i); 
    printf("global_text_addr: %p\n", &global_text); 

}
