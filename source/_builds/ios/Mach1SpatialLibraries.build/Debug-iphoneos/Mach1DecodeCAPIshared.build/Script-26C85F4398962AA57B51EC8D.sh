#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd /Volumes/git/m1-sdk/lib/_builds/ios
  /usr/local/Cellar/cmake/3.21.4/bin/cmake -E cmake_symlink_library /Volumes/git/m1-sdk/lib/_builds/ios/Debug${EFFECTIVE_PLATFORM_NAME}/libMach1DecodeCAPId.1.3.0.0.dylib /Volumes/git/m1-sdk/lib/_builds/ios/Debug${EFFECTIVE_PLATFORM_NAME}/libMach1DecodeCAPId.1.3.0.0.dylib /Volumes/git/m1-sdk/lib/_builds/ios/Debug${EFFECTIVE_PLATFORM_NAME}/libMach1DecodeCAPId.dylib
fi
if test "$CONFIGURATION" = "Release"; then :
  cd /Volumes/git/m1-sdk/lib/_builds/ios
  /usr/local/Cellar/cmake/3.21.4/bin/cmake -E cmake_symlink_library /Volumes/git/m1-sdk/lib/_builds/ios/Release${EFFECTIVE_PLATFORM_NAME}/libMach1DecodeCAPI.1.3.0.0.dylib /Volumes/git/m1-sdk/lib/_builds/ios/Release${EFFECTIVE_PLATFORM_NAME}/libMach1DecodeCAPI.1.3.0.0.dylib /Volumes/git/m1-sdk/lib/_builds/ios/Release${EFFECTIVE_PLATFORM_NAME}/libMach1DecodeCAPI.dylib
fi
if test "$CONFIGURATION" = "MinSizeRel"; then :
  cd /Volumes/git/m1-sdk/lib/_builds/ios
  /usr/local/Cellar/cmake/3.21.4/bin/cmake -E cmake_symlink_library /Volumes/git/m1-sdk/lib/_builds/ios/MinSizeRel${EFFECTIVE_PLATFORM_NAME}/libMach1DecodeCAPI.1.3.0.0.dylib /Volumes/git/m1-sdk/lib/_builds/ios/MinSizeRel${EFFECTIVE_PLATFORM_NAME}/libMach1DecodeCAPI.1.3.0.0.dylib /Volumes/git/m1-sdk/lib/_builds/ios/MinSizeRel${EFFECTIVE_PLATFORM_NAME}/libMach1DecodeCAPI.dylib
fi
if test "$CONFIGURATION" = "RelWithDebInfo"; then :
  cd /Volumes/git/m1-sdk/lib/_builds/ios
  /usr/local/Cellar/cmake/3.21.4/bin/cmake -E cmake_symlink_library /Volumes/git/m1-sdk/lib/_builds/ios/RelWithDebInfo${EFFECTIVE_PLATFORM_NAME}/libMach1DecodeCAPI.1.3.0.0.dylib /Volumes/git/m1-sdk/lib/_builds/ios/RelWithDebInfo${EFFECTIVE_PLATFORM_NAME}/libMach1DecodeCAPI.1.3.0.0.dylib /Volumes/git/m1-sdk/lib/_builds/ios/RelWithDebInfo${EFFECTIVE_PLATFORM_NAME}/libMach1DecodeCAPI.dylib
fi

