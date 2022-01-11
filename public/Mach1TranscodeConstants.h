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

struct Mach1FormatInfo {
	std::string name;
	int channels;
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
};

#endif