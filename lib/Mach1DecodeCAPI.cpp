//  Mach1 SDK
//  Copyright © 2018 Mach1. All rights reserved.
//
//

#include "Mach1DecodeCore.h"
#include "Mach1DecodeCAPI.h"

void* Mach1DecodeCAPI_create(void)
{
	return new Mach1DecodeCore();
}

void Mach1DecodeCAPI_delete(void * M1obj)
{
	if (M1obj != nullptr) {
		delete (Mach1DecodeCore*)M1obj;
		M1obj = nullptr;
	}
}

void Mach1DecodeCAPI_setDecodeAlgoType(void * M1obj, enum Mach1DecodeAlgoType newAlgorithmType)
{
	if (M1obj != nullptr) {
		((Mach1DecodeCore*)M1obj)->setDecodeAlgoType(newAlgorithmType);
	}
}

void Mach1DecodeCAPI_setPlatformType(void * M1obj, enum Mach1PlatformType type)
{
	if (M1obj != nullptr) {
		((Mach1DecodeCore*)M1obj)->setPlatformType(type);
	}
}

Mach1DecodeAlgoType Mach1DecodeCAPI_getDecodeAlgoType(void * M1obj)
{
	if (M1obj != nullptr) {
		return ((Mach1DecodeCore*)M1obj)->getDecodeAlgoType();
	}
	return (Mach1DecodeAlgoType)0;
}

Mach1PlatformType Mach1DecodeCAPI_getPlatformType(void * M1obj)
{
	if (M1obj != nullptr) {
		return ((Mach1DecodeCore*)M1obj)->getPlatformType();
	}
	return (Mach1PlatformType)0;
}

void Mach1DecodeCAPI_decode(void * M1obj, float Yaw, float Pitch, float Roll, float * result, int bufferSize, int sampleIndex)
{
	// clear
	for (int i = 0; i < ((Mach1DecodeCore*)M1obj)->getOutputChannelsCount(); i++) result[i] = 0;

	if (M1obj != nullptr) {
		((Mach1DecodeCore*)M1obj)->decode(Yaw, Pitch, Roll, result, bufferSize, sampleIndex);
	}
}
 
void Mach1DecodeCAPI_setFilterSpeed(void* M1obj, float filterSpeed)
{
	((Mach1DecodeCore*)M1obj)->setFilterSpeed(filterSpeed);
}

void Mach1DecodeCAPI_beginBuffer(void* M1obj)
{
	((Mach1DecodeCore*)M1obj)->beginBuffer();
}

void Mach1DecodeCAPI_endBuffer(void* M1obj)
{
	((Mach1DecodeCore*)M1obj)->endBuffer();
}

int Mach1DecodeCAPI_getOutputChannelsCount(void * M1obj)
{
	return ((Mach1DecodeCore*)M1obj)->getOutputChannelsCount();
}
	
long Mach1DecodeCAPI_getCurrentTime(void * M1obj)
{
	return ((Mach1DecodeCore*)M1obj)->getCurrentTime();
}

long Mach1DecodeCAPI_getLastCalculationTime(void * M1obj)
{
	return ((Mach1DecodeCore*)M1obj)->getLastCalculationTime();
}

char* Mach1DecodeCAPI_getLog(void * M1obj)
{
	return ((Mach1DecodeCore*)M1obj)->getLog();
}

Mach1Point3D Mach1DecodeCAPI_getCurrentAngle(void* M1obj)
{
	Mach1Point3DCore angle = ((Mach1DecodeCore*)M1obj)->getCurrentAngle();
	Mach1Point3D p = { angle.x, angle.y, angle.z };
	return p;
}
