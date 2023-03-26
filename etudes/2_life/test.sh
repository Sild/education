#!/bin/bash
cd build
cmake ../ && cmake --build . && ./iteration_perf_test
cd ../
