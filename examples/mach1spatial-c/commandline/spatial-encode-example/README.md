# Mach1Encode C++ Example

### IDE Project Files
 - `spatial-encode-example.xcodeproj` Xcode project file
 - `spatial-encode-example.vcxproj` Visual Studio project file

### Makefile Options
 - `build`: generic build instructions for macos & linux
 - `build-beaglebone`: build instructions for Beaglebone Black
 - `build-arm-cortexm0`: build instrcutions for cortexm0 (soft float, cxx14)
 - `build-armhf-neonvfpv4`: build instrcutions for arm neon vfpv4 (hard float, cxx11)
 - `build-rpi2`: build instructions for Raspberry Pi 2 (cxx11)
 - `build-rpi3`: build instructions for Raspberry Pi 3 (cxx11)

## Common Key Commands

### Encode Key Commands
 - I: input mode: changes the input channel type (mono, stereo, quad, lcrs, a-format, b-format acn, b-format fuma)
 - O: output mode: changes the output mach1 spatial channel type (mach1 horizon, mach1 spatial)
 - E: isotropic: enable/disable isotropic encoding
 - T: auto orbit: enable/disable stereo auto orbit
 - C: stereo rotation: if auto orbit is disabled manually rotate stereo inputs: ++ angle
 - V: stereo rotation: if auto orbit is disabled manually rotate stereo inputs: -- angle
 - B: stereo spread: diverge between stereo input channels ++
 - N: stereo spread: diverge between stereo input channels --