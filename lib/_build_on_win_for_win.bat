python ../../polly-master/bin/polly.py --clear --install --config Release --toolchain vs-15-2017
python ../../polly-master/bin/polly.py --clear --install --config Release --toolchain vs-15-2017-win64

del /f /s /q _win 1>nul
rmdir /s /q _win

cd %~dp0

xcopy _install\vs-15-2017\lib ..\binaries\windows-x86\x86 /Y
xcopy _install\vs-15-2017-win64\lib ..\binaries\windows-x86_64\x64 /Y

xcopy _install\vs-15-2017\libUnity ..\examples\mach1spatial-c\Unity\M1UnityDecodeTest\Assets\Mach1\Plugins\Windows\x86 /Y
xcopy _install\vs-15-2017-win64\libUnity ..\examples\mach1spatial-c\Unity\M1UnityDecodeTest\Assets\Mach1\Plugins\Windows\x86_64 /Y

xcopy _install\vs-15-2017\libUnity ..\examples\mach1spatial-c\Unity\DSPGraph\Assets\Mach1\Plugins\Windows\x86 /Y
xcopy _install\vs-15-2017-win64\libUnity ..\examples\mach1spatial-c\Unity\DSPGraph\Assets\Mach1\Plugins\Windows\x86_64 /Y

xcopy _install\vs-15-2017\libUnity "..\examples\mach1spatial-c\Unreal Engine\Mach1DecodePlugin\Source\Mach1DecodePlugin\ThirdParty\Mach1\bin\Win32" /Y
xcopy _install\vs-15-2017-win64\libUnity "..\examples\mach1spatial-c\Unreal Engine\Mach1DecodePlugin\Source\Mach1DecodePlugin\ThirdParty\Mach1\bin\Win64" /Y

xcopy _install\vs-15-2017\lib ..\examples\mach1spatial-c\openframeworks\ofxMach1\libs\libmach1\lib\vs\x86 /Y
xcopy _install\vs-15-2017-win64\lib ..\examples\mach1spatial-c\openframeworks\ofxMach1\libs\libmach1\lib\vs\x64 /Y

pause
