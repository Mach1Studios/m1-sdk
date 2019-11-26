//  Mach1 SDK
//  Copyright © 2018 Mach1. All rights reserved.
//
//  Header file

#pragma once

#ifndef Mach1EncodeCore_h
#define Mach1EncodeCore_h

#include "Mach1DecodeCAPI.h"

#include <vector>
#include <string>
#include <chrono>

#ifndef SWIG
using namespace std::chrono;
#endif

#ifndef PI
#define PI 3.14159265358979323846f
#endif

#ifndef DEG_TO_RAD
#define DEG_TO_RAD (PI/180)
#endif

struct M1EncodeCorePoint
{
	float x, y, z;
};

class M1EncodeCorePointResults {
	M1EncodeCorePoint ppoints[7];
	int pointsCount;
	std::string pointsNames[7];
	std::vector<std::vector<float>> gains;

	friend class M1EncodeCore;

public:

	M1EncodeCorePointResults();
	~M1EncodeCorePointResults();

	std::vector<M1EncodeCorePoint> getPoints();
	std::vector<std::vector<float>> getGains();
	std::vector<std::string> getPointsNames();
	std::vector<float> getGainsForInputChannelNamed(std::string pointName);
	int getPointsCount();
};

class M1EncodeCore {
public:
	enum InputMode { 
		INPUT_MONO, 
		INPUT_STEREO,
		INPUT_QUAD, 
		INPUT_LCRS, 
		INPUT_AFORMAT, 
		#if __cplusplus > 201402L
		[[deprecated("INPUT_BFORMAT is not specific enough, please use either: INPUT_FOAACN or INPUT_FOAFUMA")]]
		#endif
		INPUT_BFORMAT, 
		INPUT_FOAACN, 
		INPUT_FOAFUMA,
		INPUT_2OAACN, 
		INPUT_2OAFUMA,
		INPUT_3OAACN, 
		INPUT_3OAFUMA,
		INPUT_LCR
	};
	enum OutputMode { 
		OUTPUT_4CH = 0, 
		OUTPUT_8CH 
	};

	// arrays for CAPI
	M1EncodeCorePoint* arr_Points = nullptr;
	float** arr_Gains = nullptr;
	char** arr_PointsNames = nullptr;
	float* arr_GainsForInputChannelNamed = nullptr;
	float* arr_ResultingVolumesDecoded = nullptr;

private:
	InputMode inputMode;
	OutputMode outputMode;

	float rotation, diverge, pitch;
	float sRotate, sSpread;
	bool autoOrbit;
	bool isotropicEncode;

	int outputChannelCount;

	void processGains4Channels(float x, float y, float(&result)[4]);
	void processGains8Channels(float x, float y, float z, float(&result)[8]);

	milliseconds ms;

	long timeLastCalculation;

public:
	M1EncodeCore();
	~M1EncodeCore();

	void generatePointResults();
	M1EncodeCorePointResults resultingPoints;

	void getResultingVolumesDecoded(Mach1DecodeAlgoType decodeType, float *decodeResult, float *result);

	void setInputMode(InputMode inputMode);
	void setOutputMode(OutputMode outputMode);
	void setRotation(float rotation);
	void setDiverge(float diverge);
	void setPitch(float pitch);
	void setStereoRotate(float sRotate);
	void setStereoSpread(float sSpread);
	void setAutoOrbit(bool autoOrbit);
	void setIsotropicEncode(bool isotropicEncode);

	long getCurrentTime();
	long getLastCalculationTime();
};

#endif /* Mach1EncodeCore_h */
