#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd /Volumes/git/m1-sdk/lib/_builds/xcode
  /usr/local/Cellar/cmake/3.21.4/bin/cmake -DBUILD_TYPE=$CONFIGURATION$EFFECTIVE_PLATFORM_NAME -P cmake_install.cmake
fi
if test "$CONFIGURATION" = "Release"; then :
  cd /Volumes/git/m1-sdk/lib/_builds/xcode
  /usr/local/Cellar/cmake/3.21.4/bin/cmake -DBUILD_TYPE=$CONFIGURATION$EFFECTIVE_PLATFORM_NAME -P cmake_install.cmake
fi
if test "$CONFIGURATION" = "MinSizeRel"; then :
  cd /Volumes/git/m1-sdk/lib/_builds/xcode
  /usr/local/Cellar/cmake/3.21.4/bin/cmake -DBUILD_TYPE=$CONFIGURATION$EFFECTIVE_PLATFORM_NAME -P cmake_install.cmake
fi
if test "$CONFIGURATION" = "RelWithDebInfo"; then :
  cd /Volumes/git/m1-sdk/lib/_builds/xcode
  /usr/local/Cellar/cmake/3.21.4/bin/cmake -DBUILD_TYPE=$CONFIGURATION$EFFECTIVE_PLATFORM_NAME -P cmake_install.cmake
fi

