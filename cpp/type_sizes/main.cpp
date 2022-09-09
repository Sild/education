#include <iostream>
#include <optional>
#include <vector>
#include <array>

template<typename... T>
void printLine(T... args);

template<typename F>
void printLine(const F& aFirst)
{
    std::cout << aFirst << std::endl;
}

template<typename F, typename... O>
void printLine(F aFirst, O... aOther)
{
    std::cout << aFirst << " ";
    printLine(std::forward<O>(aOther)...);
}

struct A
{
    int f;
    int s;
};

struct B
{
    int f;
    int s;
    std::string t;
};

int main()
{
    printLine("int32_t", sizeof(int32_t));
    printLine("optional<int32_t>", sizeof(std::optional<int32_t>));
    printLine("std::string", sizeof(std::string));

    std::string str = "123";
    printLine("std::string", str, sizeof(str));

    str = "1234567891011121314151617181920212223241526";
    printLine("std::string", str, "sizeof", sizeof(str));
    printLine("std::string", str, "size", str.size());
    printLine("sizeof(A)", sizeof(A));
    printLine("sizeof(B)", sizeof(B));
    printLine("sizeof(vector<int>())", sizeof(std::vector<int>));
    printLine("sizeof(vector<double>())", sizeof(std::vector<double>));

    std::vector<double> sVec1(46);
    printLine("sizeof(vector<int>(46))", sizeof(sVec1));
    printLine("sizeof + capacity * inner_size)", sizeof(std::vector<double>) + sVec1.capacity() * sizeof(double));

    sVec1.push_back(13.45);

    printLine("sizeof + capacity * inner_size)", "push_back", sizeof(std::vector<double>) + sVec1.capacity() * sizeof(double));

    std::array<double, 10> sArr;
    printLine("sizeof(array)", sizeof(sArr));
}
