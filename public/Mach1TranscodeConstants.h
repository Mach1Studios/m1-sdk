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

enum Mach1FormatChannelType {
	Mach1FormatChannelDiscrete = (int)0,
	Mach1FormatChannelLFE,
	Mach1FormatChannelC
};

struct Mach1FormatChannel {
	int index;
	Mach1FormatChannelType type;
};

struct Mach1FormatInfo {
	std::string name;
	int channelsCount;
	std::vector<Mach1FormatChannel> channelTypes;
};

struct Mach1SpatialSoundMatrix {
	std::string formatFrom;
	std::string formatTo;
	std::vector<std::vector<float>> data;
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
		{ "FuMa", 4 },
		{ "ACNSN3D", 4 },
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
		{ "Mono", 1 },
		{ "Stereo", 2 },
		{ "LCR", 3 },
		{ "FiveOh", 5 },
		{ "FiveOneFilm", 6 },
		{ "FiveOneFilm_Cinema", 6 },
		{ "FiveOneSmpte", 6 },
		{ "FiveOneDts", 6 },
		{ "SixOh", 6 },
		{ "SevenOnePt", 8 },
		{ "SevenOnePt_Cinema", 8 },
		{ "ACNSN3DO2A", 9 },
		{ "FuMaO2A", 9 },
		{ "TBE", 8 },
		{ "ACNSN3DO3A", 16 },
		{ "FuMaO3A", 16 },
		{ "SevenOneSDDS", 8 },
		{ "SevenZeroSDDS", 7 },
		{ "SevenOneTwo", 10 },
		{ "SevenZeroTwo", 9 },
		{ "NineOne", 10 },
		{ "NineZero", 9 },
		{ "Stereo_Cinema", 2 },
		{ "FiveOneTwo", 8 },
		{ "FiveZeroTwo", 7 },
		{ "FiveOneFour", 10 },
		{ "FiveOneFourSMPTE", 10 },
		{ "FiveZeroFour", 9 },
		{ "SevenOneFour", 12 },
		{ "SevenZeroFour", 11 },
		{ "SevenZero_Cinema", 7 },
		{ "M1SpatialFaces", 6 },
		{ "NineOneFour", 14 },
		{ "NineOneSix", 16 },
		{ "MarcoSixteen", 16 },
		{ "ACNSN3DmaxRE1oa", 4 },
		{ "ACNSN3DmaxRE2oa", 9 },
		{ "ACNSN3DmaxRE3oa", 16 },
		{ "ACNSN3DmaxRE4oa", 25 },
		{ "ACNSN3DmaxRE5oa", 36 },
		{ "ACNSN3DmaxRE6oa", 49 },
		{ "ACNSN3DmaxRE7oa", 64 },
		{ "DolbyAtmosSevenOneTwo", 10 },
	};
};

#endif