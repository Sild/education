#include <vector>
#include <algorithm>
#include <concepts>

template<class T>
concept IterToComparable = 
    requires(T a, T b) {
        {*a < *b} -> std::convertible_to<bool>;
    };
    
// обратите внимание на IterToComparable вместо слова class
template<IterToComparable InputIt>
void SortDefaultComparator(InputIt begin, InputIt end) {
    std::sort(begin, end);
}

struct X {
    auto operator<=>(const X&) const = default;
    int a;
};

int main() {
    std::vector<X> v = { {10}, {9}, {11} };
    SortDefaultComparator(v.begin(), v.end());
}