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

#ifndef MAX_CHANNELS_COUNT
#define MAX_CHANNELS_COUNT 20 // 18 + 2
#endif

#ifndef MAX_POINTS_COUNT
#define MAX_POINTS_COUNT 8
#endif

struct M1EncodeCorePoint
{
	float x, y, z;

	M1EncodeCorePoint operator + (M1EncodeCorePoint p)
	{
		return { this->x + p.x, this->y + p.y, this->z + p.z };
	}
	
	M1EncodeCorePoint operator / (float f)
	{
		return { this->x / f, this->y / f, this->z / f };
	}

	M1EncodeCorePoint operator - ()
	{
		return { -this->x, -this->y, -this->z };
	}

};

class M1EncodeCorePointResults {
	M1EncodeCorePoint ppoints[MAX_CHANNELS_COUNT];
	int pointsCount;
	std::string pointsNames[MAX_CHANNELS_COUNT];
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
		INPUT_1OAACN, 
		INPUT_1OAFUMA,
		INPUT_2OAACN, 
		INPUT_2OAFUMA,
		INPUT_3OAACN, 
		INPUT_3OAFUMA,
		INPUT_LCR
	};
	enum OutputMode { 
		OUTPUT_HORIZON_4CH = 0, 
		OUTPUT_SPATIAL_8CH,
		OUTPUT_SPATIALPLUS_12CH,
		OUTPUT_SPATIALPLUSPLUS_14CH,
		OUTPUT_SPATIALEXT_16CH,
		OUTPUT_SPATIALEXTPLUS_18CH
	};

	// arrays for CAPI
	M1EncodeCorePoint* arr_Points = nullptr;
	float** arr_Gains = nullptr;
	char** arr_PointsNames = nullptr;
	float* arr_GainsForInputChannelNamed = nullptr;
	float* arr_ResultingCoeffsDecoded = nullptr;

private:
	InputMode inputMode;
	OutputMode outputMode;

	float azimuth, diverge, elevation;
	float orbitRotation, sSpread;
	bool autoOrbit;
	bool isotropicEncode;

	void processGains4Channels(float x, float y, float(&result)[4]);
	void processGains8Channels(float x, float y, float z, float(&result)[8]);

	milliseconds ms;

	long timeLastCalculation;

public:
	M1EncodeCore();
	~M1EncodeCore();

	void generatePointResults();
	M1EncodeCorePointResults resultingPoints;

	void getResultingCoeffsDecoded(Mach1DecodeAlgoType decodeType, float *decodeResult, float *result);
#if __cplusplus > 201103L
	[[deprecated]]
#endif	
	void getResultingVolumesDecoded(Mach1DecodeAlgoType decodeType, float *decodeResult, float *result);

	InputMode getInputMode();
	OutputMode getOutputMode();
	int getOutputChannelsCount();

	void setInputMode(InputMode inputMode);
	void setOutputMode(OutputMode outputMode);
	void setAzimuth(float azimuth);
	void setAzimuthDegrees(float azimuth);
	void setAzimuthRadians(float azimuth);
	void setDiverge(float diverge);
	void setElevation(float elevation);
	void setElevationDegrees(float elevation);
	void setElevationRadians(float elevation);
	void setIsotropicEncode(bool isotropicEncode);

	void setOrbitRotation(float orbitRotation);
	void setOrbitRotationDegrees(float orbitRotation);
	void setOrbitRotationRadians(float orbitRotation);
	void setStereoSpread(float sSpread);
	void setAutoOrbit(bool autoOrbit);

	long getCurrentTime();
	long getLastCalculationTime();
};

#endif /* Mach1EncodeCore_h */
