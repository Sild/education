//
// Created by Dmitry Korchagin on 04/03/2017.
//
#include <stdexcept>
#include <assert.h>
#include "string.hpp"

using namespace sild;
using namespace std;

void test_constructors();
void test_eq_operators();
void test_taking_operators();
void test_index();
void test_exceptions();

int main() {
    while(true) {
        String str("5555555");
        test_constructors();
        test_eq_operators();
        test_taking_operators();
        test_index();
        test_exceptions();
    };

}

void test_constructors() {
    String a("abcdef");
    assert(a.length() == 6);
    assert(strcmp(a.c_str().get(), "abcdef") == 0);
    String b;
    assert(b.length() == 0);
    assert(strcmp(b.c_str().get(), "") == 0);
    String c(a);
    assert(c.length() == 6);
    assert(strcmp(c.c_str().get(), "abcdef") == 0);
}

void test_eq_operators() {
    String a("123");
    String b("234");
    String c("123");

    assert(a != b);
    assert(a == c);


}

void test_taking_operators() {
    String a = "zzz";
    assert(a.length() == 3);
    assert(strcmp(a.c_str().get(), "zzz") == 0);
    a = "";
    assert(a.length() == 0);

    assert(strcmp(a.c_str().get(), "") == 0);

    String c = "000";
    a = c;
    assert(a == c);

    String b;
    a = b = c;
    assert(a == b && a == c && b == c);
}

void test_index() {
    String a = "012345";
    assert(a[0] == '0');
    assert(a[3] == '3');
    assert(a[5] == '5');

    String hello("erllo");
    hello[0] = 'h';
    hello[1] = 'e';
    assert(hello == "hello");
}

void test_exceptions() {
    String a = "";
    try {
        a[0];
        assert(false);
    } catch (std::length_error e) {}

    a = "123";
    try {
        a[4];
        assert(false);
    } catch (std::length_error e) {}
    try {
        a[5];
        assert(false);
    } catch (std::length_error e) {}
}