echo "### CLEANING OLD TESTS ###"
if exist _builds del /f /q _builds

echo "### BUILD WIN ###"
if defined cmake_generator (
    cmake . -B_builds/windows-x86_64 -G "%cmake_generator%" -A x64 -DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%/scripts/buildsystems/vcpkg.cmake" -DM1S_BUILD_TESTS=ON -DM1S_BUILD_EXAMPLES=OFF -DM1S_BUILD_SIGNAL_SUITE=OFF
) else (
    cmake . -B_builds/windows-x86_64 -A x64 -DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%/scripts/buildsystems/vcpkg.cmake" -DM1S_BUILD_TESTS=ON -DM1S_BUILD_EXAMPLES=OFF -DM1S_BUILD_SIGNAL_SUITE=OFF
)
cmake --build _builds/windows-x86_64 --config "Debug"

start /WAIT /B _builds/windows-x86_64/tests/Debug/Mach1EncodeTests
start /WAIT /B _builds/windows-x86_64/tests/Debug/Mach1DecodeTests
start /WAIT /B _builds/windows-x86_64/tests/Debug/Mach1DecodePositionalTests
start /WAIT /B _builds/windows-x86_64/tests/Debug/Mach1TranscodeTests

echo "### FINISHED TESTING ###"
endlocal
