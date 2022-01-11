#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd /Volumes/git/m1-sdk/lib/_builds/xcode
  make -f /Volumes/git/m1-sdk/lib/_builds/xcode/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "Release"; then :
  cd /Volumes/git/m1-sdk/lib/_builds/xcode
  make -f /Volumes/git/m1-sdk/lib/_builds/xcode/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "MinSizeRel"; then :
  cd /Volumes/git/m1-sdk/lib/_builds/xcode
  make -f /Volumes/git/m1-sdk/lib/_builds/xcode/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "RelWithDebInfo"; then :
  cd /Volumes/git/m1-sdk/lib/_builds/xcode
  make -f /Volumes/git/m1-sdk/lib/_builds/xcode/CMakeScripts/ReRunCMake.make
fi

