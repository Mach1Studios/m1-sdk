#!/bin/bash

PATH=${TRAVIS_BUILD_DIR}

if [ ! -z "$PATH" ]; then
    echo "Detected travis CI pathing..."
    mkdir -p ${TRAVIS_BUILD_DIR}/source/_install/vs-15-2017/Release/lib-shared
    mkdir -p ${TRAVIS_BUILD_DIR}/source/_install/vs-15-2017/Debug/lib-shared
    mkdir -p ${TRAVIS_BUILD_DIR}/source/_install/vs-15-2017-win64/Release/lib-shared
    mkdir -p ${TRAVIS_BUILD_DIR}/source/_install/vs-15-2017-win64/Debug/lib-shared
    mkdir -p ${TRAVIS_BUILD_DIR}/source/_install/vs-15-2017/Release/lib
    mkdir -p ${TRAVIS_BUILD_DIR}/source/_install/vs-15-2017/Debug/lib
    mkdir -p ${TRAVIS_BUILD_DIR}/source/_install/vs-15-2017-win64/Release/lib
    mkdir -p ${TRAVIS_BUILD_DIR}/source/_install/vs-15-2017-win64/Debug/lib
    mkdir -p ${TRAVIS_BUILD_DIR}/source/_install/vs-15-2017-mt/Release/lib
    mkdir -p ${TRAVIS_BUILD_DIR}/source/_install/vs-15-2017-mt/Debug/lib
    mkdir -p ${TRAVIS_BUILD_DIR}/source/_install/vs-15-2017-win64-mt/Release/lib
    mkdir -p ${TRAVIS_BUILD_DIR}/source/_install/vs-15-2017-win64-mt/Debug/lib
else
    # Local non CI/CD install
fi