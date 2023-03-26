#!/bin/bash
cd build
cmake ../ && cmake --build . && ./life
cd ../
