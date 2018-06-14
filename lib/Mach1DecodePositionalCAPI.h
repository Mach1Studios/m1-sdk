//  Mach1 SDK
//  Copyright © 2017 Mach1. All rights reserved.
//
//  Header file

#pragma once

#include "external/cpp/Mach1DecodeCAPI.h"

#ifdef Mach1DecodeCore_h

#if defined(_WINDOWS) || defined(WIN32)
#define M1_API __declspec(dllexport)
#else
#define M1_API
#endif 

#else

#if defined(_WINDOWS) || defined(WIN32)
#define M1_API __declspec(dllimport)
#else
#define M1_API
#endif 

#endif 


extern "C" {
	M1_API void* Mach1DecodePositionalCAPI_create();
	M1_API void Mach1DecodePositionalCAPI_delete(void* M1obj);

	M1_API void Mach1DecodePositionalCAPI_setPlatformType(void* M1obj, Mach1PlatformType platformType);

	// settings
	M1_API void Mach1DecodePositionalCAPI_setUseBlendMode(void* M1obj, bool useBlendMode);
	M1_API void Mach1DecodePositionalCAPI_setIgnoreTopBottom(void* M1obj, bool ignoreTopBottom);

	M1_API void Mach1DecodePositionalCAPI_setMuteWhenOutsideObject(void* M1obj, bool muteWhenOutsideObject);
	M1_API void Mach1DecodePositionalCAPI_setMuteWhenInsideObject(void* M1obj, bool muteWhenInsideObject);

	M1_API void Mach1DecodePositionalCAPI_setUseFalloff(void* M1obj, bool useFalloff);
	M1_API void Mach1DecodePositionalCAPI_setUseClosestPointRotationMuteInside(void* M1obj, bool useClosestPointRotationMuteInside);

	M1_API void Mach1DecodePositionalCAPI_setUseYawForRotation(void* M1obj, bool useYawForRotation);
	M1_API void Mach1DecodePositionalCAPI_setUsePitchForRotation(void* M1obj, bool usePitchForRotation);
	M1_API void Mach1DecodePositionalCAPI_setUseRollForRotation(void* M1obj, bool useRollForRotation);

	// parameters for algo
	M1_API void Mach1DecodePositionalCAPI_setCameraPosition(void* M1obj, Mach1Point3D point);
	M1_API void Mach1DecodePositionalCAPI_setCameraRotation(void* M1obj, Mach1Point3D point);
	M1_API void Mach1DecodePositionalCAPI_setDecoderAlgoPosition(void* M1obj, Mach1Point3D point);
	M1_API void Mach1DecodePositionalCAPI_setDecoderAlgoRotation(void* M1obj, Mach1Point3D point);
	M1_API void Mach1DecodePositionalCAPI_setDecoderAlgoScale(void* M1obj, Mach1Point3D point);

	M1_API void Mach1DecodePositionalCAPI_evaluatePostionResults(void* M1obj);
	M1_API float Mach1DecodePositionalCAPI_getVolumeWalls(void* M1obj);
	M1_API float Mach1DecodePositionalCAPI_getVolumeRoom(void* M1obj);
	M1_API Mach1Point3D Mach1DecodePositionalCAPI_getVolumeRotation(void* M1obj);
}
