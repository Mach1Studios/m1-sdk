#! /bin/bash

mkdir libs
git clone --depth 1 git@github.com:irt-open-source/libbw64.git libs/libbw64
cd libs/libbw64
mkdir build && cd build
cmake ..
make