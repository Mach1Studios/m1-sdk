//  Mach1 Spatial SDK
//  Copyright © 2017-2022 Mach1. All rights reserved.
//

#pragma once

#ifndef Mach1TranscodeConstants_h
#define Mach1TranscodeConstants_h

#include <math.h>
#include <stddef.h>
#include <vector>
#include <map>
#include <string>

enum Mach1ChannelType {
	Mach1ChannelTypeDiscrete = (int)0,
	Mach1ChannelTypeLFE,
	Mach1ChannelTypeC
};

struct Mach1ChannelDescription {
	int index;
	Mach1ChannelType type;
};

struct Mach1FormatInfo {
	std::string name;
	int numChannels;
	std::vector<Mach1ChannelDescription> channelTypes;
};

struct Mach1GainCoeff {
    Mach1GainCoeff(float _gain = 1.0f, bool _isDecibel = false)
    {
        gain = _gain;
        isDecibel = _isDecibel;
    }
    float gain;
    bool isDecibel;
};

struct Mach1TranscodeChannelBase {
	virtual ~Mach1TranscodeChannelBase() { }
};

struct Mach1TranscodeCoeffs : public Mach1TranscodeChannelBase {
	std::vector<float> data;
};

struct Mach1TranscodePanner : public Mach1TranscodeChannelBase {
	float azimuth;
	float elevation;
	float diverge;
	Mach1GainCoeff outputGain; // can be used to apply pan-law concepts
};

class Mach1TranscodeChannel {
public:
	static Mach1TranscodeCoeffs* Coeffs(std::vector<float> data)
	{
		Mach1TranscodeCoeffs* obj = new Mach1TranscodeCoeffs();
		obj->data = data;
		return obj;
	}

	static Mach1TranscodePanner* Panner(float azimuthFromMinus180to180, float elevationFromMinus90to90, float divergeFromMinus1To1, const Mach1GainCoeff & outputGain)
	{
		Mach1TranscodePanner* obj = new Mach1TranscodePanner();
		obj->azimuth = azimuthFromMinus180to180;
		obj->elevation = elevationFromMinus90to90;
		obj->diverge = divergeFromMinus1To1;
		obj->outputGain = outputGain;
		return obj;
	}
	static Mach1TranscodePanner* Panner(float azimuthFromMinus180to180, float elevationFromMinus90to90, float divergeFromMinus1To1)
	{
		Mach1TranscodePanner* obj = new Mach1TranscodePanner();
		Mach1GainCoeff default_gain = {1.0f, false};
		obj->azimuth = azimuthFromMinus180to180;
		obj->elevation = elevationFromMinus90to90;
		obj->diverge = divergeFromMinus1To1;
		obj->outputGain = default_gain;
		return obj;
	}
};

struct Mach1TranscodeMatrix {
	std::string formatFrom;
	std::string formatTo;
	std::vector<Mach1TranscodeChannelBase*> channels;
};

namespace Mach1TranscodeConstants {

	const int MAXCHANS = 64;

