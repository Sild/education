#!/bin/bash -e
user_dir="$(pwd)"
root_dir="$(git rev-parse --show-toplevel)"
cd "${root_dir}" || (echo "Fail to change dir" && exit 1)

mkdir -p build
cd build
cmake ../
cmake --build .
./server
cd ${user_dir}
