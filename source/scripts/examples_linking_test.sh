#!/bin/bash

cd ${TRAVIS_BUILD_DIR}/source

echo "RUN LINKING TESTS ON CURRENT DEVICE"
cd ${TRAVIS_BUILD_DIR}/examples/mach1spatial-c/commandline/spatial-decode-example
mkdir -p build && cd build
cmake ../
cmake --build .
cd ${TRAVIS_BUILD_DIR}/examples/mach1spatial-c/commandline/spatial-encode-example
mkdir -p build && cd build
cmake ../
cmake --build .
cd ${TRAVIS_BUILD_DIR}/examples/mach1spatial-c/commandline/spatial-decodepositional-example
mkdir -p build && cd build
cmake ../
cmake --build .
cd ${TRAVIS_BUILD_DIR}/examples/mach1spatial-c/commandline/spatial-transcode-example
mkdir -p build && cd build
cmake ../
cmake --build .