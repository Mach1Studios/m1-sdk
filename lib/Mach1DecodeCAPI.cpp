//  Mach1 SDK
//  Copyright © 2017 Mach1. All rights reserved.
//
//

#include "Mach1DecodeCAPI.h"
#include "Mach1Decode.h"

void* Mach1DecodeCAPI_create(void)
{
	return new Mach1Decode();
}

void Mach1DecodeCAPI_delete(void * M1obj)
{
	if (M1obj != nullptr) {
		delete (Mach1Decode*)M1obj;
		M1obj = nullptr;
	}
}

float* Mach1DecodeCAPI_horizonAlgo(void* M1obj, float Yaw, float Pitch, float Roll, int bufferSize, int sampleIndex)
{
	static float data[10];
	
	if (M1obj != nullptr) {
		std::vector<float> v = ((Mach1Decode*)M1obj)->horizonAlgo(Yaw, Pitch, Roll, bufferSize, sampleIndex);
		for (int i = 0; i < 10; i++) data[i] = v[i];
	}
	else {
		for (int i = 0; i < 10; i++) data[i] = 0;
	}

	return data;
}

float* Mach1DecodeCAPI_horizonPairsAlgo(void* M1obj, float Yaw, float Pitch, float Roll, int bufferSize, int sampleIndex)
{
	static float data[8];

	if (M1obj != nullptr) {
		std::vector<float> v = ((Mach1Decode*)M1obj)->horizonPairsAlgo(Yaw, Pitch, Roll, bufferSize, sampleIndex);
		for (int i = 0; i < 8; i++) data[i] = v[i];
	}
	else {
		for (int i = 0; i < 8; i++) data[i] = 0;
	}

	return data;
}

float* Mach1DecodeCAPI_spatialAlgo(void* M1obj, float Yaw, float Pitch, float Roll, int bufferSize, int sampleIndex)
{
	static float data[18];

	if (M1obj != nullptr) {
		std::vector<float> v = ((Mach1Decode*)M1obj)->spatialAlgo(Yaw, Pitch, Roll, bufferSize, sampleIndex);
		for (int i = 0; i < 18; i++) data[i] = v[i];
	}
	else {
		for (int i = 0; i < 18; i++) data[i] = 0;
	}

	return data;
}

float* Mach1DecodeCAPI_spatialAltAlgo(void* M1obj, float Yaw, float Pitch, float Roll, int bufferSize, int sampleIndex)
{
	static float data[18];

	if (M1obj != nullptr) {
		std::vector<float> v = ((Mach1Decode*)M1obj)->spatialAltAlgo(Yaw, Pitch, Roll, bufferSize, sampleIndex);
		for (int i = 0; i < 18; i++) data[i] = v[i];
	}
	else {
		for (int i = 0; i < 18; i++) data[i] = 0;
	}

	return data;
}

float* Mach1DecodeCAPI_spatialPairsAlgo(void* M1obj, float Yaw, float Pitch, float Roll, int bufferSize, int sampleIndex)
{
	static float data[16];

	if (M1obj != nullptr) {
		std::vector<float> v = ((Mach1Decode*)M1obj)->spatialPairsAlgo(Yaw, Pitch, Roll, bufferSize, sampleIndex);
		for (int i = 0; i < 16; i++) data[i] = v[i];
	}
	else {
		for (int i = 0; i < 16; i++) data[i] = 0;
	}
	
	return data;
}

void Mach1DecodeCAPI_setAngularSettingsType(void* M1obj, int type)
{
	((Mach1Decode*)M1obj)->setAngularSettingsType(static_cast<Mach1Decode::AngularSettingsType>(type));
}

void Mach1DecodeCAPI_beginBuffer(void* M1obj)
{
	((Mach1Decode*)M1obj)->beginBuffer();
}

void Mach1DecodeCAPI_endBuffer(void* M1obj)
{
	((Mach1Decode*)M1obj)->endBuffer();
}
	
long Mach1DecodeCAPI_getCurrentTime(void * M1obj)
{
	return ((Mach1Decode*)M1obj)->getCurrentTime();
}

char* Mach1DecodeCAPI_getLog(void * M1obj)
{
	return ((Mach1Decode*)M1obj)->getLog();
}
