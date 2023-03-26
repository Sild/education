#!/bin/bash
clang++ -std=c++17 main.cpp || exit 1
echo "test1"
cat test1.txt | ./a.out
echo "test2"
cat test2.txt | ./a.out
echo "test3"
cat test3.txt | ./a.out
echo "test4"
cat test4.txt | ./a.out
echo "test5"
cat test5.txt | ./a.out
echo "test6"
cat test6.txt | ./a.out
