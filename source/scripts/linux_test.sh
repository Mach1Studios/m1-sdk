#!/bin/bash

cd ${TRAVIS_BUILD_DIR}/source/tests

echo "LINUX: TESTS"
cmake . -B_builds/gcc -DCMAKE_INSTALL_PREFIX=`pwd`/_install/gcc
cmake --build _builds/gcc --config Release --target install

echo "RUN TESTS"
_install/gcc/bin/Mach1EncodeTests
_install/gcc/bin/Mach1DecodeTests
_install/gcc/bin/Mach1DecodePositionalTests
_install/gcc/bin/Mach1SpatialTests
_install/gcc/bin/Mach1TranscodeTests

echo "RENDER CHECK#"
echo "5.1_C -> M1Spatial"
cd Mach1TranscodeRenderTest
mkdir -p build && cd build
cmake ../
cmake --build .
./spatial-transcode-render -in-file "../m1-debug-shortpt-fiveone.wav" -in-fmt 5.1_C -out-file "../m1-debug-shortpt-fiveone_m1spatial-out.wav" -out-fmt M1Spatial -out-file-chans 0