#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd /Volumes/git/m1-sdk/lib/_builds/ios
  /usr/local/Cellar/cmake/3.21.4/bin/cmake -E cmake_symlink_library /Volumes/git/m1-sdk/lib/_builds/ios/Debug${EFFECTIVE_PLATFORM_NAME}/libMach1EncodeCAPId.1.4.0.0.dylib /Volumes/git/m1-sdk/lib/_builds/ios/Debug${EFFECTIVE_PLATFORM_NAME}/libMach1EncodeCAPId.1.4.0.0.dylib /Volumes/git/m1-sdk/lib/_builds/ios/Debug${EFFECTIVE_PLATFORM_NAME}/libMach1EncodeCAPId.dylib
fi
if test "$CONFIGURATION" = "Release"; then :
  cd /Volumes/git/m1-sdk/lib/_builds/ios
  /usr/local/Cellar/cmake/3.21.4/bin/cmake -E cmake_symlink_library /Volumes/git/m1-sdk/lib/_builds/ios/Release${EFFECTIVE_PLATFORM_NAME}/libMach1EncodeCAPI.1.4.0.0.dylib /Volumes/git/m1-sdk/lib/_builds/ios/Release${EFFECTIVE_PLATFORM_NAME}/libMach1EncodeCAPI.1.4.0.0.dylib /Volumes/git/m1-sdk/lib/_builds/ios/Release${EFFECTIVE_PLATFORM_NAME}/libMach1EncodeCAPI.dylib
fi
if test "$CONFIGURATION" = "MinSizeRel"; then :
  cd /Volumes/git/m1-sdk/lib/_builds/ios
  /usr/local/Cellar/cmake/3.21.4/bin/cmake -E cmake_symlink_library /Volumes/git/m1-sdk/lib/_builds/ios/MinSizeRel${EFFECTIVE_PLATFORM_NAME}/libMach1EncodeCAPI.1.4.0.0.dylib /Volumes/git/m1-sdk/lib/_builds/ios/MinSizeRel${EFFECTIVE_PLATFORM_NAME}/libMach1EncodeCAPI.1.4.0.0.dylib /Volumes/git/m1-sdk/lib/_builds/ios/MinSizeRel${EFFECTIVE_PLATFORM_NAME}/libMach1EncodeCAPI.dylib
fi
if test "$CONFIGURATION" = "RelWithDebInfo"; then :
  cd /Volumes/git/m1-sdk/lib/_builds/ios
  /usr/local/Cellar/cmake/3.21.4/bin/cmake -E cmake_symlink_library /Volumes/git/m1-sdk/lib/_builds/ios/RelWithDebInfo${EFFECTIVE_PLATFORM_NAME}/libMach1EncodeCAPI.1.4.0.0.dylib /Volumes/git/m1-sdk/lib/_builds/ios/RelWithDebInfo${EFFECTIVE_PLATFORM_NAME}/libMach1EncodeCAPI.1.4.0.0.dylib /Volumes/git/m1-sdk/lib/_builds/ios/RelWithDebInfo${EFFECTIVE_PLATFORM_NAME}/libMach1EncodeCAPI.dylib
fi

