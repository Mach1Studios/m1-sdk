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
		INPUT_MONO = (int) 0, 
		INPUT_STEREO,
		INPUT_QUAD, 
		INPUT_LCRS, 
		INPUT_AFORMAT, 
		INPUT_1OAACN, 
		INPUT_1OAFUMA,
		INPUT_2OAACN, 
		INPUT_2OAFUMA,
		INPUT_3OAACN, 
		INPUT_3OAFUMA,
		INPUT_LCR,
		INPUT_FIVE_ZERO, /// (Using Mach1Transcode is recommended instead)
		INPUT_FIVE_ONE_FILM, /// (Using Mach1Transcode is recommended instead)
		INPUT_FIVE_ONE_DTS, /// (Using Mach1Transcode is recommended instead)
		INPUT_FIVE_ONE_SMPTE /// (Using Mach1Transcode is recommended instead)
	};
	enum OutputMode { 
		OUTPUT_HORIZON_4CH = (int) 0, 
		OUTPUT_SPATIAL_8CH,
		OUTPUT_SPATIAL_12CH,
		OUTPUT_SPATIAL_14CH,
		OUTPUT_SPATIAL_16CH,
		OUTPUT_SPATIAL_18CH,
		OUTPUT_SPATIAL_24CH
	};

	enum PannerMode {
		MODE_ISOTROPICLINEAR = (int) 0,
		MODE_ISOTROPICEQUALPOWER,
		MODE_PERIPHONICLINEAR
	};

	struct InputModeName { InputMode mode; std::string name; };
	std::vector<InputModeName> inputModeNames = {
		 { INPUT_MONO, "1.0"},
		 { INPUT_STEREO, "2.0_C"},
		 { INPUT_LCR, "3.0_LCR"},
		 { INPUT_QUAD, "M1Horizon"},
		 { INPUT_LCRS, "4.0_LCRS"},
		 { INPUT_AFORMAT, "4.0_AFormat"},
		 { INPUT_FIVE_ZERO, "5.0_C"},
		 { INPUT_FIVE_ONE_FILM, "5.1_C"},
		 { INPUT_FIVE_ONE_DTS, "5.1_C_Dts"},
		 { INPUT_FIVE_ONE_SMPTE, "5.1_C_SMPTE"},
		 { INPUT_1OAACN, "ACNSN3D"},
		 { INPUT_1OAFUMA, "FuMa"},
		 { INPUT_2OAACN, "ACNSN3DO2A"},
		 { INPUT_2OAFUMA, "FuMaO2A"},
		 { INPUT_3OAACN, "FuMaO3A"},
		 { INPUT_3OAFUMA, "ACNSN3DO3A"},
	};

	struct OutputModeName { OutputMode mode; std::string name; };
	std::vector<OutputModeName> outputModeNames = {
		 { OUTPUT_HORIZON_4CH, "M1Horizon-4"},
		 { OUTPUT_SPATIAL_8CH, "M1Spatial-8"},
		 { OUTPUT_SPATIAL_12CH, "M1Spatial-12"},
		 { OUTPUT_SPATIAL_14CH, "M1Spatial-14"},
		 { OUTPUT_SPATIAL_16CH, "M1Spatial-16"},
		 { OUTPUT_SPATIAL_18CH, "M1Spatial-18"},
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
	bool frontSurroundPerspective;
	float outputGainLinearMultipler;

	float getCoeffForStandardPoint(float x, float y, float z, Mach1Point3DCore point, bool ignoreZ);
	std::vector<float> getCoeffSetForStandardPointSet(float x, float y, float z, std::vector<Mach1Point3DCore>& pointSet, bool ignoreZ);
	void processGainsChannels(float x, float y, float z, std::vector<float>& result);

	milliseconds ms;
	long timeLastCalculation;

	float clamp(float n, float lower, float upper);

public:
	M1EncodeCore();
	~M1EncodeCore();

	void generatePointResults();
	M1EncodeCorePointResults resultingPoints;

	void getResultingCoeffsDecoded(Mach1DecodeAlgoType decodeType, float *decodeResult, float *result);

	InputMode getInputMode();
	OutputMode getOutputMode();
	int getInputChannelsCount();
	int getOutputChannelsCount();

	int getInputModeFromString(std::string name);
	int getOutputModeFromString(std::string name);

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
	void setFrontSurroundPerspective(bool frontSurroundPerspective);
	void setOutputGain(float outputGainMultipler, bool isDecibel);

	void setOrbitRotation(float orbitRotationFromMinusOnetoOne);
	void setOrbitRotationDegrees(float orbitRotationDegrees);
	void setOrbitRotationRadians(float orbitRotationRadians);
	void setStereoSpread(float sSpreadFrom0to1);
	void setAutoOrbit(bool autoOrbit);

	long getCurrentTime();
	long getLastCalculationTime();
};

#endif /* Mach1EncodeCore_h */
