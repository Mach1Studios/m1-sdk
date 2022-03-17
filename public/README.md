## Mach1Transcode Conversion Matrices
The Mach1Transcode API is designed to handle and convert multichannel and spatial audio streams and soundfields between each other by leveraging intermediary VVBP/SPS multichannel configurations. This API operates with gain coefficient multiplication per channel but can easily be expanded with additional signal processing effects to keep it flexible yet maintaining a high quality and transparent conversion pipeline as it's default and baseline behavior.

#### Description
Each matrix is described as the input of the multichannel as rows and the converted output coeffs are the columns.
 - Inputs are rows
 - Outputs are columns
```
I1 -> | I1 | I2 | I3 | I4 |
I2 -> | I1 | I2 | I3 | I4 |
I3 -> | I1 | I2 | I3 | I4 |
		↓    ↓    ↓    ↓ 
		O1   O2   O3   O4
```

#### Multichannel Format/Configuration Naming Conventions:
 - M or Music			= `Music Mix` (Channels are spaced out evenly throughout the horizontal soundfield)
 - C or Cinema			= `Cinema Mix` (Channels are more focused on the front)
 - S or SideSurround 	= `Side Surround Mix` (Surround channels are oriented more to the sides instead of rear (+-110 azimuth instead of +-135))
 - R or RearSurround 	= `Rear Surround Mix` (Surround channels are oriented more to the rears instead of sides (+-154 azimuth instead of +-135))
 - SIM or Simulated		= `Simulated Room Mix` (Lessens the divergence of virtual speakers to quickly simulate hearing front/back soundfield within a real world listening environment)

#### Channel Order Variants:
 - Default / Film / C|24 = L, C, R, Ls, Rs, LFE ...
 - SMPTE				 = L, R, C, LFE, Ls, Rs ...
 - Dts					 = L, R, Ls, Rs, C, LFE ...

#### Describing Each Channel
Currently there are two ways to describe how an input channel distributes to the intended output channels. These two designs can be used together within the same matrix to allow more flexible multichannel designs and concepts.

##### `Mach1TranscodeChannel::Coeffs`
Describe how each input channel should be applied to each output channel directly. 

##### `Mach1TranscodeChannel::Panner`
Describe the location of the input channel in a vector space to return the coefficients of the intermediary Mach1 Spatial supported multichannel vector configurations. Use traditional vector panner descriptions typically seen in other similar solutions or audio engineering tools.

`Mach1TranscodeChannel::Panner(float azimuth, float elevation, float diverge)`