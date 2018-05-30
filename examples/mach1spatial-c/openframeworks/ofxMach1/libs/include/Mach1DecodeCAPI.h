//  Mach1 SDK
//  Copyright © 2017 Mach1. All rights reserved.
//
//  Header file

#pragma once

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

struct Mach1Point3D {
	float x, y, z;
};

enum Mach1PlatformType {
	m1Default = 0, m1Unity, m1UE, m1oFEasyCam, m1Android, m1iOSPortrait, m1iOSLandscape
};

enum Mach1DecodeAlgoType {
	m1Spatial = 0, m1AltSpatial, m1Horizon, m1HorizonPairs, m1SpatialPairs
};

extern "C" { 
	M1_API void* Mach1DecodeCAPI_create();
	M1_API void Mach1DecodeCAPI_delete(void* M1obj);
  
	M1_API void Mach1DecodeCAPI_setPlatformType(void* M1obj, Mach1PlatformType type);
    M1_API void Mach1DecodeCAPI_setDecodeAlgoType(void* M1obj, Mach1DecodeAlgoType newAlgorithmType);
	M1_API void Mach1DecodeCAPI_setFilterSpeed(void* M1obj, float filterSpeed);
    
	M1_API void Mach1DecodeCAPI_decode(void* M1obj, float Yaw, float Pitch, float Roll, float *result, int bufferSize = 0, int sampleIndex = 0);

	M1_API void Mach1DecodeCAPI_beginBuffer(void* M1obj);
	M1_API void Mach1DecodeCAPI_endBuffer(void* M1obj);
  
	M1_API long Mach1DecodeCAPI_getCurrentTime(void* M1obj);
	M1_API char* Mach1DecodeCAPI_getLog(void * M1obj);

	M1_API Mach1Point3D Mach1DecodeCAPI_getCurrentAngle(void* M1obj);
}
