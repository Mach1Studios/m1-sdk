#!/bin/bash

# 
# Mach1 INTERNAL
# Properly copy and propogate all license files to this repo and to the release repo
# 
# Setup: 
#  - Install Libreoffice
#  - `brew install unoconvert libreoffice`
#  - `pip install psd-tools`
#
# Please complete the following first:
#  - Update the license version number
#

# Generate files
python process_summary.py
unoconv -f pdf Mach1SpatialSDK-OpenUseLicense.docx
soffice --headless --convert-to "txt:Text (encoded):UTF8" Mach1SpatialSDK-OpenUseLicense.docx
cat Mach1-LicenseSummary.txt Mach1SpatialSDK-OpenUseLicense.txt > _Mach1SpatialSDK-OpenUseLicense.txt
mv _Mach1SpatialSDK-OpenUseLicense.txt Mach1SpatialSDK-OpenUseLicense.txt
mv Mach1SpatialSDK-OpenUseLicense.txt LICENSE.txt
soffice --headless --convert-to "html:XHTML Writer File:UTF8" --convert-images-to "jpg" Mach1SpatialSDK-OpenUseLicense.docx 
mv Mach1SpatialSDK-OpenUseLicense.html LICENSE.html

# copy to repo places
cp LICENSE.txt ../LICENSE.txt
cp LICENSE.txt ../examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/LICENSE.txt
cp LICENSE.txt ../examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI/LICENSE.txt

cp LICENSE.txt ../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecode/Assets/Mach1/license/LICENSE.txt
cp Mach1SpatialSDK-OpenUseLicense.pdf ../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecode/Assets/Mach1/license/Mach1SpatialSDK-OpenUseLicense.pdf
cp -r attribution ../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecode/Assets/Mach1/license/
cp LICENSE.txt ../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1UnityDecode/Assets/Mach1/Plugins/LICENSE.txt

cp LICENSE.txt ../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/license/LICENSE.txt
cp Mach1SpatialSDK-OpenUseLicense.pdf ../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/license/Mach1SpatialSDK-OpenUseLicense.pdf
cp -r attribution ../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/license/
cp LICENSE.txt ../examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI/M1DSPGraph/Assets/Mach1/Plugins/LICENSE.txt

cp LICENSE.txt ../examples/mach1spatial-c/Unreal\ Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/Resources/license/LICENSE.txt
cp Mach1SpatialSDK-OpenUseLicense.pdf ../examples/mach1spatial-c/Unreal\ Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/Resources/license/Mach1SpatialSDK-OpenUseLicense.pdf
cp -r attribution ../examples/mach1spatial-c/Unreal\ Engine/UE-Mach1SpatialAPI/Mach1DecodePlugin/Resources/license/

cp LICENSE.txt ../examples/mach1spatial-web/js/LICENSE.txt
cp LICENSE.txt ../examples/mach1spatial-web/m1-web-spatialaudioplayer/js/LICENSE.txt

# copy to website and s3
rm "Mach1SpatialSDK-License.zip"
zip Mach1SpatialSDK-License.zip -r . -x '*.docx' -x 'Mach1-LicenseSummary.png' -x '*.DS_Store' -x '*.sh' -x '*.html' -x 'Mach1-LicenseDiagram_nobug.png' -x '.gitignore'

aws s3 cp "LICENSE.txt" "s3://mach1-public/assets/licenses/LICENSE.txt" --cache-control 'max-age 0,no-cache,no-store,must-revalidate' --metadata-directive REPLACE --profile mach1
aws s3 cp "LICENSE.html" "s3://mach1-public/assets/licenses/LICENSE.html" --cache-control 'max-age 0,no-cache,no-store,must-revalidate' --metadata-directive REPLACE --profile mach1
aws s3 cp "Mach1SpatialSDK-OpenUseLicense.pdf" "s3://mach1-public/assets/licenses/Mach1SpatialSDK-OpenUseLicense.pdf" --cache-control 'max-age 0,no-cache,no-store,must-revalidate' --metadata-directive REPLACE --profile mach1
aws s3 cp "Mach1-LicenseSummary.png" "s3://mach1-public/assets/licenses/Mach1-LicenseSummary.png" --cache-control 'max-age 0,no-cache,no-store,must-revalidate' --metadata-directive REPLACE --profile mach1
aws s3 cp "Mach1-LicenseSummary-wbg.png" "s3://mach1-public/assets/licenses/Mach1-LicenseSummary-wbg.png" --cache-control 'max-age 0,no-cache,no-store,must-revalidate' --metadata-directive REPLACE --profile mach1
aws s3 cp "attribution/Mach1-EndUserLicenseAgreement-General.pdf" "s3://mach1-public/assets/licenses/Mach1-EndUserLicenseAgreement-General.pdf" --cache-control 'max-age 0,no-cache,no-store,must-revalidate' --metadata-directive REPLACE --profile mach1
aws s3 cp "Mach1SpatialSDK-License.zip" "s3://mach1-releases/license/Mach1SpatialSDK-License.zip" --cache-control 'max-age 0,no-cache,no-store,must-revalidate' --metadata-directive REPLACE --profile mach1
