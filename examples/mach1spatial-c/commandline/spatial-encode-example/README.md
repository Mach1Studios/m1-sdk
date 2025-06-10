# Mach1Encode C++ Example

## CMake Setup
 - `cmake . -Bbuild`
 - `cmake --build build`

### Makefile Options
 - `build`: generic build instructions crossplatform via cmake (the above steps)

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
