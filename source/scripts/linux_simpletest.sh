#!/bin/bash

cd ${TRAVIS_BUILD_DIR}/source/tests

echo "LINUX: TESTS"
cmake . -B_builds/gcc -DCMAKE_INSTALL_PREFIX=`pwd`/_install/gcc
cmake --build _builds/gcc --config Release --target install

echo "RUN POLLY TESTS"
_install/gcc/bin/Mach1EncodeTests
_install/gcc/bin/Mach1DecodeTests
_install/gcc/bin/Mach1DecodePositionalTests
_install/gcc/bin/Mach1SpatialTests
_install/gcc/bin/Mach1TranscodeTests