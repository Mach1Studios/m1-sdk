#!/bin/bash

# 
# Mach1 INTERNAL
# Properly copy and propogate all license files to this repo and to the release repo
# 
# Please complete the following first:
#  - Update the license version number
#  - Export a pdf named with version
#  - Create a plain text version and replace LICENSE.txt
#  - Create html version of LICENSE.txt
#

# copy to repo places
cp LICENSE.txt ../binaries/LICENSE.txt
cp LICENSE.txt ../examples/mach1spatial-c/android/mach1-decode-example/LICENSE.txt
cp LICENSE.txt ../examples/mach1spatial-c/android/mach1-example/LICENSE.txt
cp LICENSE.txt ../examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/LICENSE.txt

cp LICENSE.txt ../examples/mach1spatial-c/Unity/M1UnityDecodeTest/Assets/Mach1/license/LICENSE.txt
cp Mach1SpatialSDK-RoyaltyFreeLicense.pdf ../examples/mach1spatial-c/Unity/M1UnityDecodeTest/Assets/Mach1/license/Mach1SpatialSDK-RoyaltyFreeLicense.pdf
cp -r attribution ../examples/mach1spatial-c/Unity/M1UnityDecodeTest/Assets/Mach1/license/
cp LICENSE.txt ../examples/mach1spatial-c/Unity/M1UnityDecodeTest/Assets/Mach1/Plugins/LICENSE.txt

cp LICENSE.txt ../examples/mach1spatial-c/Unity/DSPGraph/Assets/Mach1/license/LICENSE.txt
cp Mach1SpatialSDK-RoyaltyFreeLicense.pdf ../examples/mach1spatial-c/Unity/DSPGraph/Assets/Mach1/license/Mach1SpatialSDK-RoyaltyFreeLicense.pdf
cp -r attribution ../examples/mach1spatial-c/Unity/DSPGraph/Assets/Mach1/license/
cp LICENSE.txt ../examples/mach1spatial-c/Unity/DSPGraph/Assets/Mach1/Plugins/LICENSE.txt

cp LICENSE.txt ../examples/mach1spatial-c/Unreal\ Engine/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/Resources/license/LICENSE.txt
cp Mach1SpatialSDK-RoyaltyFreeLicense.pdf ../examples/mach1spatial-c/Unreal\ Engine/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/Resources/license/Mach1SpatialSDK-RoyaltyFreeLicense.pdf
cp -r attribution ../examples/mach1spatial-c/Unreal\ Engine/Mach1DecodePlugin/SourcePlugin/Mach1DecodePlugin/Resources/license/

cp LICENSE.txt ../examples/mach1spatial-web/js/LICENSE.txt

# copy to release places
cp LICENSE.txt ../../m1-sdk-release/binaries/LICENSE.txt
cp LICENSE.txt ../../m1-sdk-release/examples/mach1spatial-c/android/mach1-decode-example/LICENSE.txt
cp LICENSE.txt ../../m1-sdk-release/examples/mach1spatial-c/android/mach1-example/LICENSE.txt

cp LICENSE.txt ../../m1-sdk-release/examples/mach1spatial-web/js/LICENSE.txt

# copy to website and s3
rm "Mach1SpatialSDK-License.zip"
zip Mach1SpatialSDK-License.zip -r . -x 'Mach1-LicenseDiagram.graffle/*' -x '*.docx' -x 'Mach1-LicenseDiagram.png' -x '*.DS_Store' -x '*.sh' -x '*.html'

aws s3 cp "LICENSE.txt" "s3://mach1-public/assets/license/LICENSE.txt" --profile mach1
aws s3 cp "LICENSE.html" "s3://mach1-public/assets/license/LICENSE.html" --profile mach1
aws s3 cp "Mach1SpatialSDK-RoyaltyFreeLicense.pdf" "s3://mach1-public/assets/license/Mach1SpatialSDK-RoyaltyFreeLicense.pdf" --profile mach1
aws s3 cp "Mach1SpatialSDK-License.zip" "s3://mach1-releases/license/Mach1SpatialSDK-License.zip" --profile mach1
