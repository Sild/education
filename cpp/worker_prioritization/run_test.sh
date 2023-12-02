#!/bin/bash
mkdir -p build
cd build
cmake ../ && cmake --build . && ./worker_prio_perf
cd ../
