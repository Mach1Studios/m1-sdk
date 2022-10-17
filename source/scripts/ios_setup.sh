#!/bin/bash

cd ${TRAVIS_BUILD_DIR}
echo "INSTALLING CMAKE"
wget --quiet https://github.com/Kitware/CMake/releases/download/v3.19.3/cmake-3.19.3.zip
unzip -qq cmake-3.19.3.zip
cd cmake-3.19.3
./bootstrap 1> /dev/null
make -s 
sudo make -s install
export PATH=/usr/local/bin/cmake:$PATH

cd ${TRAVIS_BUILD_DIR}
echo "INSTALLING SNDFILE"
wget --quiet https://github.com/libsndfile/libsndfile/releases/download/1.0.31/libsndfile-1.0.31.tar.bz2
tar -xf libsndfile-1.0.31.tar.bz2
cd libsndfile-1.0.31/
./configure 1> /dev/null
make -s
make -s install