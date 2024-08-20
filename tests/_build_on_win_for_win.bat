echo "### CLEANING OLD TESTS ###"
if exist _builds del /f /q _builds

echo "### BUILD WIN ###"
if defined cmake_generator (
    cmake . -B_builds/windows-x86_64 -G "%cmake_generator%" -A x64 -DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%/scripts/buildsystems/vcpkg.cmake"
) else (
    cmake . -B_builds/windows-x86_64 -A x64 -DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%/scripts/buildsystems/vcpkg.cmake"
)
cmake --build _builds/windows-x86_64 --config "Debug"

start /WAIT /B _builds/windows-x86_64/tests/Debug/Mach1EncodeTests
start /WAIT /B _builds/windows-x86_64/tests/Debug/Mach1DecodeTests
start /WAIT /B _builds/windows-x86_64/tests/Debug/Mach1DecodePositionalTests
start /WAIT /B _builds/windows-x86_64/tests/Debug/Mach1TranscodeTests
start /WAIT /B _builds/windows-x86_64/tests/Debug/Mach1SpatialTests

echo "### FINISHED TESTING ###"
endlocal
