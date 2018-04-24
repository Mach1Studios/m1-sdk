//  Mach1 SDK
//  Copyright © 2017 Mach1. All rights reserved.
//
//  Header file

#pragma once

#ifndef Mach1EncodeCore_h
#define Mach1EncodeCore_h

#include <vector>
#include <string>

#ifndef PI
#define PI 3.14159265358979323846
#endif

#ifndef DEG_TO_RAD
#define DEG_TO_RAD (PI/180.0)
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
};

class M1EncodeCore {
public:
	enum InputMode { INPUT_MONO = 0, INPUT_STEREO, INPUT_QUAD, INPUT_AFORMAT, INPUT_BFORMAT };
	enum OutputMode { OUTPUT_4CH = 0, OUTPUT_8CH };

private:
	InputMode inputMode;
	OutputMode outputMode;

	float rotation, diverge, pitch;
	float sRotate, sSpread;
	bool autoOrbit;

	int outputChannelCount;

	void processGains4Channels(float x, float y, float(&result)[4]);
	void processGains8Channels(float x, float y, float z, float(&result)[8]);

public:
	M1EncodeCore();
	~M1EncodeCore();

	M1EncodeCorePointResults generatePointResults();

	void setInputMode(InputMode inputMode);
	void setOutputMode(OutputMode outputMode);
	void setRotation(float rotation);
	void setDiverge(float diverge);
	void setPitch(float pitch);
	void setStereoRotate(float sRotate);
	void setStereoSpread(float sSpread);
	void setAutoOrbit(bool autoOrbit);
};

#endif /* Mach1EncodeCore_h */
