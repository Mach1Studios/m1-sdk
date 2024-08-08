echo "### CLEANING OLD TESTS ###"
if exist _builds del /f /q _builds

echo "### BUILD WIN ###"
if defined cmake_generator (
    cmake . -B_builds/windows-x86_64 -G "%cmake_generator%" -A x64
) else (
    cmake . -B_builds/windows-x86_64 -A x64
)

cmake --build _builds/windows-x86_64

_builds/windows-x86_64/tests/Debug/Mach1EncodeTests
_builds/windows-x86_64/tests/Debug/Mach1DecodeTests
_builds/windows-x86_64/tests/Debug/Mach1DecodePositionalTests
_builds/windows-x86_64/tests/Debug/Mach1TranscodeTests
_builds/windows-x86_64/tests/Debug/Mach1SpatialTests

rem echo "### RENDER CHECK ####"
rem echo "### 5.1_C -> M1Spatial ###"
rem if exist Mach1TranscodeRenderTest\build (
rem     rd /s /q Mach1TranscodeRenderTest\build
rem )
rem rm -rf Mach1TranscodeRenderTest/build
rem cd Mach1TranscodeRenderTest && mkdir build && cd build 
rem if defined cmake_generator (
rem     cmake .. -G "%cmake_generator%" -A x64 -DCMAKE_TOOLCHAIN_FILE="D:/git/vcpkg/scripts/buildsystems/vcpkg.cmake" -DVCPKG_TARGET_TRIPLET=x64-windows-static
rem ) else (
rem     cmake .. -A x64 -DCMAKE_TOOLCHAIN_FILE="D:/git/vcpkg/scripts/buildsystems/vcpkg.cmake" -DVCPKG_TARGET_TRIPLET=x64-windows-static
rem )
rem cmake --build . 
rem ./spatial-transcode-render -in-file "../m1-debug-shortpt-fiveone.wav" -in-fmt 5.1_C -out-file "../m1-debug-shortpt-fiveone_m1spatial-out.wav" -out-fmt M1Spatial-8 -out-file-chans 0

echo "### FINISHED TESTING ###"
endlocal