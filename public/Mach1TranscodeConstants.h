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

struct Mach1TranscodeMatrix {
	std::string formatFrom;
	std::string formatTo;
	std::vector< std::vector<float> > data;
};

namespace Mach1TranscodeConstants {
	const float r2 = (float)sqrt(2.0);
	const float r3 = (float)sqrt(3.0);
	const float oor2 = (float)(1.0 / sqrt(2.0));
	const float oor3 = (float)(1.0 / sqrt(3.0));
	const float oor4 = (float)(1.0 / sqrt(4.0));
	const float oor8 = (float)(1.0 / sqrt(8.0));
	const float oor16 = (float)(1.0 / sqrt(16.0));
	const float oo2r2 = (float)(1.0 / (2.0 * sqrt(2.0)));
	const float r3or2 = (float)(sqrt(3.0) / sqrt(2.0));
	const float r3o2 = (float)sqrt(3.0) / 2.0f;
	const float r2o2 = (float)sqrt(2.0) / 2.0f;
	const float r2o3 = (float)sqrt(2.0) / 3.0f;
	const float r2o4 = (float)sqrt(2.0) / 4.0f;
	const float r2o6 = (float)sqrt(2.0) / 6.0f;
	const float r2o8 = (float)sqrt(2.0) / 8.0f;
	const float r2o12 = (float)sqrt(2.0) / 12.0f;
	const float r2o20 = (float)sqrt(2.0) / 20.0f;
	const float oo8 = (float)1.0f / 8.0f;

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
		{ "7.1_M", 8 },
		{ "7.1_C", 8 },
		{ "7.1_S", 8 },
		{ "7.1_M_SDDS", 8 },
		{ "7.1_C_SDDS", 8 },
		{ "7.1_M_SMPTE", 8 },
		{ "7.1_C_SMPTE", 8 },
		{ "7.1_S_SMPTE", 8 },
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
