# Mach1Decode Minified C++ Example

## Description
This example and the linked Mach1DecodeMinifiedCAPI.a are only for use on very small footprint CPUs, it has stripped out several protective features and has a limited usage design.

### IDE Project Files
 - `spatial-decode-example.xcodeproj` Xcode project file

### Makefile Options
 - `build`: generic build instructions for macos & linux
 - `build-beaglebone`: build instructions for Beaglebone Black
 - `build-arm-cortexm0`: build instrcutions for cortexm0 (soft float, cxx14)
 - `build-armhf-neonvfpv4`: build instrcutions for arm neon vfpv4 (hard float, cxx11)
 - `build-rpi2`: build instructions for Raspberry Pi 2 (cxx11)
 - `build-rpi3`: build instructions for Raspberry Pi 3 (cxx11)

## Common Key Commands

### Orientation & Position Key Commands
 - A: orientation: yaw -- left
 - D: orientation: yaw ++ right
 - W: orientation: pitch ++ up
 - S: orientation: pitch -- down
 - Z: orientation: roll -- left
 - X: orientation: roll ++ right