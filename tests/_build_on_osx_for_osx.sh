echo "### CLEANING OLD TESTS ###"
rm -rf ./_builds

echo "### BUILD macOS ###"
# Use Unix Makefiles so CMake finds CC/CXX from PATH without invoking xcodebuild (avoids
# IDESimulatorFoundation/DVTDownloads plugin failures when Xcode generator is broken).
cmake . -B_builds/xcode -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug -DM1S_BUILD_TESTS=ON -DM1S_BUILD_EXAMPLES=OFF -DM1S_BUILD_SIGNAL_SUITE=OFF
cmake --build _builds/xcode

# Single-config generator: test executables are under tests/ (no Debug/ subdir)
# TODO: add install command in cmake so that we can predict where these exes exist
_builds/xcode/tests/Mach1EncodeTests || { echo 'Mach1Encode API test failed...' ; exit 1; }
_builds/xcode/tests/Mach1DecodeTests || { echo 'Mach1Decode API test failed...' ; exit 1; }
_builds/xcode/tests/Mach1DecodePositionalTests || { echo 'Mach1DecodePositional API test failed...' ; exit 1; }
_builds/xcode/tests/Mach1TranscodeTests || { echo 'Mach1Transcode API test failed...' ; exit 1; }

# TODO: Check if file was built first!
# echo "### RENDER CHECK ####"
# echo "### 5.1_C -> M1Spatial ###"
# _builds/xcode/tests/Debug/spatial-transcode-render -in-file "../m1-debug-shortpt-fiveone.wav" -in-fmt 5.1_C -out-file "../m1-debug-shortpt-fiveone_m1spatial-out.wav" -out-fmt M1Spatial-8 -out-file-chans 0 || { echo 'Render test failed...' ; exit 1; }
# echo "### FINISHED TESTING ###"
