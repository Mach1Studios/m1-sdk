#!/bin/bash

cd ${TRAVIS_BUILD_DIR}
echo "INSTALLING PYTHON3"
wget --quiet https://www.python.org/ftp/python/3.8.4/Python-3.8.4.tgz
tar -xf Python-3.8.4.tgz
cd Python-3.8.4
./configure 1> /dev/null
make -s
make -s install