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

void Mach1DecodeCAPI_setDecodeAlgoType(void * M1obj, Mach1DecodeAlgoType newAlgorithmType)
{
	if (M1obj != nullptr) {
		((Mach1DecodeCore*)M1obj)->setDecodeAlgoType(newAlgorithmType);
	}
}

void Mach1DecodeCAPI_setPlatformType(void * M1obj, Mach1PlatformType type)
{
	if (M1obj != nullptr) {
		((Mach1DecodeCore*)M1obj)->setPlatformType(type);
	}
}

void Mach1DecodeCAPI_decode(void * M1obj, float Yaw, float Pitch, float Roll, float * result, int bufferSize, int sampleIndex)
{
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
	
long Mach1DecodeCAPI_getCurrentTime(void * M1obj)
{
	return ((Mach1DecodeCore*)M1obj)->getCurrentTime();
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
