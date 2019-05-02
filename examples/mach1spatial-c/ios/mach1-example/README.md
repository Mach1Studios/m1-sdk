### Mach1 Example: All Libraries
This example showcases (n) number of Mach1Encode classes streaming directly to a single Mach1Decode class for an inline spatial soundfield mix, this can be expanded so that (n) number of Mach1Decodes can be used for different location positionally if expanding to Mach1DecodePositional. This could also be used to write the resulting streams into a rendered Mach1Spatial multichannel file instead of streaming and playing directly to hardware output via the Mach1Decode used as a sum.

`mach1Encodes[n] -> sum all streams -> mach1Decode -> hardware output`
`mach1Encodes[n] -> sum all streams of location -> mach1DecodePositional[n] -> sum all streams of outputs -> hardware output`

### How to Use

- Click a button to select a new sound file
- Tap on soundfield grid to place the sound
- Tap and drag selected sound to any rotation/diverge around center
- Tap and hold to remove sound
- Tap to select to change Height/Volume paramters of that selected sound