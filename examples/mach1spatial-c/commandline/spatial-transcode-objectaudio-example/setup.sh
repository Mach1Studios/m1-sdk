#! /bin/bash

mkdir libs && cd libs

cd ../../ && git clone --depth 1 git@github.com:irt-open-source/libbw64.git
cd libbw64
mkdir build && cd build
cmake ..
make
make install
