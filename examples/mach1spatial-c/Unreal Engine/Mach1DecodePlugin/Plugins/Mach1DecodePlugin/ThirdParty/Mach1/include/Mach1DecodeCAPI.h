//  Mach1 SDK
//  Copyright � 2017 Mach1. All rights reserved.
//
//  Header file

#pragma once

#ifdef Mach1Decode_h

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
	M1_API void* Mach1DecodeCAPI_create();
	M1_API void Mach1DecodeCAPI_delete(void* M1obj);

	M1_API float* Mach1DecodeCAPI_horizonAlgo(void* M1obj, float Yaw, float Pitch, float Roll, int bufferSize = 0, int sampleIndex = 0);
	M1_API float* Mach1DecodeCAPI_horizonPairsAlgo(void* M1obj, float Yaw, float Pitch, float Roll, int bufferSize = 0, int sampleIndex = 0);
	M1_API float* Mach1DecodeCAPI_spatialAlgo(void* M1obj, float Yaw, float Pitch, float Roll, int bufferSize = 0, int sampleIndex = 0);
	M1_API float* Mach1DecodeCAPI_spatialAltAlgo(void* M1obj, float Yaw, float Pitch, float Roll, int bufferSize = 0, int sampleIndex = 0);
	M1_API float* Mach1DecodeCAPI_spatialPairsAlgo(void* M1obj, float Yaw, float Pitch, float Roll, int bufferSize = 0, int sampleIndex = 0);

	M1_API void Mach1DecodeCAPI_setAngularSettingsType(void* M1obj, int type);
	M1_API void Mach1DecodeCAPI_beginBuffer(void* M1obj);
	M1_API void Mach1DecodeCAPI_endBuffer(void* M1obj);
 
	M1_API long Mach1DecodeCAPI_getCurrentTime(void* M1obj);
	M1_API char* Mach1DecodeCAPI_getLog(void * M1obj);
}