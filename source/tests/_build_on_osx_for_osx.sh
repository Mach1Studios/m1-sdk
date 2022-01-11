echo "### BUILD macOS ###"
polly --clear --install --config Release --toolchain xcode

echo "### RUN TESTS ###"
_install/xcode/bin/Mach1EncodeTest
_install/xcode/bin/Mach1DecodeTest
_install/xcode/bin/Mach1DecodePositionalTest