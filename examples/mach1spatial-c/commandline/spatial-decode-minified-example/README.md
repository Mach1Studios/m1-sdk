# Mach1Decode C++ Example

### IDE Project Files
 - `spatial-decode-example.xcodeproj` Xcode project file
 - `spatial-decode-example.vcxproj` Visual Studio project file

### Makefile Options
 - `build`: generic build instructions for macos & linux
 - `build-beaglebone`: build instructions for Beaglebone Black
 - `build-arm-cortexm0`: build instrcutions for cortexm0 (soft float, cxx14)
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