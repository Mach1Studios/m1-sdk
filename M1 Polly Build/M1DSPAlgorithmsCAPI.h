//
//  Mach1 SDK
//
//  Multichannel audio format family
//
//  Mixing algorithms v 0.9.94b
//
//  Header file

#pragma once

#ifdef M1DSPAlgorithms_h

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
	M1_API void* M1DSPAlgorithmsCAPI_create();
	M1_API void M1DSPAlgorithmsCAPI_delete(void* M1obj);

	M1_API float* M1DSPAlgorithmsCAPI_fourChannelAlgorithm(void* M1obj, float Yaw, float Pitch, float Roll, bool smoothAngles = false);
	M1_API float* M1DSPAlgorithmsCAPI_fourPairsAlgorithm(void* M1obj, float Yaw, float Pitch, float Roll, bool smoothAngles = false);
	M1_API float* M1DSPAlgorithmsCAPI_eightChannelsIsotropicAlgorithm(void* M1obj, float Yaw, float Pitch, float Roll, bool smoothAngles = false);
	M1_API float* M1DSPAlgorithmsCAPI_eightChannelsAlgorithm(void* M1obj, float Yaw, float Pitch, float Roll, bool smoothAngles = false);
	M1_API float* M1DSPAlgorithmsCAPI_eightPairsAlgorithm(void* M1obj, float Yaw, float Pitch, float Roll, bool smoothAngles = false);

	M1_API long M1DSPAlgorithmsCAPI_getCurrentTime(void* M1obj);
}