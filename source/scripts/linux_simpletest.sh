#!/bin/bash

cd ${TRAVIS_BUILD_DIR}/source/tests

echo "LINUX: TESTS"
${POLLY_SOURCE_DIR}/bin/polly.py --clear --install --config Release --toolchain gcc

echo "RUN POLLY TESTS"
_install/gcc/bin/Mach1EncodeTests
_install/gcc/bin/Mach1DecodeTests
_install/gcc/bin/Mach1DecodePositionalTests
_install/gcc/bin/Mach1SpatialTests
_install/gcc/bin/Mach1TranscodeTests