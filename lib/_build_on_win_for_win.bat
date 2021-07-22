python ../../polly/bin/polly.py --clear --install --config Release --toolchain vs-16-2019
python ../../polly/bin/polly.py --clear --install --config Release --toolchain vs-16-2019-win64

cd %~dp0

xcopy _install\vs-16-2019\lib ..\binaries\windows-x86 /Y  /I
xcopy _install\vs-16-2019-win64\lib ..\binaries\windows-x86_64 /Y  /I

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
