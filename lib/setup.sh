#!/bin/bash

# This is a utility script to download and install a modern version of all the needed dependencies
# WARNING: Please run this script from the directory it is in
# NOTE: This script expects that you will want m1-sdk repo and other repos to all share the same parent directory

# Dependency Versions
cmake_version=3.20.2

# polly
cd ../../ && cwd=$(pwd)
git clone git@github.com:Mach1Studios/polly.git
touch ~/.bashrc
echo -e "alias polly=$(pwd)/polly/bin/polly" >> ~/.bashrc 
source ~/.bashrc
echo "TESTING POLLY ALIAS"
which polly

# cmake > 3.19
mkdir ~/temp
cd ~/temp
wget wget https://cmake.org/files/v$version/cmake-$version.tar.gz
tar -xzvf cmake-$version.tar.gz
cd cmake-$version/
./bootstrap
make
sudo make install
