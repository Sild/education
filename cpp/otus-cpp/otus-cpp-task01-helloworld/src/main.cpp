#include <iostream>
#include <version.h>
int main() {
    std::cout << "build " << get_build_number() << std::endl;
    std::cout << "Hello, World!" << std::endl;
}