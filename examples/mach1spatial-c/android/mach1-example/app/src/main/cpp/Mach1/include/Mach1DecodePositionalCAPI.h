//  Mach1 SDK
//  Copyright © 2018 Mach1. All rights reserved.
//
//  Header file

#pragma once

#include "Mach1DecodeCAPI.h"

#ifdef Mach1PositionalCore_h
    #ifndef M1_API
        #if defined(_WINDOWS) || defined(WIN32)
        #define M1_API __declspec(dllexport)
        #else
        #define M1_API
        #endif
    #endif
#else
    #ifndef M1_API
        #if defined(_WINDOWS) || defined(WIN32)
            #define M1_API __declspec(dllimport)
        #else
            #define M1_API
        #endif
    #endif
#endif

#ifndef Mach1PositionalCAPI_h
#define Mach1PositionalCAPI_h

#ifdef __cplusplus
extern "C" {
#endif
	M1_API void* Mach1DecodePositionalCAPI_create();
	M1_API void Mach1DecodePositionalCAPI_delete(void* M1obj);

	M1_API void Mach1DecodePositionalCAPI_setPlatformType(void* M1obj, enum Mach1PlatformType platformType);
	M1_API void Mach1DecodePositionalCAPI_setDecodeAlgoType(void* M1obj, enum Mach1DecodeAlgoType newAlgorithmType);

	M1_API void Mach1DecodePositionalCAPI_setMuteWhenOutsideObject(void* M1obj, bool muteWhenOutsideObject);
	M1_API void Mach1DecodePositionalCAPI_setMuteWhenInsideObject(void* M1obj, bool muteWhenInsideObject);

	M1_API void Mach1DecodePositionalCAPI_setUseAttenuation(void* M1obj, bool useAttenuation);
	M1_API void Mach1DecodePositionalCAPI_setAttenuationCurve(void* M1obj, float attenuationCurve);

	//Default uses `usePointCalculation`
	M1_API void Mach1DecodePositionalCAPI_setUsePlaneCalculation(void* M1obj, bool usePlaneCalculation);

	M1_API void Mach1DecodePositionalCAPI_setUseYawForRotation(void* M1obj, bool useYawForRotation);
	M1_API void Mach1DecodePositionalCAPI_setUsePitchForRotation(void* M1obj, bool usePitchForRotation);
	M1_API void Mach1DecodePositionalCAPI_setUseRollForRotation(void* M1obj, bool useRollForRotation);

	M1_API void Mach1DecodePositionalCAPI_setListenerPosition(void* M1obj, struct Mach1Point3D point);
	M1_API void Mach1DecodePositionalCAPI_setListenerRotation(void* M1obj, struct Mach1Point3D point);
	M1_API void Mach1DecodePositionalCAPI_setListenerRotationQuat(void* M1obj, struct Mach1Point4D point);
	M1_API void Mach1DecodePositionalCAPI_setDecoderAlgoPosition(void* M1obj, struct Mach1Point3D point);
	M1_API void Mach1DecodePositionalCAPI_setDecoderAlgoRotation(void* M1obj, struct Mach1Point3D point);
	M1_API void Mach1DecodePositionalCAPI_setDecoderAlgoRotationQuat(void* M1obj, struct Mach1Point4D point);
	M1_API void Mach1DecodePositionalCAPI_setDecoderAlgoScale(void* M1obj, struct Mach1Point3D point);

	M1_API void Mach1DecodePositionalCAPI_evaluatePositionResults(void* M1obj);
	M1_API void Mach1DecodePositionalCAPI_getCoefficients(void * M1obj, float* result);
	M1_API float Mach1DecodePositionalCAPI_getDist(void* M1obj);
	M1_API struct Mach1Point3D Mach1DecodePositionalCAPI_getCurrentAngle(void* M1obj);
	M1_API struct Mach1Point3D Mach1DecodePositionalCAPI_getCoefficientsRotation(void* M1obj);
	M1_API void Mach1DecodePositionalCAPI_setFilterSpeed(void* M1obj, float filterSpeed);

/* DEPRECATED START*/
#if __cplusplus > 201103L
	[[deprecated("setUseFalloff is deprecated, please use setUseAttenuation instead")]]
#endif
	M1_API void Mach1DecodePositionalCAPI_setUseFalloff(void* M1obj, bool useFalloff);
#if __cplusplus > 201103L
	[[deprecated("setFalloffCurve is deprecated, please use setAttenuationCurve instead")]]
#endif
	M1_API void Mach1DecodePositionalCAPI_setFalloffCurve(void* M1obj, float falloffCurve);
#if __cplusplus > 201103L
	[[deprecated("setFalloffCurveBlendMode is deprecated, please use setAttenuationCurveBlendMode instead")]]
#endif
    M1_API void Mach1DecodePositionalCAPI_setUseClosestPointRotationMuteInside(void* M1obj, bool useClosestPointRotationMuteInside);
#if __cplusplus > 201103L
	[[deprecated("setCameraPosition is deprecated, please use setListenerPosition instead")]]
#endif
    M1_API void Mach1DecodePositionalCAPI_setCameraPosition(void* M1obj, struct Mach1Point3D point);
#if __cplusplus > 201103L
	[[deprecated("setCameraRotation is deprecated, please use setListenerRotation instead")]]
#endif
    M1_API void Mach1DecodePositionalCAPI_setCameraRotation(void* M1obj, struct Mach1Point3D point);
#if __cplusplus > 201103L
	[[deprecated("setCameraRotationQuat is deprecated, please use setListenerRotationQuat instead")]]
#endif
    M1_API void Mach1DecodePositionalCAPI_setCameraRotationQuat(void* M1obj, struct Mach1Point4D point);
#if __cplusplus > 201103L
	[[deprecated("getVolumesWalls is deprecated, please use getCoefficients instead")]]
#endif
    M1_API void Mach1DecodePositionalCAPI_getVolumesWalls(void * M1obj, float* result);
#if __cplusplus > 201103L
	[[deprecated("getVolumeRotation is deprecated, please use getCoefficientsRotation instead")]]
#endif
    M1_API struct Mach1Point3D Mach1DecodePositionalCAPI_getVolumeRotation(void* M1obj);
/* DEPRECATED END*/

	/* Experimental Functions/Features */
	M1_API void Mach1DecodePositionalCAPI_setUseBlendMode(void* M1obj, bool useBlendMode);
	M1_API void Mach1DecodePositionalCAPI_setIgnoreTopBottom(void* M1obj, bool ignoreTopBottom);
#if __cplusplus > 201103L
	[[deprecated("setFalloffCurveBlendMode is deprecated, please use setAttenuationCurveBlendMode instead")]]
#endif
	M1_API void Mach1DecodePositionalCAPI_setFalloffCurveBlendMode(void* M1obj, float falloffCurveBlendMode);
	M1_API void Mach1DecodePositionalCAPI_setAttenuationCurveBlendMode(void* M1obj, float attenuationCurveBlendMode);
#if __cplusplus > 201103L
	[[deprecated("getVolumesRoom is deprecated, please use getCoefficientsInterior instead")]]
#endif
    M1_API void Mach1DecodePositionalCAPI_getVolumesRoom(void * M1obj, float* result);
	M1_API void Mach1DecodePositionalCAPI_getCoefficientsInterior(void * M1obj, float* result);
#ifdef __cplusplus
}
#endif

#endif
