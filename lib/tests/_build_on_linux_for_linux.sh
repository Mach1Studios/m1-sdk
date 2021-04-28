echo "### BUILD Linux ###"
~/git/polly/bin/polly --clear --install --config Release --toolchain gcc

echo "### RUN TESTS ###"
_install/gcc/bin/Mach1EncodeTest
_install/gcc/bin/Mach1DecodeTest
_install/gcc/bin/Mach1DecodePositionalTest