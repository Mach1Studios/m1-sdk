#!/bin/bash

if [ -d "$TRAVIS_BUILD_DIR" ]; then
    cd ${TRAVIS_BUILD_DIR}
    echo "Travis CI detected, installing cmake in ${TRAVIS_BUILD_DIR}..."
    if [ -d "$TRAVIS_CMAKE_PATH/cmake-3.19.3" ]; then
        # Take action if $DIR exists. #
        echo "Cmake folder already exists: ${TRAVIS_CMAKE_PATH}/cmake-3.19.3..."
        cd cmake-3.19.3
        ./bootstrap 1> /dev/null
        make -s 
        sudo make -s install
        export PATH=/usr/local/bin/cmake:$PATH
    else
        echo "Installing config files in ${TRAVIS_BUILD_DIR}/..."
        wget --quiet -nc https://github.com/Kitware/CMake/releases/download/v3.19.3/cmake-3.19.3.zip
        unzip -qq -n cmake-3.19.3.zip
        cd cmake-3.19.3
        ./bootstrap 1> /dev/null
        make -s 
        sudo make -s install
        export PATH=/usr/local/bin/cmake:$PATH
    fi
else
    # Local non CI/CD install
    echo "Warning: Installing cmake in current work directy..."
    wget -nc https://github.com/Kitware/CMake/releases/download/v3.19.3/cmake-3.19.3.zip
    unzip -n cmake-3.19.3.zip
    cd cmake-3.19.3
    ./bootstrap
    make -s 
    sudo make -s install
    export PATH=/usr/local/bin/cmake:$PATH
fi