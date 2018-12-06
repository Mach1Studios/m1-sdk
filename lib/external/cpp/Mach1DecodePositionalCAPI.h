//  Mach1 SDK
//  Copyright © 2018 Mach1. All rights reserved.
//
//  Header file

#pragma once

#include "Mach1DecodeCAPI.h"

#include "Mach1Point3D.h"
#include "Mach1Point4D.h"

#ifdef Mach1PositionalCore_h

#if defined(_WINDOWS) || defined(WIN32)
#ifndef M1_API 
#define M1_API __declspec(dllexport)
#else
#define M1_API
#endif 
#endif 

#else

#if (defined(_WINDOWS) || defined(WIN32)) && !defined(M1_API)
#ifndef M1_API 
#define M1_API __declspec(dllimport)
#else
#define M1_API
#endif 
#endif 

#endif 

#ifndef Mach1PositionalCAPI_h
#define Mach1PositionalCAPI_h


extern "C" {
	M1_API void* Mach1DecodePositionalCAPI_create();
	M1_API void Mach1DecodePositionalCAPI_delete(void* M1obj);

	M1_API void Mach1DecodePositionalCAPI_setPlatformType(void* M1obj, Mach1PlatformType platformType);
	M1_API void Mach1DecodePositionalCAPI_setDecodeAlgoType(void* M1obj, Mach1DecodeAlgoType newAlgorithmType);

	// settings
	M1_API void Mach1DecodePositionalCAPI_setUseBlendMode(void* M1obj, bool useBlendMode);
	M1_API void Mach1DecodePositionalCAPI_setIgnoreTopBottom(void* M1obj, bool ignoreTopBottom);

	M1_API void Mach1DecodePositionalCAPI_setMuteWhenOutsideObject(void* M1obj, bool muteWhenOutsideObject);
	M1_API void Mach1DecodePositionalCAPI_setMuteWhenInsideObject(void* M1obj, bool muteWhenInsideObject);

	M1_API void Mach1DecodePositionalCAPI_setUseFalloff(void* M1obj, bool useFalloff);
	M1_API void Mach1DecodePositionalCAPI_setFalloffCurve(void* M1obj, float falloffCurve);
	M1_API void Mach1DecodePositionalCAPI_setFalloffCurveBlendMode(void* M1obj, float falloffCurveBlendMode);

	M1_API void Mach1DecodePositionalCAPI_setUseClosestPointRotationMuteInside(void* M1obj, bool useClosestPointRotationMuteInside);

	M1_API void Mach1DecodePositionalCAPI_setUseYawForRotation(void* M1obj, bool useYawForRotation);
	M1_API void Mach1DecodePositionalCAPI_setUsePitchForRotation(void* M1obj, bool usePitchForRotation);
	M1_API void Mach1DecodePositionalCAPI_setUseRollForRotation(void* M1obj, bool useRollForRotation);

	// parameters for algo
	M1_API void Mach1DecodePositionalCAPI_setCameraPosition(void* M1obj, Mach1Point3D point);
	M1_API void Mach1DecodePositionalCAPI_setCameraRotation(void* M1obj, Mach1Point3D point);
	M1_API void Mach1DecodePositionalCAPI_setCameraRotationQuat(void* M1obj, Mach1Point4D point);
	M1_API void Mach1DecodePositionalCAPI_setDecoderAlgoPosition(void* M1obj, Mach1Point3D point);
	M1_API void Mach1DecodePositionalCAPI_setDecoderAlgoRotation(void* M1obj, Mach1Point3D point);
	M1_API void Mach1DecodePositionalCAPI_setDecoderAlgoRotationQuat(void* M1obj, Mach1Point4D point);
	M1_API void Mach1DecodePositionalCAPI_setDecoderAlgoScale(void* M1obj, Mach1Point3D point);

	M1_API void Mach1DecodePositionalCAPI_evaluatePostionResults(void* M1obj);
	M1_API void Mach1DecodePositionalCAPI_getVolumesWalls(void * M1obj, float* result);
	M1_API void Mach1DecodePositionalCAPI_getVolumesRoom(void * M1obj, float* result);
	M1_API float Mach1DecodePositionalCAPI_getDist(void* M1obj);
	M1_API Mach1Point3D Mach1DecodePositionalCAPI_getCurrentAngle(void* M1obj);
	M1_API Mach1Point3D Mach1DecodePositionalCAPI_getVolumeRotation(void* M1obj);
	M1_API void Mach1DecodePositionalCAPI_setFilterSpeed(void* M1obj, float filterSpeed);
}
