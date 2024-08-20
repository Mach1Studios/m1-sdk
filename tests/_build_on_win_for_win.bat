echo "### CLEANING OLD TESTS ###"
if exist _builds del /f /q _builds

echo "### BUILD WIN ###"
if defined cmake_generator (
    cmake . -B_builds/windows-x86_64 -G "%cmake_generator%" -A x64 -DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%/scripts/buildsystems/vcpkg.cmake"
) else (
    cmake . -B_builds/windows-x86_64 -A x64 -DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%/scripts/buildsystems/vcpkg.cmake"
)
cmake --build _builds/windows-x86_64 --config "Debug"

_builds/windows-x86_64/tests/Debug/Mach1EncodeTests
_builds/windows-x86_64/tests/Debug/Mach1DecodeTests
_builds/windows-x86_64/tests/Debug/Mach1DecodePositionalTests
_builds/windows-x86_64/tests/Debug/Mach1TranscodeTests
_builds/windows-x86_64/tests/Debug/Mach1SpatialTests

echo "### FINISHED TESTING ###"
endlocal
