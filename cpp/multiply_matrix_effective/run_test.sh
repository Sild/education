#!/bin/bash
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ../ && cmake --build . && ./m_multi_perf
cd ../
