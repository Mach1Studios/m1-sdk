//  Mach1 SDK
//  Copyright © 2017 Mach1. All rights reserved.

#pragma once

#include <vector>
#include "Mach1DecodeCAPI.h"

class Mach1Decode
{
	void* M1obj;

public:
 
	Mach1Decode();
	~Mach1Decode();

	void setAngularSettingsType(Mach1AngularSettingsType type);
	void setAlgorithmType(Mach1AlgorithmType newAlgorithmType);

	void decode(float Yaw, float Pitch, float Roll, float *result, int bufferSize = 0, int sampleIndex = 0);
	std::vector<float> decode(float Yaw, float Pitch, float Roll, int bufferSize = 0, int sampleIndex = 0);

	void setFilterSpeed(float filterSpeed);

	void beginBuffer();
    void endBuffer();
	
	long getCurrentTime();
	char* getLog();

    Mach1Point3D getCurrentAngle();
};
