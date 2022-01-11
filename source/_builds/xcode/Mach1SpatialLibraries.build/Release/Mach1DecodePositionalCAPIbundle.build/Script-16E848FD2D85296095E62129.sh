#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd /Volumes/git/m1-sdk/lib/_builds/xcode
  /usr/local/Cellar/cmake/3.21.4/bin/cmake -E cmake_symlink_library /Volumes/git/m1-sdk/lib/_builds/xcode/Debug/libMach1DecodePositionalCAPId.1.3.0.0.bundle /Volumes/git/m1-sdk/lib/_builds/xcode/Debug/libMach1DecodePositionalCAPId.1.3.0.0.bundle /Volumes/git/m1-sdk/lib/_builds/xcode/Debug/libMach1DecodePositionalCAPId.bundle
fi
if test "$CONFIGURATION" = "Release"; then :
  cd /Volumes/git/m1-sdk/lib/_builds/xcode
  /usr/local/Cellar/cmake/3.21.4/bin/cmake -E cmake_symlink_library /Volumes/git/m1-sdk/lib/_builds/xcode/Release/libMach1DecodePositionalCAPI.1.3.0.0.bundle /Volumes/git/m1-sdk/lib/_builds/xcode/Release/libMach1DecodePositionalCAPI.1.3.0.0.bundle /Volumes/git/m1-sdk/lib/_builds/xcode/Release/libMach1DecodePositionalCAPI.bundle
fi
if test "$CONFIGURATION" = "MinSizeRel"; then :
  cd /Volumes/git/m1-sdk/lib/_builds/xcode
  /usr/local/Cellar/cmake/3.21.4/bin/cmake -E cmake_symlink_library /Volumes/git/m1-sdk/lib/_builds/xcode/MinSizeRel/libMach1DecodePositionalCAPI.1.3.0.0.bundle /Volumes/git/m1-sdk/lib/_builds/xcode/MinSizeRel/libMach1DecodePositionalCAPI.1.3.0.0.bundle /Volumes/git/m1-sdk/lib/_builds/xcode/MinSizeRel/libMach1DecodePositionalCAPI.bundle
fi
if test "$CONFIGURATION" = "RelWithDebInfo"; then :
  cd /Volumes/git/m1-sdk/lib/_builds/xcode
  /usr/local/Cellar/cmake/3.21.4/bin/cmake -E cmake_symlink_library /Volumes/git/m1-sdk/lib/_builds/xcode/RelWithDebInfo/libMach1DecodePositionalCAPI.1.3.0.0.bundle /Volumes/git/m1-sdk/lib/_builds/xcode/RelWithDebInfo/libMach1DecodePositionalCAPI.1.3.0.0.bundle /Volumes/git/m1-sdk/lib/_builds/xcode/RelWithDebInfo/libMach1DecodePositionalCAPI.bundle
fi

