<a href="http://dev.mach1.tech"><img src="https://mach1-public.s3.amazonaws.com/assets/logo_big_b_l.png"></a>

## MACH1 SPATIAL SDK ##
[![Slack Channel](https://img.shields.io/badge/Slack-Join-purple)](https://join.slack.com/t/spatialaudio/shared_invite/enQtNjk0ODE4NjQ4NjExLWQ5YWUyNWQ4NWEwMDEwZmJiNmI5MzBhYjM3OTE3NTYxYzdjZDE2YTlhZDI4OGY0ZjdkNmM1NzgxNjI5OGU4ZWE)

* [SUMMARY](#summary)
* [CONTRIBUTION](#contribution)
* [CONTACT](#contact)

### [SUMMARY](#summary) ###
Exposed in this directory are the transcode matrices used and compiled into the Mach1Transcode API library. We have split the transcode matrices into categories for ease of contribution: 
Matrices are declared here: 
 - [Mach1TranscodeConstants](https://github.com/Mach1Studios/m1-sdk/blob/master/public/Mach1TranscodeConstants.h)
Matrices are described here:
 - [Ambisonic transcode matrices](https://github.com/Mach1Studios/m1-sdk/blob/master/public/Mach1TranscodeAmbisonicFormats.h)
 - [Surround transcode matrices](https://github.com/Mach1Studios/m1-sdk/blob/master/public/Mach1TranscodeSurroundFormats.h)
 - [Mic Array transcode matrices](https://github.com/Mach1Studios/m1-sdk/blob/master/public/Mach1TranscodeMicArrayFormats.h)
 - [Vector transcode matrices](https://github.com/Mach1Studios/m1-sdk/blob/master/public/Mach1TranscodeVectorFormats.h)

#### Description of matrices ####
Each transcode matrix is described via the expected input to output relationship, with inputs and outputs explained per channel like so: 
 - Inputs are rows
 - Outputs are columns
```
I1 -> | I1 | I2 | I3 | I4 |
I2 -> | I1 | I2 | I3 | I4 |
I3 -> | I1 | I2 | I3 | I4 |
      ↓    ↓    ↓    ↓ 
      O1   O2   O3   O4
```
Example: 
```
{
	"M1Spatial", "ACNSN3D",
	{   

	}
},
```

#### Additional Surround Descriptions
The surround matrices use additional naming nomenclature to help keep things organized and understandable, the nomenclature is described as such: 
```
 Multichannel Format/Configuration Naming Conventions:
 - M or Music			   = `Music Mix` (Channels are spaced out evenly throughout the horizontal soundfield)
 - C or Cinema			   = `Cinema Mix` (Channels are more focused on the front)
 - S or SideSurround 	= `Side Surround Mix` (Surround channels are oriented more to the sides instead of rear (+-110 azimuth instead of +-135))
 - R or RearSurround 	= `Rear Surround Mix` (Surround channels are oriented more to the rears instead of sides (+-154 azimuth instead of +-135))
 - SIM or Simulated		= `Simulated Room Mix` (Lessens the divergence of virtual speakers to quickly simulate hearing front/back soundfield within a real world listening environment)

Channel Order Variants:
 - Default / Film / C|24  	= L, C, R, Ls, Rs, LFE ...
 - SMPTE				      	= L, R, C, LFE, Ls, Rs ...
 - Dts					     	= L, R, Ls, Rs, C, LFE ...
```
#### Describing Each Channel
Currently there are two ways to describe how an input channel distributes to the intended output channels. These two designs can be used together within the same matrix to allow more flexible multichannel designs and concepts.

##### `Mach1TranscodeChannel::Coeffs`
Describe how each input channel should be applied to each output channel directly. 

##### `Mach1TranscodeChannel::Panner`
Describe the location of the input channel in a vector space to return the coefficients of the intermediary Mach1 Spatial supported multichannel vector configurations. Use traditional vector panner descriptions typically seen in other similar solutions or audio engineering tools.

`Mach1TranscodeChannel::Panner(float azimuth, float elevation, float diverge)`

### [PROPOSED IMPROVEMENTS](#improvements) ###
 - Describing channels per matrix either with `::Coeffs` or `::Panner` for easier description models
 - CI/CD on pull requests to recompile libraries after changes or additions are made to matrices

### [CONTRIBUTION](#contribution) ###
Please submit a pull request for the requested changes and upon review they will be added to the next build cycle of the libraries. CI/CD is in progress to encourage faster maintainability and more community oversight.

### [CONTACT](#contact) ###

General:
whatsup@mach1.tech