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
	void decode(float *result, int bufferSize = 0, int sampleIndex = 0);
#endif
	std::vector<float> decode(float Yaw, float Pitch, float Roll, int bufferSize = 0, int sampleIndex = 0);
	std::vector<float> decode(int bufferSize = 0, int sampleIndex = 0);

	int getOutputChannelsCount();
	void setRotationDegrees(Mach1Point3D rotation);

	void setFilterSpeed(float filterSpeed);

	void beginBuffer();
    void endBuffer();
	
	template<typename T>
	void decodeBuffer(std::vector<std::vector<T>>* inBuffer, std::vector<std::vector<T>>* outBuffer, int bufferSize);

	template<typename T>
	void decodeBuffer(std::vector<T*>* inBuffer, std::vector<T*>* outBuffer, int bufferSize);

	long getCurrentTime();
#ifndef  __EMSCRIPTEN__ 
	char* getLog();
#else
	std::string getLog();
#endif


    Mach1Point3D getCurrentAngle();

};
