#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd /Volumes/git/m1-sdk/lib/_builds/xcode
  /usr/local/Cellar/cmake/3.21.4/bin/cmake -E cmake_symlink_library /Volumes/git/m1-sdk/lib/_builds/xcode/Debug/libMach1TranscodeCAPId.1.4.1.3.dylib /Volumes/git/m1-sdk/lib/_builds/xcode/Debug/libMach1TranscodeCAPId.1.4.1.3.dylib /Volumes/git/m1-sdk/lib/_builds/xcode/Debug/libMach1TranscodeCAPId.dylib
fi
if test "$CONFIGURATION" = "Release"; then :
  cd /Volumes/git/m1-sdk/lib/_builds/xcode
  /usr/local/Cellar/cmake/3.21.4/bin/cmake -E cmake_symlink_library /Volumes/git/m1-sdk/lib/_builds/xcode/Release/libMach1TranscodeCAPI.1.4.1.3.dylib /Volumes/git/m1-sdk/lib/_builds/xcode/Release/libMach1TranscodeCAPI.1.4.1.3.dylib /Volumes/git/m1-sdk/lib/_builds/xcode/Release/libMach1TranscodeCAPI.dylib
fi
if test "$CONFIGURATION" = "MinSizeRel"; then :
  cd /Volumes/git/m1-sdk/lib/_builds/xcode
  /usr/local/Cellar/cmake/3.21.4/bin/cmake -E cmake_symlink_library /Volumes/git/m1-sdk/lib/_builds/xcode/MinSizeRel/libMach1TranscodeCAPI.1.4.1.3.dylib /Volumes/git/m1-sdk/lib/_builds/xcode/MinSizeRel/libMach1TranscodeCAPI.1.4.1.3.dylib /Volumes/git/m1-sdk/lib/_builds/xcode/MinSizeRel/libMach1TranscodeCAPI.dylib
fi
if test "$CONFIGURATION" = "RelWithDebInfo"; then :
  cd /Volumes/git/m1-sdk/lib/_builds/xcode
  /usr/local/Cellar/cmake/3.21.4/bin/cmake -E cmake_symlink_library /Volumes/git/m1-sdk/lib/_builds/xcode/RelWithDebInfo/libMach1TranscodeCAPI.1.4.1.3.dylib /Volumes/git/m1-sdk/lib/_builds/xcode/RelWithDebInfo/libMach1TranscodeCAPI.1.4.1.3.dylib /Volumes/git/m1-sdk/lib/_builds/xcode/RelWithDebInfo/libMach1TranscodeCAPI.dylib
fi

