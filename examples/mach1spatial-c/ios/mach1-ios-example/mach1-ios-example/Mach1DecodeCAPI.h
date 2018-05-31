//  Mach1 SDK
//  Copyright © 2017 Mach1. All rights reserved.
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

#define Voidpointer void* 

struct Mach1Point3D {
    float x, y, z;
};

enum Mach1PlatformType {
    Mach1PlatformDefault = 0, Mach1PlatformUnity, Mach1PlatformUE, Mach1PlatformOfEasyCam, Mach1PlatformAndroid, Mach1PlatformiOSPortrait, Mach1PlatformiOSLandscape
};

enum Mach1DecodeAlgoType {
    Mach1DecodeAlgoSpatial = 0, Mach1DecodeAlgoAltSpatial, Mach1DecodeAlgoHorizon, Mach1DecodeAlgoHorizonPairs, Mach1DecodeAlgoSpatialPairs
};

#ifdef __cplusplus
extern "C" {
#endif
	M1_API void* Mach1DecodeCAPI_create();
	M1_API void Mach1DecodeCAPI_delete(void* M1obj);
    
    M1_API void Mach1DecodeCAPI_setDecodeAlgoType(void* M1obj, int newAlgorithmType);
    M1_API void Mach1DecodeCAPI_setPlatformType(void* M1obj, int type);

    M1_API void Mach1DecodeCAPI_decode(void* M1obj, float Yaw, float Pitch, float Roll, float *result, int bufferSize, int sampleIndex);

    M1_API void Mach1DecodeCAPI_setFilterSpeed(void* M1obj, float filterSpeed);
	M1_API void Mach1DecodeCAPI_beginBuffer(void* M1obj);
	M1_API void Mach1DecodeCAPI_endBuffer(void* M1obj);
 
	M1_API long Mach1DecodeCAPI_getCurrentTime(void* M1obj);
	M1_API char* Mach1DecodeCAPI_getLog(void * M1obj);
    
    M1_API struct Mach1Point3D Mach1DecodeCAPI_getCurrentAngle(void* M1obj);
#ifdef __cplusplus
}
#endif
