#include <algorithm>
#include <iostream>
#include <vector>

template<typename T>
void print(T b, T e) {
    int i = 0;
    std::cout << "++++++" << std::endl;
    while (b < e) {
        std::cout << i++ << ": " << *b++ << std::endl;
    }
    std::cout << "++++++" << std::endl;
}

//TODO find the bug
void quick_search(std::vector<int>& v, int begin, int end) {
    auto b = begin, e = end;
    if (begin == --end) return;
    auto pos = begin + (end - begin) / 2;
    auto middle = v[pos];
    while (begin < end) {
        while (v[begin] < middle) begin++;
        while (v[end] > middle) end--;
        if (begin < end) {
            std::swap(v[begin], v[end]);
            begin++; end--;
        }
    }
    std::cout << "pos: " << pos << std::endl;
    std::cout << "middle: " << middle << std::endl;
    std::cout << "b: " << b << "; e: " << e << std::endl;
    std::cout << "begin: " << begin << "; end: " << end << std::endl;
    print(v.begin(), v.end());
    std::cout << "=======" << std::endl;

    quick_search(v, b, begin+1);
    quick_search(v, begin + 1, e);

}


int main() {
    std::vector<int> a{9, 3, 7, 6, 5, 4, 3, 2, 1, 0};
    print(a.begin(), a.end());
    quick_search(a, 0, 10);
//    print(a.begin(), a.end());
}