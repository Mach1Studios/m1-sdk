//  Mach1 SDK
//  Copyright © 2018 Mach1. All rights reserved.
//
//  Header file

#pragma once

#include "Mach1EncodeCore.h"
#include <cstring>

#ifdef Mach1Encode_h

#ifdef _WINDOWS
#define M1_API __declspec(dllexport)
#else
#define M1_API
#endif 

#else

#ifdef _WINDOWS
#define M1_API __declspec(dllimport)
#else
#define M1_API
#endif 

#endif 

extern "C" {

	M1_API void* M1EncodePointResultsCAPI_create();
	M1_API void M1EncodePointResultsCAPI_delete(void* M1obj);

	M1_API void* M1EncodePointResultsCAPI_getPoints(void* M1obj);
	M1_API void* M1EncodePointResultsCAPI_getGains(void* M1obj);
	M1_API void* M1EncodePointResultsCAPI_getPointsNames(void* M1obj);
	M1_API void* M1EncodePointResultsCAPI_getGainsForInputChannelNamed(void* M1obj, char* pointName);




	M1_API void* Mach1EncodeCAPI_create();
	M1_API void Mach1EncodeCAPI_delete(void* M1obj);

	M1_API void* Mach1EncodeCAPI_generatePointResults(void* M1obj);

	M1_API void Mach1EncodeCAPI_setInputMode(void* M1obj, int inputMode);
	M1_API void Mach1EncodeCAPI_setOutputMode(void* M1obj, int outputMode);
	M1_API void Mach1EncodeCAPI_setRotation(void* M1obj, float rotation);
	M1_API void Mach1EncodeCAPI_setDiverge(void* M1obj, float diverge);
	M1_API void Mach1EncodeCAPI_setPitch(void* M1obj, float pitch);
	M1_API void Mach1EncodeCAPI_setStereoRotate(void* M1obj, float sRotate);
	M1_API void Mach1EncodeCAPI_setStereoSpread(void* M1obj, float sSpread);
	M1_API void Mach1EncodeCAPI_setAutoOrbit(void* M1obj, bool autoOrbit);
	M1_API void Mach1EncodeCAPI_setIsotropicEncode(void* M1obj, bool isotropicEncode);
}
