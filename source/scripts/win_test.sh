#!/bin/bash

cd ${TRAVIS_BUILD_DIR}/source/tests

echo "WIN: TESTS"
cmake . -B_builds/vs-15-2017 -G "Visual Studio 15 2017" -A x64 -DCMAKE_INSTALL_PREFIX=`pwd`/_install/vs-15-2017
cmake --build _builds/vs-15-2017 --config Release --target install -- /nologo /verbosity:minimal

echo "RUN TESTS"
echo " " | _install/vs-15-2017/bin/Mach1EncodeTests.exe
echo " " | _install/vs-15-2017/bin/Mach1DecodeTests.exe
echo " " | _install/vs-15-2017/bin/Mach1DecodePositionalTests.exe
echo " " | _install/vs-15-2017/bin/Mach1SpatialTests.exe
echo " " | _install/vs-15-2017/bin/Mach1TranscodeTests.exe