//  Mach1 SDK
//  Copyright © 2017 Mach1. All rights reserved.

#pragma once

#include <vector>
#include <string>
#include "Mach1DecodeCAPI.h"

class Mach1Decode
{
	void* M1obj;

public:
 
	Mach1Decode();
	~Mach1Decode();

	void setPlatformType(Mach1PlatformType type);
	void setDecodeAlgoType(Mach1DecodeAlgoType newAlgorithmType);

#ifndef  __EMSCRIPTEN__ 
	void decode(float Yaw, float Pitch, float Roll, float *result, int bufferSize = 0, int sampleIndex = 0);
#endif
	std::vector<float> decode(float Yaw, float Pitch, float Roll, int bufferSize = 0, int sampleIndex = 0);

	void setFilterSpeed(float filterSpeed);

	void beginBuffer();
    void endBuffer();
	
	long getCurrentTime();
#ifndef  __EMSCRIPTEN__ 
	char* getLog();
#else
	std::string getLog();
#endif


    Mach1Point3D getCurrentAngle();

};
