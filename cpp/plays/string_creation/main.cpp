#include <cstring>
#include <map>
#include <stdio.h>
#include <string>

using namespace std::string_literals;

std::string str1() {
    return "test1"; // ok
}

const char* str2() {
    char* x = new char[123];
    strcpy(x, "test2"); // ok
    return x;
}

const char* str3() {
    char x[] = "test5"; // fail
    return x;
}

const char* str4() {
    return "test4"; // ok
}

const char* str5() {
    char x[123];
    strcpy(x, "test3"); // fail
    return x;
}


std::string const6 = std::string("test6", 6); // ok?
static std::string s7 = str8(); // ok


const std::string& str6() {
    return const6;
}


const std::string& str7() {
    s7 = "test7";
    return s7;
}

const std::string str8() {
    static std::string s = str1();
    s[4] = '8';
    return s;
}

using Map = std::map<const char*, int>;

Map fill() {
    Map result;
    result.emplace(str1().c_str(), 444);
    result.emplace(str2(), 333);
    result.emplace(str3(), 342);
    result.emplace(str4(), 999);
    result.emplace(str5(), 1999);
    result.emplace(str6().c_str(), 19);
    result.emplace(str7().c_str(), 1233);
    result.emplace(str8().c_str(), 1233);
    result.emplace("Hello", 123);
    result["world"] = 321;
    return result;
}

int main() {
    auto m = fill();
    for (auto [str, val] : m)
        printf("%s : %d\n", str, val);
}

