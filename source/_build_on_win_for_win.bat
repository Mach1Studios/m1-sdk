setlocal enableextensions

python ../../polly/bin/polly.py --clear --install --config Release --toolchain vs-14-2015
python ../../polly/bin/polly.py --clear --install --config Release --toolchain vs-14-2015-win64

mkdir ..\mach1spatial-libs\vs-15-2017-x86\lib\Static\MD\Debug
mkdir ..\mach1spatial-libs\vs-15-2017-x86_64\lib\Static\MD\Debug
mkdir ..\mach1spatial-libs\vs-15-2017-x86\lib\Static\MD\Release
mkdir ..\mach1spatial-libs\vs-15-2017-x86_64\lib\Static\MD\Release
mkdir ..\mach1spatial-libs\vs-15-2017-x86\lib\Static\MT\Debug
mkdir ..\mach1spatial-libs\vs-15-2017-x86_64\lib\Static\MT\Debug
mkdir ..\mach1spatial-libs\vs-15-2017-x86\lib\Static\MT\Release
mkdir ..\mach1spatial-libs\vs-15-2017-x86_64\lib\Static\MT\Release
mkdir ..\mach1spatial-libs\vs-15-2017-x86\lib\Dynamic\Debug
mkdir ..\mach1spatial-libs\vs-15-2017-x86_64\lib\Dynamic\Debug
mkdir ..\mach1spatial-libs\vs-15-2017-x86\lib\Dynamic\Release
mkdir ..\mach1spatial-libs\vs-15-2017-x86_64\lib\Dynamic\Release

python ../../polly/bin/polly.py --clear --install --config Release --toolchain vs-15-2017
xcopy _install\vs-15-2017\lib ..\mach1spatial-libs\vs-15-2017-x86\lib\Static\MD\Release\*.lib /Y  /I
xcopy _install\vs-15-2017-win64\lib-shared ..\mach1spatial-libs\vs-15-2017-x86\lib\Dynamic\Release\*.dll /Y  /I
xcopy _install\vs-15-2017\lib ..\mach1spatial-libs\windows-x86 /Y  /I
xcopy _install\vs-15-2017\lib-shared ..\examples\mach1spatial-c\Unity\Unity-Mach1SpatialAPI\M1UnityDecodeTest\Assets\Mach1\Plugins\Windows\x86 /Y  /I
xcopy _install\vs-15-2017\lib-shared ..\examples\mach1spatial-c\Unity\Unity-Mach1SpatialAPI\M1DSPGraph\Assets\Mach1\Plugins\Windows\x86 /Y  /I
xcopy _install\vs-15-2017\lib "..\examples\mach1spatial-c\Unreal Engine\UE-Mach1SpatialAPI\Mach1DecodePlugin\SourcePlugin\Mach1DecodePlugin\ThirdParty\Mach1\bin\Win32" /Y  /I
xcopy _install\vs-15-2017\lib-shared "..\examples\mach1spatial-c\Unreal Engine\UE-Mach1SpatialAPI\Mach1DecodePlugin\SourcePlugin\Mach1DecodePlugin\ThirdParty\Mach1\bin\Win32" /Y  /I
xcopy _install\vs-15-2017\lib ..\examples\mach1spatial-c\openframeworks\ofxMach1\libs\libmach1\lib\vs\x86 /Y  /I

python ../../polly/bin/polly.py --clear --install --config Debug --toolchain vs-15-2017
xcopy _install\vs-15-2017\lib ..\mach1spatial-libs\vs-15-2017-x86\lib\Static\MD\Debug\*.lib /Y  /I
xcopy _install\vs-15-2017\lib-shared ..\mach1spatial-libs\vs-15-2017-x86\lib\Dynamic\Debug\*.dll /Y  /I

python ../../polly/bin/polly.py --clear --install --config Release --toolchain vs-15-2017-win64
xcopy _install\vs-15-2017-win64\lib ..\mach1spatial-libs\vs-15-2017-x86_64\lib\Static\MD\Release\*.lib /Y  /I
xcopy _install\vs-15-2017-win64\lib-shared ..\mach1spatial-libs\vs-15-2017-x86_64\lib\Dynamic\Release\*.dll /Y  /I
xcopy _install\vs-15-2017-win64\lib ..\mach1spatial-libs\windows-x86_64 /Y  /I
xcopy _install\vs-15-2017-win64\lib-shared ..\examples\mach1spatial-c\Unity\Unity-Mach1SpatialAPI\M1UnityDecodeTest\Assets\Mach1\Plugins\Windows\x86_64 /Y  /I
xcopy _install\vs-15-2017-win64\lib-shared ..\examples\mach1spatial-c\Unity\Unity-Mach1SpatialAPI\M1DSPGraph\Assets\Mach1\Plugins\Windows\x86_64 /Y  /I
xcopy _install\vs-15-2017-win64\lib "..\examples\mach1spatial-c\Unreal Engine\UE-Mach1SpatialAPI\Mach1DecodePlugin\SourcePlugin\Mach1DecodePlugin\ThirdParty\Mach1\bin\Win64" /Y  /I
xcopy _install\vs-15-2017-win64\lib-shared "..\examples\mach1spatial-c\Unreal Engine\UE-Mach1SpatialAPI\Mach1DecodePlugin\SourcePlugin\Mach1DecodePlugin\ThirdParty\Mach1\bin\Win64" /Y  /I
xcopy _install\vs-15-2017-win64\lib ..\examples\mach1spatial-c\openframeworks\ofxMach1\libs\libmach1\lib\vs\x64 /Y  /I

