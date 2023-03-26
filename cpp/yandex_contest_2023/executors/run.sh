#!/bin/bash
clang++ -std=c++20 main.cpp || exit 1
echo "test1"
cat test1.txt | ./a.out
echo "test2"
cat test2.txt | ./a.out
echo "test3"
cat test3.txt | ./a.out
