# Migrations
Currently the Mach1 Spatial SDK has gone through some revisions to improve on its usability based on usage and feedback. This document is to help describe any breaking changes to help anyone migrating from an older version of the repo.

#### Stable legacy versions:
[Version 2](https://github.com/Mach1Studios/m1-sdk)
[Version 1](https://github.com/Mach1Studios/m1-sdk/tree/1.2)

## Migration to 3.0+
Version 3 removes some old deprecated variables and focuses on clearer and more concise function/variable terminology. Major changes for each API as follows: 
- All APIs: Use specific vector based `Mach1Spatial-n` formats where `n` is the number of channels in the configuration of that format, typically derived or inspired by from platonic solid shapes.
- Mach1Encode API: Azimuth, Elevation and Diverge are the only terms used for setting a source sound's direction and signal distribution. 
- Mach1Decode API: Verbose description of orientation data is required within function name.
- Mach1Transcode API: Swapped inputs/outputs of matrix columns/rows, inputs are now rows to allow inline description of an input channel's "location" via Mach1Encode API style Azimuth/Elevation/Diverge as well as the coefficients directly to the output channels. 