#!/bin/bash
mkdir -p build
cd build
cmake ../ && cmake --build . && ./m_multi_perf
cd ../
