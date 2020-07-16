#include <cassert>
#include <iostream>
#include "version.h"

int main() {
    assert(0 == get_build_number());
    
    std::cout << "All tests passed" << std::endl;
}