	const std::vector<Mach1FormatInfo> formats = {
		/*
		 * VECTOR (VVBP / SPS)
		 */
		{ "CustomPoints", 0 },
		{ "Empty", 0 },
		{ "1.0", 1 },
		{ "M1Spatial-4", 4 },		// MACH1SPATIAL-4
		{ "M1Horizon-4_2", 6 },		// MACH1SPATIAL-4 + HEADLOCKED STEREO
		{ "M1HorizonPairs", 8 },
		{ "M1Spatial-6", 6 },		// MACH1SPATIAL-6 (hexadron planes)
		{ "M1Spatial-8", 8 }, 		// MACH1SPATIAL-8 (default)
		{ "M1Spatial-8_2", 10 }, 	// MACH1SPATIAL-8 + HEADLOCKED STEREO
		{ "M1Spatial-12", 12 }, 	// MACH1SPATIAL-12 (hexadron vertices + horizontal hexadron planes)
		{ "M1Spatial-14", 14 }, 	// MACH1SPATIAL-14 (hexadron vertices + hexadron planes)
		{ "M1Spatial-32", 32 }, 	// MACH1SPATIAL-32
		{ "M1Spatial-36", 36 }, 	// MACH1SPATIAL-36
		{ "M1Spatial-48", 48 }, 	// MACH1SPATIAL-48
		{ "M1Spatial-60", 60 }, 	// MACH1SPATIAL-60
		/*
		 * SURROUND
		 */
		{ "2.0_M", 2 },
		{ "2.0_C", 2 },
		{ "3.0_LCR", 3 },
		{ "4.0_LCRS", 4 },
		{ "4.0_AFormat", 4 },
		{ "5.0_M", 5 },
		{ "5.0_C", 5 },
		{ "5.0_S", 5 },
		{ "5.0_R", 5 },
		{ "5.0_C_SIM", 5 },
		{ "5.1_M", 6 },
		{ "5.1_C", 6 },
		{ "5.1_S", 6 },
		{ "5.1_R", 6 },
		{ "5.1_M_SMPTE", 6 },
		{ "5.1_C_SMPTE", 6 },
		{ "5.1_S_SMPTE", 6 },
		{ "5.1_R_SMPTE", 6 },
		{ "5.1_M_Dts", 6 },
		{ "5.1_C_Dts", 6 },
		{ "5.1_S_Dts", 6 },
		{ "5.1_R_Dts", 6 },
		{ "5.1_C_SIM", 6 },
		{ "5.0.2_M", 7 },
		{ "5.0.2_C", 7 },
		{ "5.0.2_S", 7 },
		{ "5.1.2_M", 8 },
		{ "5.1.2_C", 8 },
		{ "5.1.2_S", 8 },
		{ "5.0.4_M", 9 },
		{ "5.0.4_C", 9 },
		{ "5.0.4_S", 9 },
		{ "5.1.4_M", 10 },
		{ "5.1.4_C", 10 },
		{ "5.1.4_S", 10 },
		{ "5.0.5_C", 10 },
		{ "5.0.6_C", 11 },
		{ "Octahedron_M", 6 },
		{ "6.0_M", 6 },
		{ "7.0_M", 7 },
		{ "7.0_C", 7 },
		{ "7.0_S", 7 },
		{ "7.0_M_SDDS", 7 },
		{ "7.0_C_SDDS", 7 },
		{ "7.0_M_SMPTE", 7 },
		{ "7.0_C_SMPTE", 7 },
		{ "7.0_S_SMPTE", 7 },
		{ "7.0_C_3D", 7 },
		{ "7.0_C_SIM", 7 },
		{ "7.1_M", 8 },
		{ "7.1_C", 8 },
		{ "7.1_S", 8 },
		{ "7.1_M_SDDS", 8 },
		{ "7.1_C_SDDS", 8 },
		{ "7.1_M_SMPTE", 8 },
		{ "7.1_C_SMPTE", 8 },
		{ "7.1_S_SMPTE", 8 },
		{ "7.1_C_SIM", 8 },
		{ "7.0.2_M", 9 },
		{ "7.0.2_C", 9 },
		{ "7.0.2_S", 9 },
		{ "7.0.2_M_SDDS", 9 },
		{ "7.0.2_C_SDDS", 9 },
		{ "7.0.2_M_SMPTE", 9 },
		{ "7.0.2_C_SMPTE", 9 },
		{ "7.0.2_S_SMPTE", 9 },
		{ "7.1.2_M", 10 },
		{ "7.1.2_C", 10 },
		{ "7.1.2_C_SIM", 10 },
		{ "7.1.2_S", 10 },
		{ "7.1.2_M_SDDS", 10 },
		{ "7.1.2_C_SDDS", 10 },
		{ "7.1.2_M_SMPTE", 10 },
		{ "7.1.2_C_SMPTE", 10 },
		{ "7.1.2_S_SMPTE", 10 },
		{ "7.0.4_M", 11 },
		{ "7.0.4_C", 11 },
		{ "7.0.4_S", 11 },
		{ "7.1.4_M", 12 },
		{ "7.1.4_C", 12 },
		{ "7.1.4_C_SIM", 12 },
		{ "7.1.4_S", 12 },
		{ "7.0.6_C", 13 },
		{ "8.0.2_C_THX", 10},
		{ "9.0_M", 9 },
		{ "9.0_C", 9 },
		{ "9.1_M", 10 },
		{ "9.1_C", 10 },
		{ "9.1.6_M", 16 },
		{ "9.1.6_C", 16 },
		{ "10.0.2_C_THX", 12 },
		{ "16.0_M", 16 },
		{ "22.0_C", 22 },
		/*
		 * AMBISONIC
		 */
		{ "FOA-Ambix", 4 },
		{ "FOA-FuMa", 4 },
		{ "FuMa", 4 },    // legacy conversion, TODO: deprecate
		{ "ACNSN3D", 4 }, // legacy conversion, TODO: deprecate
		{ "TBE", 8 },
		{ "TBE_2", 10 },
		{ "ACNSN3DO2A", 9 },
		{ "FuMaO2A", 9 },
		{ "ACNSN3DO3A", 16 },
		{ "FuMaO3A", 16 },
		{ "ACNSN3DYorkBasic1oa", 4 },
		{ "ACNSN3DYorkmaxRE1oa", 4 },
		{ "ACNSN3DmaxRE1oa", 4 },
		{ "ACNSN3DmaxRE2oa", 9 },
		{ "ACNSN3DmaxRE3oa", 16 },
		{ "ACNSN3DmaxRE4oa", 25 },
		{ "ACNSN3DmaxRE5oa", 36 },
		{ "ACNSN3DmaxRE6oa", 49 },
		{ "ACNSN3DmaxRE7oa", 64 },
		/*
		 * MIC ARRAY
		 */
		{ "Ambeo", 4 },
		{ "TetraMic", 4 },
		{ "SPS-200", 4 },
		{ "NT-SF1", 4 },
		{ "ORTF3D", 8 },
	};
};

#endif
