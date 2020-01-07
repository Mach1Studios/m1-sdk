#!/bin/bash

# 
# Mach1 INTERNAL
# Properly copy and propogate all license files to this repo and to the release repo
# 
# Please complete the following first:
#  - Update the license version number
#  - Export a pdf named with version
#  - Create a plain text version and replace LICENSE.txt
#

cp LICENSE.txt ../binaries/LICENSE.txt
cp LICENSE.txt ../examples/mach1spatial-c/android/mach1-decode-example/LICENSE.txt
cp LICENSE.txt ../examples/mach1spatial-c/android/mach1-example/LICENSE.txt

#TODO rest of attributions
cp LICENSE.txt ../examples/mach1spatial-c/Unity/M1UnityDecodeTest/Assets/Mach1/license/LICENSE.txt
cp LICENSE.txt ../examples/mach1spatial-c/Unity/M1UnityDecodeTest/Assets/Mach1/Plugins/LICENSE.txt

#TODO rest of attributions
cp LICENSE.txt ../examples/mach1spatial-c/Unity/DSPGraph/Assets/Mach1/license/LICENSE.txt
cp LICENSE.txt ../examples/mach1spatial-c/Unity/DSPGraph/Assets/Mach1/Plugins/LICENSE.txt

#TODO rest of attributions
cp LICENSE.txt ../examples/mach1spatial-c/Unreal\ Engine/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/Resources/license/LICENSE.txt

cp LICENSE.txt ../examples/mach1spatial-web/js/LICENSE.txt



# copy to release places
# 
# copy to website and s3