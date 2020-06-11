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
cp LICENSE.txt ../LICENSE.txt
cp LICENSE.txt ../binaries/LICENSE.txt
cp LICENSE.txt ../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/LICENSE.txt
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
cp LICENSE.txt ../../m1-sdk-release/LICENSE.txt
cp LICENSE.txt ../../m1-sdk-release/binaries/LICENSE.txt

cp LICENSE.txt ../../m1-sdk-release/examples/mach1spatial-web/js/LICENSE.txt

cp LICENSE.txt ../../m1-sdk-release/license/LICENSE.txt
cp Mach1SpatialSDK-RoyaltyFreeLicense.pdf ../../m1-sdk-release/license/Mach1SpatialSDK-RoyaltyFreeLicense.pdf
cp -r attribution ../../m1-sdk-release/license/

# copy to website and s3
rm "Mach1SpatialSDK-License.zip"
zip Mach1SpatialSDK-License.zip -r . -x 'Mach1-LicenseDiagram.graffle/*' -x '*.docx' -x 'Mach1-LicenseDiagram.png' -x '*.DS_Store' -x '*.sh' -x '*.html' -x 'Mach1-LicenseDiagram_nobug.png' -x '*.ai' -x 'attribution/Mach1-EndUserLicenseAgreement-General.docx' -x '.gitignore'

aws s3 cp "LICENSE.txt" "s3://mach1-public/assets/license/LICENSE.txt" --cache-control 'max-age 0,no-cache,no-store,must-revalidate' --metadata-directive REPLACE --profile mach1
aws s3 cp "LICENSE.html" "s3://mach1-public/assets/license/LICENSE.html" --cache-control 'max-age 0,no-cache,no-store,must-revalidate' --metadata-directive REPLACE --profile mach1
aws s3 cp "Mach1SpatialSDK-RoyaltyFreeLicense.pdf" "s3://mach1-public/assets/license/Mach1SpatialSDK-RoyaltyFreeLicense.pdf" --cache-control 'max-age 0,no-cache,no-store,must-revalidate' --metadata-directive REPLACE --profile mach1
aws s3 cp "Mach1-LicenseDiagram.png" "s3://mach1-public/assets/license/Mach1-LicenseDiagram.png" --cache-control 'max-age 0,no-cache,no-store,must-revalidate' --metadata-directive REPLACE --profile mach1
aws s3 cp "Mach1-LicenseDiagram-wbg.png" "s3://mach1-public/assets/license/Mach1-LicenseDiagram-wbg.png" --cache-control 'max-age 0,no-cache,no-store,must-revalidate' --metadata-directive REPLACE --profile mach1
aws s3 cp "attribution/Mach1-EndUserLicenseAgreement-General.pdf" "s3://mach1-public/assets/license/Mach1-EndUserLicenseAgreement-General.pdf" --cache-control 'max-age 0,no-cache,no-store,must-revalidate' --metadata-directive REPLACE --profile mach1
aws s3 cp "Mach1SpatialSDK-License.zip" "s3://mach1-releases/license/Mach1SpatialSDK-License.zip" --cache-control 'max-age 0,no-cache,no-store,must-revalidate' --metadata-directive REPLACE --profile mach1
