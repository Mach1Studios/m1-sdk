python ../../polly-master/bin/polly.py --clear --install --config Release --toolchain vs-15-2017
python ../../polly-master/bin/polly.py --clear --install --config Release --toolchain vs-15-2017-win64

cd %~dp0

xcopy _install\vs-15-2017\lib ..\binaries\windows-x86 /Y
xcopy _install\vs-15-2017-win64\lib ..\binaries\windows-x86_64 /Y

xcopy _install\vs-15-2017\libUnity ..\examples\mach1spatial-c\Unity\M1UnityDecodeTest\Assets\Mach1\Plugins\Windows\x86 /Y
xcopy _install\vs-15-2017-win64\libUnity ..\examples\mach1spatial-c\Unity\M1UnityDecodeTest\Assets\Mach1\Plugins\Windows\x86_64 /Y

xcopy _install\vs-15-2017\libUnity ..\examples\mach1spatial-c\Unity\DSPGraph\Assets\Mach1\Plugins\Windows\x86 /Y
xcopy _install\vs-15-2017-win64\libUnity ..\examples\mach1spatial-c\Unity\DSPGraph\Assets\Mach1\Plugins\Windows\x86_64 /Y

xcopy _install\vs-15-2017\libUnity "..\examples\mach1spatial-c\Unreal Engine\Mach1DecodePlugin\SourcePlugin\Mach1DecodePlugin\ThirdParty\Mach1\bin\Win32" /Y
xcopy _install\vs-15-2017-win64\libUnity "..\examples\mach1spatial-c\Unreal Engine\Mach1DecodePlugin\SourcePlugin\Mach1DecodePlugin\ThirdParty\Mach1\bin\Win64" /Y

xcopy _install\vs-15-2017\lib ..\examples\mach1spatial-c\openframeworks\ofxMach1\libs\libmach1\lib\vs\x86 /Y
xcopy _install\vs-15-2017-win64\lib ..\examples\mach1spatial-c\openframeworks\ofxMach1\libs\libmach1\lib\vs\x64 /Y

pause
