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

struct Mach1TranscodeChannelBase {
	virtual ~Mach1TranscodeChannelBase() { }
};

struct Mach1TranscodeCoeffs : public Mach1TranscodeChannelBase {
	std::vector<float> data;
};

struct Mach1TranscodePanner : public Mach1TranscodeChannelBase {
	float X;
	float Y;
	float Z;
};

class Mach1TranscodeChannel {
public:
	static Mach1TranscodeCoeffs* Coeffs(std::vector<float> data)
	{
		Mach1TranscodeCoeffs* obj = new Mach1TranscodeCoeffs();
		obj->data = data;
		return obj;
	}

	static Mach1TranscodePanner* Panner(float X, float Y, float Z)
	{
		Mach1TranscodePanner* obj = new Mach1TranscodePanner();
		obj->X = X;
		obj->Y = Y;
		obj->Z = Z;
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
		{ "CustomPoints", 0 },
		{ "Empty", 0 },
		{ "M1Horizon", 4 },
		{ "M1HorizonS", 6 },
		{ "M1HorizonPairs", 8 },
		{ "M1Spatial", 8 },
		{ "M1SpatialS", 10 },
		{ "M1SpatialPairs", 16 },
		{ "M1SpatialPlus", 12 },
		{ "M1SpatialPlusPlus", 14 },
		{ "M1SpatialExtended", 16 },
		{ "M1SpatialExtendedPlus", 18 },
		{ "M1SpatialFaces", 6 },
		{ "1.0", 1 },
		{ "2.0_M", 2 },
		{ "2.0_C", 2 },
		{ "3.0_LCR", 3 },
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
		{ "FuMa", 4 },
		{ "ACNSN3D", 4 },
		{ "TBE", 8 },
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
	};
};

#endif
