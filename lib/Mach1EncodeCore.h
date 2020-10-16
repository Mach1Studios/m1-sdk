//  Mach1 Spatial SDK
//  Copyright © 2017-2020 Mach1. All rights reserved.

/*
Internal Orientation Implementation:
  -  Azimuth[+] = rotate right 0-1 [Range: 0->360 | -180->180]
  -  Azimuth[-] = rotate left 0-1 [Range: 0->360 | -180->180]
  -  Elevation[+] = rotate up 0-1 [Range: -90->90]
  -  Elevation[-] = rotate down 0-1 [Range: -90->90]
  -  OrbitRotation[+] = rotate right 0-1 [Range: 0->360 | -180->180]
  -  OrbitRotation[-] = rotate left 0-1 [Range: 0->360 | -180->180]

Mach1EncodeCore normalizes all input ranges to an unsigned "0 to 1" range for Azimuth, Elevation and OrbitRotation.
 */

#pragma once

#ifndef Mach1EncodeCore_h
#define Mach1EncodeCore_h

#include "Mach1Point3DCore.h"
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


class M1EncodeCorePointResults {
	Mach1Point3DCore ppoints[MAX_POINTS_COUNT];
	int pointsCount;
	std::string pointsNames[MAX_POINTS_COUNT];
	std::vector<std::vector<float>> gains;

	friend class M1EncodeCore;

public:

	M1EncodeCorePointResults();
	~M1EncodeCorePointResults();

	std::vector<Mach1Point3DCore> getPoints();
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

	enum PannerMode {
		MODE_ISOTROPICLINEAR = 0,
		MODE_ISOTROPICEQUALPOWER,
		MODE_PERIPHONICLINEAR
	};

	// arrays for CAPI
	Mach1Point3DCore* arr_Points = nullptr;
	float** arr_Gains = nullptr;
	char** arr_PointsNames = nullptr;
	float* arr_GainsForInputChannelNamed = nullptr;
	float* arr_ResultingCoeffsDecoded = nullptr;

private:
	InputMode inputMode;
	OutputMode outputMode;
	PannerMode pannerMode;

	float azimuth, diverge, elevation;
	float orbitRotation, sSpread;
	bool autoOrbit;

	float getCoeffForStandardPoint(float x, float y, float z, Mach1Point3DCore point, bool ignoreZ);
	std::vector<float> getCoeffSetForStandardPointSet(float x, float y, float z, std::vector<Mach1Point3DCore>& pointSet, bool ignoreZ);
	void processGainsChannels(float x, float y, float z, std::vector<float>& result);

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
	int getInputChannelsCount();
	int getOutputChannelsCount();

	void setInputMode(InputMode inputMode);
	void setOutputMode(OutputMode outputMode);
	void setAzimuth(float azimuthFromMinus1To1);
	void setAzimuthDegrees(float azimuthDegrees);
	void setAzimuthRadians(float azimuthRadians);
	void setDiverge(float divergeFromMinus1To1);
	void setElevation(float elevationFromMinus1to1);
	void setElevationDegrees(float elevationFromMinus90to90);
	void setElevationRadians(float elevationFromMinusHalfPItoHalfPI);
	void setIsotropicEncode(bool isotropicEncode);
	void setPannerMode(PannerMode pannerMode);

	void setOrbitRotation(float orbitRotationFromMinusOnetoOne);
	void setOrbitRotationDegrees(float orbitRotationDegrees);
	void setOrbitRotationRadians(float orbitRotationRadians);
	void setStereoSpread(float sSpreadFrom0to1);
	void setAutoOrbit(bool autoOrbit);

	long getCurrentTime();
	long getLastCalculationTime();
};

#endif /* Mach1EncodeCore_h */
