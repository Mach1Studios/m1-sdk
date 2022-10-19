#!/bin/bash

cd ${TRAVIS_BUILD_DIR}
echo "INSTALLING CMAKE"
wget --quiet -nc https://github.com/Kitware/CMake/releases/download/v3.19.3/cmake-3.19.3.zip
unzip -qq -n cmake-3.19.3.zip
cd cmake-3.19.3
./bootstrap 1> /dev/null
make -s 
sudo make -s install
export PATH=/usr/local/bin/cmake:$PATH