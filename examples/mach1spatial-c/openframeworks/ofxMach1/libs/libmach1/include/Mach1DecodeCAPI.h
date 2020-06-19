//  Mach1 Spatial SDK
//  Copyright © 2017-2020 Mach1. All rights reserved.

#pragma once

#include "Mach1Point3D.h"
#include "Mach1Point4D.h"
#include <stdbool.h>

#if defined(Mach1DecodeCore_h) || defined(Mach1EncodeCore_h) ||  defined(Mach1PositionalCore_h)
#ifndef M1_API
        #if defined(_WINDOWS) || defined(WIN32)
                #define M1_API __declspec(dllexport)
        #else
                #define M1_API
        #endif
    #endif
#else
    #ifndef M1_API
		#if !defined(M1_STATIC) && (defined(_WINDOWS) || defined(WIN32))
            #define M1_API __declspec(dllimport)
        #else
            #define M1_API
        #endif
    #endif
#endif


#ifndef Mach1DecodeCAPI_h
#define Mach1DecodeCAPI_h

enum Mach1PlatformType {
	Mach1PlatformDefault = 0, Mach1PlatformUnity, Mach1PlatformUE, Mach1PlatformOfEasyCam, Mach1PlatformAndroid, Mach1PlatformiOS
};

enum Mach1DecodeAlgoType {
	Mach1DecodeAlgoSpatial = 0, Mach1DecodeAlgoAltSpatial, Mach1DecodeAlgoHorizon, Mach1DecodeAlgoHorizonPairs, Mach1DecodeAlgoSpatialPairs, Mach1DecodeAlgoSpatialPlus, Mach1DecodeAlgoSpatialPlusPlus, Mach1DecodeAlgoSpatialExt, Mach1DecodeAlgoSpatialExtPlus
};


#ifdef __cplusplus
extern "C" {
#endif
	M1_API void* Mach1DecodeCAPI_create();
	M1_API void Mach1DecodeCAPI_delete(void* M1obj);
  
	M1_API void Mach1DecodeCAPI_setDecodeAlgoType(void* M1obj, enum Mach1DecodeAlgoType algorithmType);
	M1_API void Mach1DecodeCAPI_setPlatformType(void* M1obj, enum Mach1PlatformType platformType);

	M1_API enum Mach1DecodeAlgoType Mach1DecodeCAPI_getDecodeAlgoType(void* M1obj);
	M1_API enum Mach1PlatformType Mach1DecodeCAPI_getPlatformType(void* M1obj);

	M1_API void Mach1DecodeCAPI_decode(void* M1obj, float Yaw, float Pitch, float Roll, float *result, int bufferSize, int sampleIndex);
	M1_API void Mach1DecodeCAPI_decodeCoeffs(void* M1obj, float *result, int bufferSize, int sampleIndex);
	M1_API void Mach1DecodeCAPI_decodeCoeffsUsingTranscodeMatrix(void* M1obj, float *matrix, int channels, float *result, int bufferSize, int sampleIndex);

	M1_API void Mach1DecodeCAPI_setFilterSpeed(void* M1obj, float filterSpeed);
	M1_API void Mach1DecodeCAPI_beginBuffer(void* M1obj);
	M1_API void Mach1DecodeCAPI_endBuffer(void* M1obj);
  
	M1_API int Mach1DecodeCAPI_getFormatChannelCount(void* M1obj);
	M1_API void Mach1DecodeCAPI_setRotation(void* M1obj, Mach1Point3D newRotationFromMinusOnetoOne);
	M1_API void Mach1DecodeCAPI_setRotationDegrees(void* M1obj, Mach1Point3D newRotationDegrees);
	M1_API void Mach1DecodeCAPI_setRotationRadians(void* M1obj, Mach1Point3D newRotationRadians);
	M1_API void Mach1DecodeCAPI_setRotationQuat(void* M1obj, Mach1Point4D newRotationQuat);

	M1_API long Mach1DecodeCAPI_getCurrentTime(void* M1obj);
	M1_API long Mach1DecodeCAPI_getLastCalculationTime(void* M1obj);

	M1_API char* Mach1DecodeCAPI_getLog(void * M1obj);

	M1_API struct Mach1Point3D Mach1DecodeCAPI_getCurrentAngle(void* M1obj);
#ifdef __cplusplus
}
#endif

#endif 
