//  Mach1 SDK
//  Copyright © 2017 Mach1. All rights reserved.

#include "Mach1Decode.h"
#include "Mach1DecodeCAPI.h"

Mach1Decode::Mach1Decode()
{
	M1obj = Mach1DecodeCAPI_create();
}

Mach1Decode::~Mach1Decode()
{
	Mach1DecodeCAPI_delete(M1obj);
} 

std::vector<float> Mach1Decode::horizonAlgo(float Yaw, float Pitch, float Roll, int bufferSize, int sampleIndex)
{
	float* data = Mach1DecodeCAPI_horizonAlgo(M1obj, Yaw, Pitch, Roll, bufferSize, sampleIndex);
	return std::vector<float>(data, data + 10);
}

std::vector<float> Mach1Decode::horizonPairsAlgo(float Yaw, float Pitch, float Roll, int bufferSize, int sampleIndex)
{
	float* data = Mach1DecodeCAPI_horizonPairsAlgo(M1obj, Yaw, Pitch, Roll, bufferSize, sampleIndex);
	return std::vector<float>(data, data + 6);
}

std::vector<float> Mach1Decode::spatialAlgo(float Yaw, float Pitch, float Roll, int bufferSize, int sampleIndex)
{
	float* data = Mach1DecodeCAPI_spatialAlgo(M1obj, Yaw, Pitch, Roll, bufferSize, sampleIndex);
	return std::vector<float>(data, data + 18);
}

std::vector<float> Mach1Decode::spatialAltAlgo(float Yaw, float Pitch, float Roll, int bufferSize, int sampleIndex)
{
	float* data = Mach1DecodeCAPI_spatialAltAlgo(M1obj, Yaw, Pitch, Roll, bufferSize, sampleIndex);
	return std::vector<float>(data, data + 18);
}

std::vector<float> Mach1Decode::spatialPairsAlgo(float Yaw, float Pitch, float Roll, int bufferSize, int sampleIndex)
{
	float* data = Mach1DecodeCAPI_spatialPairsAlgo(M1obj, Yaw, Pitch, Roll, bufferSize, sampleIndex);
	return std::vector<float>(data, data + 10);
}

void Mach1Decode::setAngularSettingsType(AngularSettingsType type)
{
	Mach1DecodeCAPI_setAngularSettingsType(M1obj, (int)type);
}

void Mach1Decode::beginBuffer()
{
	Mach1DecodeCAPI_beginBuffer(M1obj);
}

void Mach1Decode::endBuffer()
{
	Mach1DecodeCAPI_endBuffer(M1obj);
}

long Mach1Decode::getCurrentTime()
{
	return Mach1DecodeCAPI_getCurrentTime(M1obj);
}

char* Mach1Decode::getLog()
{
	return Mach1DecodeCAPI_getLog(M1obj);
}
