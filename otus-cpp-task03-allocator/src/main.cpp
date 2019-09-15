#include "allocator.h"
#include "container.h"

#include <iostream>
#include <map>

// cycle is prefered, but not important now
constexpr size_t fact(size_t n) {
    return n <= 1 ? 1 : n * fact(n - 1);
};

// fast tests
static_assert(fact(0) == 1, "fact faill");
static_assert(fact(1) == 1, "fact faill");
static_assert(fact(3) == 6, "fact faill");

template<typename T>
void fill_map(T& to_fill, size_t items_count) {
    for(int i = 0; i < items_count; ++i) {
        to_fill[i] = fact(i);
    }
}

template<typename T>
void fill_list(T& to_fill, size_t items_count) {
    for(int i = 0; i < items_count; ++i) {
        to_fill.add(i);
    }
}

template<typename T>
void print_map(const T& map) {
    for(const auto& p: map) {
        std::cout << p.first << " " << p.second << std::endl;
    }
}

template<typename T>
void print_list(const T& list) {
    for(const auto& e: list) {
        std::cout << e << std::endl;
    }
}

constexpr size_t RESERVED_CONTAINER_SIZE = 10;
int main() {
    std::map<int, int> default_alloc_map;
    fill_map(default_alloc_map, RESERVED_CONTAINER_SIZE);

    std::map<int, int, std::less<>, NCustom::TFixedAllocator<int /*not important for map*/, RESERVED_CONTAINER_SIZE>> custom_alloc_map;
    fill_map(custom_alloc_map, RESERVED_CONTAINER_SIZE);
    print_map(custom_alloc_map);

    // more interesting cases
    // custom_alloc_map[RESERVED_CONTAINER_SIZE] = fact(RESERVED_CONTAINER_SIZE);         // malloc
    // custom_alloc_map.erase(RESERVED_CONTAINER_SIZE);                                   // free
    // custom_alloc_map[RESERVED_CONTAINER_SIZE] = fact(RESERVED_CONTAINER_SIZE);         // malloc

    // custom_alloc_map.erase(RESERVED_CONTAINER_SIZE / 2);                               // no free
    // custom_alloc_map[RESERVED_CONTAINER_SIZE / 2] = fact(RESERVED_CONTAINER_SIZE / 2); // no malloc

    NCustom::TLinkedList<int> default_alloc_container;
    fill_list(default_alloc_container, RESERVED_CONTAINER_SIZE);

    NCustom::TLinkedList<int, NCustom::TFixedAllocator<int, RESERVED_CONTAINER_SIZE>> custom_alloc_container;
    fill_list(custom_alloc_container, RESERVED_CONTAINER_SIZE);
    print_list(custom_alloc_container);

    return 0;
}
