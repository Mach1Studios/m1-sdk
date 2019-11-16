//  Mach1 SDK
//  Copyright © 2018 Mach1. All rights reserved.
//
//  Header file

#pragma once

#include "Mach1Point3D.h"
#include "Mach1Point4D.h"
#include "Mach1DecodeCAPI.h"

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

#ifndef Mach1EncodeCAPI_h
#define Mach1EncodeCAPI_h

enum Mach1EncodeInputModeType {
	Mach1EncodeInputModeMono = 0, 
	Mach1EncodeInputModeStereo, 
	Mach1EncodeInputModeQuad, 
	Mach1EncodeInputModeLCRS, 
	Mach1EncodeInputModeAFormat, 
    #if __cplusplus >  201402L
    [[deprecated("Mach1EncodeInputModeBFormat is not specific enough, please use either: Mach1EncodeInputModeBFOAACN or Mach1EncodeInputModeBFOAFUMA", true)]]
    #endif
	Mach1EncodeInputModeBFormat, 
	Mach1EncodeInputModeBFOAACN, 
	Mach1EncodeInputModeBFOAFUMA,
	Mach1EncodeInputModeB2OAACN, 
	Mach1EncodeInputModeB2OAFUMA,
	Mach1EncodeInputModeB3OAACN, 
	Mach1EncodeInputModeB3OAFUMA,
	Mach1EncodeInputModeLCR
};

enum Mach1EncodeOutputModeType {
	Mach1EncodeOutputMode4Ch = 0, 
	Mach1EncodeOutputMode8Ch
};

#ifdef __cplusplus
extern "C" {
#endif
    M1_API void* Mach1EncodeCAPI_create();
	M1_API void Mach1EncodeCAPI_delete(void* M1obj);

	M1_API void* Mach1EncodeCAPI_getPoints(void* M1obj);
	M1_API void* Mach1EncodeCAPI_getGains(void* M1obj);
	M1_API void* Mach1EncodeCAPI_getPointsNames(void* M1obj);
	M1_API void* Mach1EncodeCAPI_getGainsForInputChannelNamed(void* M1obj, char* pointName);

	M1_API void Mach1EncodeCAPI_generatePointResults(void* M1obj);
	M1_API int Mach1EncodeCAPI_getPointsCount(void* M1obj);

	M1_API void* Mach1EncodeCAPI_getResultingVolumesDecoded(void * M1obj, enum Mach1DecodeAlgoType decodeType, float* decodeResult);

	M1_API void Mach1EncodeCAPI_setInputMode(void* M1obj, enum Mach1EncodeInputModeType inputMode);
	M1_API void Mach1EncodeCAPI_setOutputMode(void* M1obj, enum Mach1EncodeOutputModeType outputMode);
	M1_API void Mach1EncodeCAPI_setRotation(void* M1obj, float rotation);
	M1_API void Mach1EncodeCAPI_setDiverge(void* M1obj, float diverge);
	M1_API void Mach1EncodeCAPI_setPitch(void* M1obj, float pitch);
	M1_API void Mach1EncodeCAPI_setStereoRotate(void* M1obj, float sRotate);
	M1_API void Mach1EncodeCAPI_setStereoSpread(void* M1obj, float sSpread);
	M1_API void Mach1EncodeCAPI_setAutoOrbit(void* M1obj, bool autoOrbit);
	M1_API void Mach1EncodeCAPI_setIsotropicEncode(void* M1obj, bool isotropicEncode);

	M1_API long Mach1EncodeCAPI_getLastCalculationTime(void* M1obj);

#ifdef __cplusplus
}
#endif

#endif 
