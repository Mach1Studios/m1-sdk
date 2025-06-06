echo "### CLEANING OLD TESTS ###"
rm -rf ./_builds

echo "### BUILD macOS ###"
cmake . -B_builds/xcode -GXcode -DM1S_BUILD_TESTS=ON -DM1S_BUILD_EXAMPLES=OFF -DM1S_BUILD_SIGNAL_SUITE=OFF
cmake --build _builds/xcode --config "Debug"

# these commands are expected to execute if this script was run from the parent ../ directory
# TODO: add install command in cmake so that we can predict where these exes exist
_builds/xcode/tests/Debug/Mach1EncodeTests || { echo 'Mach1Encode API test failed...' ; exit 1; }
_builds/xcode/tests/Debug/Mach1DecodeTests || { echo 'Mach1Decode API test failed...' ; exit 1; }
_builds/xcode/tests/Debug/Mach1DecodePositionalTests || { echo 'Mach1DecodePositional API test failed...' ; exit 1; }
_builds/xcode/tests/Debug/Mach1TranscodeTests || { echo 'Mach1Transcode API test failed...' ; exit 1; }

# TODO: Check if file was built first!
# echo "### RENDER CHECK ####"
# echo "### 5.1_C -> M1Spatial ###"
# _builds/xcode/tests/Debug/spatial-transcode-render -in-file "../m1-debug-shortpt-fiveone.wav" -in-fmt 5.1_C -out-file "../m1-debug-shortpt-fiveone_m1spatial-out.wav" -out-fmt M1Spatial-8 -out-file-chans 0 || { echo 'Render test failed...' ; exit 1; }
# echo "### FINISHED TESTING ###"
