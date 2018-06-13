//  Mach1 SDK
//  Copyright © 2017 Mach1. All rights reserved.
//
//

#include "Mach1DecodePositionalCAPI.h"
#include "Mach1DecodePositionalCore.h"

void* Mach1DecodePositionalCAPI_create(void)
{
	return new Mach1DecodePositionalCore();
}

void Mach1DecodePositionalCAPI_delete(void * M1obj)
{
	if (M1obj != nullptr) {
		delete (Mach1DecodePositionalCore*)M1obj;
		M1obj = nullptr;
	}
}

void Mach1DecodePositionalCAPI_setPlatformType(void * M1obj, Mach1PlatformType type)
{
	if (M1obj != nullptr) {
		((Mach1DecodePositionalCore*)M1obj)->setPlatformType(type);
	}
}

void Mach1DecodePositionalCAPI_setCameraPosition(void * M1obj, Mach1Point3D point)
{
	((Mach1DecodePositionalCore*)M1obj)->setCameraPosition(&Mach1Point3DCore { point.x, point.y, point.z } );
}

void Mach1DecodePositionalCAPI_setCameraRotation(void * M1obj, Mach1Point3D point)
{
	((Mach1DecodePositionalCore*)M1obj)->setCameraRotation(&Mach1Point3DCore{ point.x, point.y, point.z });
}

void Mach1DecodePositionalCAPI_setDecoderAlgoPosition(void * M1obj, Mach1Point3D point)
{
	((Mach1DecodePositionalCore*)M1obj)->setDecoderAlgoPosition(&Mach1Point3DCore{ point.x, point.y, point.z });
}

void Mach1DecodePositionalCAPI_setDecoderAlgoRotation(void * M1obj, Mach1Point3D point)
{
	((Mach1DecodePositionalCore*)M1obj)->setDecoderAlgoRotation(&Mach1Point3DCore{ point.x, point.y, point.z });
}

void Mach1DecodePositionalCAPI_setDecoderAlgoScale(void * M1obj, Mach1Point3D point)
{
	((Mach1DecodePositionalCore*)M1obj)->setDecoderAlgoScale(&Mach1Point3DCore{ point.x, point.y, point.z });
}

void Mach1DecodePositionalCAPI_evaluatePostionResults(void * M1obj)
{
	((Mach1DecodePositionalCore*)M1obj)->evaluatePostionResults();
}

float Mach1DecodePositionalCAPI_getVolumeWalls(void * M1obj)
{
	return ((Mach1DecodePositionalCore*)M1obj)->getVolumeWalls();
}

float Mach1DecodePositionalCAPI_getVolumeRoom(void * M1obj)
{
	return ((Mach1DecodePositionalCore*)M1obj)->getVolumeWalls();
}
 
Mach1Point3D Mach1DecodePositionalCAPI_getVolumeRotation(void * M1obj)
{
	Mach1Point3DCore angle = ((Mach1DecodePositionalCore*)M1obj)->getVolumeRotation();
	return Mach1Point3D{ angle.x, angle.y, angle.z };
}