python ../../polly/bin/polly.py --clear --install --config Debug --toolchain vs-15-2017-win64
xcopy _install\vs-15-2017-win64\lib ..\mach1spatial-libs\vs-15-2017-x86_64\lib\Static\MD\Debug\*.lib /Y  /I
xcopy _install\vs-15-2017-win64\lib-shared ..\mach1spatial-libs\vs-15-2017-x86_64\lib\Dynamic\Debug\*.dll /Y  /I

python ../../polly/bin/polly.py --clear --install --config Release --toolchain vs-15-2017-mt
xcopy _install\vs-15-2017-mt\lib ..\mach1spatial-libs\vs-15-2017-x86\lib\Static\MT\Release\*.lib /Y  /I

python ../../polly/bin/polly.py --clear --install --config Debug --toolchain vs-15-2017-mt
xcopy _install\vs-15-2017-mt\lib ..\mach1spatial-libs\vs-15-2017-x86\lib\Static\MT\Debug\*.lib /Y  /I

python ../../polly/bin/polly.py --clear --install --config Release --toolchain vs-15-2017-win64-mt
xcopy _install\vs-15-2017-win64-mt\lib ..\mach1spatial-libs\vs-15-2017-x86_64\lib\Static\MT\Release\*.lib /Y  /I

python ../../polly/bin/polly.py --clear --install --config Debug --toolchain vs-15-2017-win64-mt
xcopy _install\vs-15-2017-win64-mt\lib ..\mach1spatial-libs\vs-15-2017-x86_64\lib\Static\MT\Debug\*.lib /Y  /I

mkdir ..\mach1spatial-libs\vs-16-2019-x86\lib\Static\MD\Debug
mkdir ..\mach1spatial-libs\vs-16-2019-x86_64\lib\Static\MD\Debug
mkdir ..\mach1spatial-libs\vs-16-2019-x86\lib\Static\MD\Release
mkdir ..\mach1spatial-libs\vs-16-2019-x86_64\lib\Static\MD\Release
mkdir ..\mach1spatial-libs\vs-16-2019-x86\lib\Static\MT\Debug
mkdir ..\mach1spatial-libs\vs-16-2019-x86_64\lib\Static\MT\Debug
mkdir ..\mach1spatial-libs\vs-16-2019-x86\lib\Static\MT\Release
mkdir ..\mach1spatial-libs\vs-16-2019-x86_64\lib\Static\MT\Release
mkdir ..\mach1spatial-libs\vs-16-2019-x86\lib\Dynamic\Debug
mkdir ..\mach1spatial-libs\vs-16-2019-x86_64\lib\Dynamic\Debug
mkdir ..\mach1spatial-libs\vs-16-2019-x86\lib\Dynamic\Release
mkdir ..\mach1spatial-libs\vs-16-2019-x86_64\lib\Dynamic\Release

python ../../polly/bin/polly.py --clear --install --config Release --toolchain vs-16-2019
xcopy _install\vs-16-2019\lib ..\mach1spatial-libs\vs-16-2019-x86\lib\Static\MD\Release\*.lib /Y  /I
xcopy _install\vs-16-2019-win64\lib-shared ..\mach1spatial-libs\vs-16-2019-x86\lib\Dynamic\Release\*.dll /Y  /I

python ../../polly/bin/polly.py --clear --install --config Release --toolchain vs-16-2019-win64
xcopy _install\vs-16-2019-win64\lib ..\mach1spatial-libs\vs-16-2019-x86_64\lib\Static\MD\Release\*.lib /Y  /I
xcopy _install\vs-16-2019-win64\lib-shared ..\mach1spatial-libs\vs-16-2019-x86_64\lib\Dynamic\Release\*.dll /Y  /I

python ../../polly/bin/polly.py --clear --install --config Debug --toolchain vs-16-2019
xcopy _install\vs-16-2019\lib ..\mach1spatial-libs\vs-16-2019-x86\lib\Static\MD\Debug\*.lib /Y  /I
xcopy _install\vs-16-2019\lib-shared ..\mach1spatial-libs\vs-16-2019-x86\lib\Dynamic\Debug\*.dll /Y  /I

python ../../polly/bin/polly.py --clear --install --config Debug --toolchain vs-16-2019-win64
xcopy _install\vs-16-2019-win64\lib ..\mach1spatial-libs\vs-16-2019-x86_64\lib\Static\MD\Debug\*.lib /Y  /I
xcopy _install\vs-16-2019-win64\lib-shared ..\mach1spatial-libs\vs-16-2019-x86_64\lib\Dynamic\Debug\*.dll /Y  /I

python ../../polly/bin/polly.py --clear --install --config Release --toolchain vs-16-2019-mt
xcopy _install\vs-16-2019-mt\lib ..\mach1spatial-libs\vs-16-2019-x86\lib\Static\MT\Release\*.lib /Y  /I

python ../../polly/bin/polly.py --clear --install --config Debug --toolchain vs-16-2019-mt
xcopy _install\vs-16-2019-mt\lib ..\mach1spatial-libs\vs-16-2019-x86\lib\Static\MT\Debug\*.lib /Y  /I

python ../../polly/bin/polly.py --clear --install --config Release --toolchain vs-16-2019-win64-mt
xcopy _install\vs-16-2019-win64-mt\lib ..\mach1spatial-libs\vs-16-2019-x86_64\lib\Static\MT\Release\*.lib /Y  /I

python ../../polly/bin/polly.py --clear --install --config Debug --toolchain vs-16-2019-win64-mt
xcopy _install\vs-16-2019-win64-mt\lib ..\mach1spatial-libs\vs-16-2019-x86_64\lib\Static\MT\Debug\*.lib /Y  /I

cd %~dp0

xcopy _install\vs-14-2015\lib ..\mach1spatial-libs\vs-14-2015\x86 /Y  /I
xcopy _install\vs-14-2015-win64\lib ..\mach1spatial-libs\vs-14-2015\x86_64 /Y  /I

pause
