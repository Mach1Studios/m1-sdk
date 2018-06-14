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

void Mach1DecodePositionalCAPI_setUseBlendMode(void * M1obj, bool useBlendMode)
{
	((Mach1DecodePositionalCore*)M1obj)->setUseBlendMode(useBlendMode);
}

void Mach1DecodePositionalCAPI_setIgnoreTopBottom(void * M1obj, bool ignoreTopBottom)
{
	((Mach1DecodePositionalCore*)M1obj)->setIgnoreTopBottom(ignoreTopBottom);
}

void Mach1DecodePositionalCAPI_setMuteWhenOutsideObject(void * M1obj, bool muteWhenOutsideObject)
{
	((Mach1DecodePositionalCore*)M1obj)->setMuteWhenOutsideObject(muteWhenOutsideObject);
}

void Mach1DecodePositionalCAPI_setMuteWhenInsideObject(void * M1obj, bool muteWhenInsideObject)
{
	((Mach1DecodePositionalCore*)M1obj)->setMuteWhenInsideObject(muteWhenInsideObject);
}

void Mach1DecodePositionalCAPI_setUseFalloff(void * M1obj, bool useFalloff)
{
	((Mach1DecodePositionalCore*)M1obj)->setUseFalloff(useFalloff);
}

void Mach1DecodePositionalCAPI_setUseClosestPointRotationMuteInside(void * M1obj, bool useClosestPointRotationMuteInside)
{
	((Mach1DecodePositionalCore*)M1obj)->setUseClosestPointRotationMuteInside(useClosestPointRotationMuteInside);
}

void Mach1DecodePositionalCAPI_setUseYawForRotation(void * M1obj, bool useYawForRotation)
{
	((Mach1DecodePositionalCore*)M1obj)->setUseYawForRotation(useYawForRotation);
}

void Mach1DecodePositionalCAPI_setUsePitchForRotation(void * M1obj, bool usePitchForRotation)
{
	((Mach1DecodePositionalCore*)M1obj)->setUsePitchForRotation(usePitchForRotation);
}

void Mach1DecodePositionalCAPI_setUseRollForRotation(void * M1obj, bool useRollForRotation)
{
	((Mach1DecodePositionalCore*)M1obj)->setUseRollForRotation(useRollForRotation);
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
