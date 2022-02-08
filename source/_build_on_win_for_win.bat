python ../../polly/bin/polly.py --clear --install --config Release --toolchain vs-14-2015
python ../../polly/bin/polly.py --clear --install --config Release --toolchain vs-14-2015-win64

python ../../polly/bin/polly.py --clear --install --config Release --toolchain vs-15-2017
python ../../polly/bin/polly.py --clear --install --config Release --toolchain vs-15-2017-win64

python ../../polly/bin/polly.py --clear --install --config Release --toolchain vs-16-2019
python ../../polly/bin/polly.py --clear --install --config Release --toolchain vs-16-2019-win64

cd %~dp0

xcopy _install\vs-14-2015\lib ..\mach1spatial-libs\vs-14-2015\x86 /Y  /I
xcopy _install\vs-14-2015-win64\lib ..\mach1spatial-libs\vs-14-2015\x86_64 /Y  /I

xcopy _install\vs-15-2017\lib ..\mach1spatial-libs\vs-15-2017\x86 /Y  /I
xcopy _install\vs-15-2017-win64\lib ..\mach1spatial-libs\vs-15-2017\x86_64 /Y  /I

xcopy _install\vs-16-2019\lib ..\mach1spatial-libs\windows-x86 /Y  /I
xcopy _install\vs-16-2019-win64\lib ..\mach1spatial-libs\windows-x86_64 /Y  /I

xcopy _install\vs-16-2019\libUnity ..\examples\mach1spatial-c\Unity\Unity-Mach1SpatialAPI\M1UnityDecodeTest\Assets\Mach1\Plugins\Windows\x86 /Y  /I
xcopy _install\vs-16-2019-win64\libUnity ..\examples\mach1spatial-c\Unity\Unity-Mach1SpatialAPI\M1UnityDecodeTest\Assets\Mach1\Plugins\Windows\x86_64 /Y  /I

xcopy _install\vs-16-2019\libUnity ..\examples\mach1spatial-c\Unity\Unity-Mach1SpatialAPI\M1DSPGraph\Assets\Mach1\Plugins\Windows\x86 /Y  /I
xcopy _install\vs-16-2019-win64\libUnity ..\examples\mach1spatial-c\Unity\Unity-Mach1SpatialAPI\M1DSPGraph\Assets\Mach1\Plugins\Windows\x86_64 /Y  /I

xcopy _install\vs-16-2019\lib "..\examples\mach1spatial-c\Unreal Engine\UE-Mach1SpatialAPI\Mach1DecodePlugin\SourcePlugin\Mach1DecodePlugin\ThirdParty\Mach1\bin\Win32" /Y  /I
xcopy _install\vs-16-2019\libUnity "..\examples\mach1spatial-c\Unreal Engine\UE-Mach1SpatialAPI\Mach1DecodePlugin\SourcePlugin\Mach1DecodePlugin\ThirdParty\Mach1\bin\Win32" /Y  /I
xcopy _install\vs-16-2019-win64\lib "..\examples\mach1spatial-c\Unreal Engine\UE-Mach1SpatialAPI\Mach1DecodePlugin\SourcePlugin\Mach1DecodePlugin\ThirdParty\Mach1\bin\Win64" /Y  /I
xcopy _install\vs-16-2019-win64\libUnity "..\examples\mach1spatial-c\Unreal Engine\UE-Mach1SpatialAPI\Mach1DecodePlugin\SourcePlugin\Mach1DecodePlugin\ThirdParty\Mach1\bin\Win64" /Y  /I

xcopy _install\vs-16-2019\lib ..\examples\mach1spatial-c\openframeworks\ofxMach1\libs\libmach1\lib\vs\x86 /Y  /I
xcopy _install\vs-16-2019-win64\lib ..\examples\mach1spatial-c\openframeworks\ofxMach1\libs\libmach1\lib\vs\x64 /Y  /I

pause
