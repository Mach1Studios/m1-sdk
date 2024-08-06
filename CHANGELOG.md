# BREAKING CHANGES
Currently the Mach1 Spatial SDK has gone through some revisions to improve on its usability based on usage and feedback. This document is to help describe any breaking changes to help anyone migrating from an older version of the repo.

#### Stable legacy versions:
[Version 3](https://github.com/Mach1Studios/m1-sdk/tree/3.2)
[Version 2](https://github.com/Mach1Studios/m1-sdk/tree/2.1)
[Version 1](https://github.com/Mach1Studios/m1-sdk/tree/1.2)

## Breaking changes for migrating to 4.0+
Version 4 removes more deprecated functions and simplifies the APIs to be more modular.
- Mach1Decode API: Changed the order of the Mach1DecodeAlgoType so that they follow an incremental pattern (changes the default 0th index decode algo type).
- All APIs: Renamed Mach1Horizon_4 to Mach1Spatial_4 and all naming variations that used "Horizon" are replaced with "Spatial"
- All APIs: Changed the order for any `outputType` enum to ascend in order of 4->8->12->14 instead of 8->4->12->14 (4 is now the default first index for easier use).
- Mach1Transcode API: Changed the naming for the headers containing all the conversion matrices

## Breaking changes for migrating to 3.0
Version 3 removes some old deprecated variables and focuses on clearer and more concise function/variable terminology; and introduces CI/CD to make it easier in the future for community contributions to this project allowing the codebase to self test and distribute with the goal of becoming easier to maintain over time. Major changes for each API as follows: 
- All APIs: Use specific vector based `Mach1Spatial-n` formats where `n` is the number of channels in the configuration of that format, typically derived or inspired by from platonic solid shapes.
- Mach1Encode API: Azimuth, Elevation and Diverge are the only terms used for setting a source sound's direction and signal distribution. 
- Mach1Decode API: Verbose description of orientation data is required within function name.
- Mach1Transcode API: Swapped inputs/outputs of matrix columns/rows, inputs are now rows to allow inline description of an input channel's "location" via Mach1Encode API style Azimuth/Elevation/Diverge as well as the coefficients directly to the output channels. 