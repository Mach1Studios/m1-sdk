//  Mach1 Spatial SDK
//  Copyright © 2017-2020 Mach1. All rights reserved.

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
	#if __cplusplus > 201402L
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
	Mach1EncodeOutputMode8Ch,
	Mach1EncodeOutputMode12Ch,
	Mach1EncodeOutputMode14Ch,
	Mach1EncodeOutputMode16Ch,
	Mach1EncodeOutputMode18Ch
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

	M1_API void* Mach1EncodeCAPI_getResultingCoeffsDecoded(void * M1obj, enum Mach1DecodeAlgoType decodeType, float* decodeResult);
#if __cplusplus > 201103L
	[[deprecated("getResultingVolumesDecoded is deprecated, please use getResultingCoeffsDecoded instead")]]
#endif
	M1_API void* Mach1EncodeCAPI_getResultingVolumesDecoded(void * M1obj, enum Mach1DecodeAlgoType decodeType, float* decodeResult);

	M1_API enum Mach1EncodeInputModeType Mach1EncodeCAPI_getInputMode(void* M1obj);
	M1_API enum Mach1EncodeOutputModeType Mach1EncodeCAPI_getOutputMode(void* M1obj);
	M1_API int Mach1EncodeCAPI_getOutputChannelsCount(void* M1obj);

	M1_API void Mach1EncodeCAPI_setInputMode(void* M1obj, enum Mach1EncodeInputModeType inputMode);
	M1_API void Mach1EncodeCAPI_setOutputMode(void* M1obj, enum Mach1EncodeOutputModeType outputMode);

#if __cplusplus > 201103L
	[[deprecated("setRotation is deprecated due to ambiguity of use, please use setAzimuth0to1, setAzimuthDegrees or setAzimuthRadians instead")]]
#endif
	M1_API void Mach1EncodeCAPI_setRotation(void* M1obj, float rotation);

	M1_API void Mach1EncodeCAPI_setAzimuth(void* M1obj, float azimuth);
	M1_API void Mach1EncodeCAPI_setAzimuthDegrees(void* M1obj, float azimuth);
	M1_API void Mach1EncodeCAPI_setAzimuthRadians(void* M1obj, float azimuth);

	M1_API void Mach1EncodeCAPI_setDiverge(void* M1obj, float diverge);

#if __cplusplus > 201103L
	[[deprecated("setPitch is deprecated due to ambiguity of use, please use setElevation0to1, setStereoRotationDegrees or setStereoRotationRadians instead")]]
#endif
	M1_API void Mach1EncodeCAPI_setPitch(void* M1obj, float pitch);
	M1_API void Mach1EncodeCAPI_setElevation(void* M1obj, float elevation);
	M1_API void Mach1EncodeCAPI_setElevationDegrees(void* M1obj, float elevation);
	M1_API void Mach1EncodeCAPI_setElevationRadians(void* M1obj, float elevation);

	M1_API void Mach1EncodeCAPI_setIsotropicEncode(void* M1obj, bool isotropicEncode);

#if __cplusplus > 201103L
	[[deprecated("setStereoRotate is deprecated due to ambiguity of use, please use setOrbitRotation0to1, setOrbitRotationDegrees or setOrbitRotationRadians instead")]]
#endif
	M1_API void Mach1EncodeCAPI_setStereoRotate(void* M1obj, float sRotate);
	M1_API void Mach1EncodeCAPI_setOrbitRotation(void* M1obj, float orbitRotation);
	M1_API void Mach1EncodeCAPI_setOrbitRotationDegrees(void* M1obj, float orbitRotation);
	M1_API void Mach1EncodeCAPI_setOrbitRotationRadians(void* M1obj, float orbitRotation);
	M1_API void Mach1EncodeCAPI_setStereoSpread(void* M1obj, float sSpread);
	M1_API void Mach1EncodeCAPI_setAutoOrbit(void* M1obj, bool autoOrbit);

	M1_API long Mach1EncodeCAPI_getLastCalculationTime(void* M1obj);

#ifdef __cplusplus
}
#endif

#endif 
