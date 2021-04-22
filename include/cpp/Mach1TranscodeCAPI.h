//  Mach1 Spatial SDK
//  Copyright © 2017-2021 Mach1. All rights reserved.

#pragma once

#include <stdbool.h>
#include "Mach1Point3D.h"

#if defined(Mach1DecodeCore_h) || defined(Mach1EncodeCore_h) ||  defined(Mach1PositionalCore_h) || defined(Mach1TranscodeCore_h)
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


#ifndef Mach1TranscodeCAPI_h
#define Mach1TranscodeCAPI_h

const int Mach1TranscodeMAXCHANS = 64;

enum Mach1TranscodeFormatType {
	Mach1TranscodeFormatEmpty = 0,
	Mach1TranscodeFormatFuMa,
	Mach1TranscodeFormatACNSN3D,
	Mach1TranscodeFormatM1Horizon,
	Mach1TranscodeFormatM1HorizonS,
	Mach1TranscodeFormatM1HorizonPairs,
	Mach1TranscodeFormatM1Spatial,
	Mach1TranscodeFormatM1SpatialS,
	Mach1TranscodeFormatM1SpatialPairs,
	Mach1TranscodeFormatM1SpatialPlus,
	Mach1TranscodeFormatM1SpatialPlusPlus,
	Mach1TranscodeFormatM1SpatialExtended,
	Mach1TranscodeFormatM1SpatialExtendedPlus,
	Mach1TranscodeFormatMono, 
	Mach1TranscodeFormatStereo,
	Mach1TranscodeFormatLCR,
	Mach1TranscodeFormatFiveOh,
	Mach1TranscodeFormatFiveOneFilm,
	Mach1TranscodeFormatFiveOneFilm_Cinema,
	Mach1TranscodeFormatFiveOneSmpte,
	Mach1TranscodeFormatFiveOneDts,
	Mach1TranscodeFormatSixOh,
	Mach1TranscodeFormatSevenOnePt,
	Mach1TranscodeFormatSevenOnePt_Cinema,
	Mach1TranscodeFormatACNSN3DO2A,
	Mach1TranscodeFormatFuMaO2A,
	Mach1TranscodeFormatTBE,
	Mach1TranscodeFormatACNSN3DO3A,
	Mach1TranscodeFormatFuMaO3A,
	Mach1TranscodeFormatSevenOneSDDS,
	Mach1TranscodeFormatSevenZeroSDDS,
	Mach1TranscodeFormatSevenOneTwo,
	Mach1TranscodeFormatSevenZeroTwo,
	Mach1TranscodeFormatNineOne,
	Mach1TranscodeFormatNineZero,
	Mach1TranscodeFormatStereo_Cinema,
	Mach1TranscodeFormatFiveOneTwo,
	Mach1TranscodeFormatFiveZeroTwo,
	Mach1TranscodeFormatFiveOneFour,
	Mach1TranscodeFormatFiveZeroFour,
	Mach1TranscodeFormatSevenOneFour,
	Mach1TranscodeFormatSevenZeroFour,
	Mach1TranscodeFormatSevenZero_Cinema,
	Mach1TranscodeFormatM1SpatialFaces,
	Mach1TranscodeFormatNineOneFour,
	Mach1TranscodeFormatNineOneSix,
	Mach1TranscodeFormatMarcoSixteen,
	Mach1TranscodeFormatTTPoints,
    Mach1TranscodeFormatACNSN3DmaxRE1oa,
    Mach1TranscodeFormatACNSN3DmaxRE2oa,
    Mach1TranscodeFormatACNSN3DmaxRE3oa,
    Mach1TranscodeFormatACNSN3DmaxRE4oa,
    Mach1TranscodeFormatACNSN3DmaxRE5oa,
    Mach1TranscodeFormatACNSN3DmaxRE6oa,
    Mach1TranscodeFormatACNSN3DmaxRE7oa,
};

#ifdef __cplusplus
extern "C" {
#endif
	M1_API void* Mach1TranscodeCAPI_create();
	M1_API void Mach1TranscodeCAPI_delete(void* M1obj);

	M1_API int Mach1TranscodeCAPI_getInputNumChannels(void* M1obj);
	M1_API int Mach1TranscodeCAPI_getOutputNumChannels(void* M1obj);
	M1_API enum Mach1TranscodeFormatType Mach1TranscodeCAPI_getFormatFromString(void* M1obj, char* str);
	M1_API char* Mach1TranscodeCAPI_getFormatName(void* M1obj, enum Mach1TranscodeFormatType fmt);

	M1_API float Mach1TranscodeCAPI_processNormalization(void* M1obj, float** bufs, int numSamples);
	M1_API void Mach1TranscodeCAPI_processMasterGain(void* M1obj, float** bufs, int numSamples, float masterGain);

	M1_API float Mach1TranscodeCAPI_db2level(void* M1obj, float db);
	M1_API float Mach1TranscodeCAPI_level2db(void* M1obj, float level);

	M1_API void Mach1TranscodeCAPI_setLFESub(void* M1obj, int* subChannelIndices, int numChannels, int sampleRate);
	M1_API void Mach1TranscodeCAPI_setSpatialDownmixer(void* M1obj, float corrThreshold);
	M1_API bool Mach1TranscodeCAPI_getSpatialDownmixerPossibility(void* M1obj);

	M1_API void Mach1TranscodeCAPI_setInputFormat(void* M1obj, enum Mach1TranscodeFormatType inFmt);
	M1_API void Mach1TranscodeCAPI_setInputFormatADM(void* M1obj, char* inXml);
	M1_API void Mach1TranscodeCAPI_setInputFormatTTJson(void* M1obj, char* inJson);
	M1_API void Mach1TranscodeCAPI_setInputFormatTTPoints(void* M1obj, struct Mach1Point3D* points, int count);

	M1_API void Mach1TranscodeCAPI_setOutputFormat(void* M1obj, enum Mach1TranscodeFormatType outFmt);
	M1_API void Mach1TranscodeCAPI_setOutputFormatTTJson(void* M1obj, char* outJson);
	M1_API void Mach1TranscodeCAPI_setOutputFormatTTPoints(void* M1obj, struct Mach1Point3D* points, int count);

	M1_API bool Mach1TranscodeCAPI_processConversionPath(void* M1obj);
	M1_API void Mach1TranscodeCAPI_getMatrixConversion(void* M1obj, float* matrix);
	M1_API void Mach1TranscodeCAPI_processConversion(void* M1obj, float** inBufs, float** outBufs, int numSamples);

	M1_API enum Mach1TranscodeFormatType* Mach1TranscodeCAPI_getFormatConversionPath(void* M1obj, int* count);

#ifdef __cplusplus
}
#endif

#endif 
