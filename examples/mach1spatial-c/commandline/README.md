### Mach1 C++ Demonstrations

### Introduction
These examples are for simple implementation and testing of returned results from Mach1Spatial libs. There is no audio implementation on them and are designed for easy evaluation.

### Current Demos:
 - spatial-decode-example: commandline example for testing and returning Mach1 Spatial Decode coefficients with different orientation inputs
 - spatial-decodepositional-example: commandline example for testing and returning Mach1 Spatial DecodePositional coefficients with different orientation and positional inputs
 - spatial-encode-example: commandline example for testing and returning Mach1 Spatial coefficients with different input settings and configurations

#### Build Instructions
 - Open each xcode project file, run and input the appropriate key commands into the console

### Common Key Commands

## Orientation & Position Key Commands
 - A: orientation: yaw -- left
 - D: orientation: yaw ++ right
 - W: orientation: pitch ++ up
 - S: orientation: pitch -- down
 - Z: orientation: roll -- left
 - X: orientation: roll ++ right
 - I: position: z ++ forward
 - K: position: z -- backward
 - J: position: x -- strafe left
 - L: position: x ++ strafe right
 - Y: position: y ++ move up
 - H: position: y -- move down

## Encode Key Commands
 - I: input mode: changes the input channel type (mono, stereo, quad, lcrs, a-format, b-format acn, b-format fuma)
 - O: output mode: changes the output mach1 spatial channel type (mach1 horizon, mach1 spatial)
 - E: isotropic: enable/disable isotropic encoding
 - T: auto orbit: enable/disable stereo auto orbit
 - C: stereo rotation: if auto orbit is disabled manually rotate stereo inputs: ++ angle
 - V: stereo rotation: if auto orbit is disabled manually rotate stereo inputs: -- angle
 - B: stereo spread: diverge between stereo input channels ++
 - N: stereo spread: diverge between stereo input channels --