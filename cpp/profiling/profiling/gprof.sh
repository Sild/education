#!/bin/bash -e

clang++ -std=c++17 main.cpp -pg -O2
./a.out 300000000
gprof a.out > gprof.out
vim gprof.out
