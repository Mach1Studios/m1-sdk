//  Mach1 SDK
//  Copyright © 2017 Mach1. All rights reserved.

#include "Mach1Decode.h"

Mach1Decode::Mach1Decode()
{
	M1obj = Mach1DecodeCAPI_create();
}

Mach1Decode::~Mach1Decode()
{
	Mach1DecodeCAPI_delete(M1obj);
} 

void Mach1Decode::setPlatformType(Mach1PlatformType type)
{
	Mach1DecodeCAPI_setPlatformType(M1obj, type);
}

void Mach1Decode::setDecodeAlgoType(Mach1DecodeAlgoType newAlgorithmType)
{
	Mach1DecodeCAPI_setDecodeAlgoType(M1obj, newAlgorithmType);
}

void Mach1Decode::decode(float Yaw, float Pitch, float Roll, float * result, int bufferSize, int sampleIndex)
{
	Mach1DecodeCAPI_decode(M1obj, Yaw, Pitch, Roll, result, bufferSize, sampleIndex);
}

std::vector<float> Mach1Decode::decode(float Yaw, float Pitch, float Roll, int bufferSize, int sampleIndex)
{
	static std::vector<float> vec(18);

	Mach1DecodeCAPI_decode(M1obj, Yaw, Pitch, Roll, vec.data(), bufferSize, sampleIndex);

	return vec;
}

void Mach1Decode::setFilterSpeed(float filterSpeed) 
{
	Mach1DecodeCAPI_setFilterSpeed(M1obj, filterSpeed);
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

Mach1Point3D Mach1Decode::getCurrentAngle()
{
	return Mach1DecodeCAPI_getCurrentAngle(M1obj);
}
