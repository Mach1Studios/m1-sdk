echo "### CLEANING OLD TESTS ###"
if exist _install del /f /q _install
if exist _builds del /f /q _builds

echo "### BUILD WIN ###"
if defined cmake_generator (
    cmake . -B_builds/windows-x86_64 -G "%cmake_generator%" -A x64 -DCMAKE_INSTALL_PREFIX=%cd%/_install/windows-x86_64
) else (
    cmake . -B_builds/windows-x86_64 -A x64 -DCMAKE_INSTALL_PREFIX=%cd%/_install/windows-x86_64
)

cmake --build _builds/windows-x86_64 --target install

_install/windows-x86_64/bin/Mach1EncodeTests
_install/windows-x86_64/bin/Mach1DecodeTests
_install/windows-x86_64/bin/Mach1DecodePositionalTests
_install/windows-x86_64/bin/Mach1TranscodeTests
_install/windows-x86_64/bin/Mach1SpatialTests

echo "### RENDER CHECK ####"
echo "### 5.1_C -> M1Spatial ###"
if exist Mach1TranscodeRenderTest\build (
    rd /s /q Mach1TranscodeRenderTest\build
)
rm -rf Mach1TranscodeRenderTest/build
cd Mach1TranscodeRenderTest && mkdir build && cd build 
if defined cmake_generator (
    cmake .. -G "%cmake_generator%" -A x64 -DCMAKE_TOOLCHAIN_FILE="D:/git/vcpkg/scripts/buildsystems/vcpkg.cmake" -DVCPKG_TARGET_TRIPLET=x64-windows-static
) else (
    cmake .. -A x64 -DCMAKE_TOOLCHAIN_FILE="D:/git/vcpkg/scripts/buildsystems/vcpkg.cmake" -DVCPKG_TARGET_TRIPLET=x64-windows-static
)
cmake --build . 
./spatial-transcode-render -in-file "../m1-debug-shortpt-fiveone.wav" -in-fmt 5.1_C -out-file "../m1-debug-shortpt-fiveone_m1spatial-out.wav" -out-fmt M1Spatial-8 -out-file-chans 0 && ./spatial-transcode-render -formats

endlocal