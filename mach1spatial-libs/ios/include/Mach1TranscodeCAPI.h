//  Mach1 Spatial SDK
//  Copyright © 2017-2021 Mach1. All rights reserved.

#pragma once

#include "Mach1Point3D.h"
#include <stdbool.h>

#if defined(Mach1DecodeCore_h) || defined(Mach1EncodeCore_h) || defined(Mach1PositionalCore_h) || defined(Mach1TranscodeCore_h) || defined(Mach1AudioTimelineCore_h)
#    ifndef M1_API
#        if defined(_WINDOWS) || defined(WIN32)
#            define M1_API __declspec(dllexport)
#        else
#            define M1_API
#        endif
#    endif
#else
#    ifndef M1_API
#        if !defined(M1_STATIC) && (defined(_WINDOWS) || defined(WIN32))
#            define M1_API __declspec(dllimport)
#        else
#            define M1_API
#        endif
#    endif
#endif

const int Mach1TranscodeMAXCHANS = 64;

/* TODO: Implement process settings for object-audio format conversions
struct Mach1TranscodeProcessSettings {
    bool processObjectBed = true;
    bool processChannelBed = true;
    bool enableBinauralRendering = true;
};
*/

#ifdef __cplusplus
extern "C" {
#endif
M1_API void *Mach1TranscodeCAPI_create();
M1_API void Mach1TranscodeCAPI_delete(void *M1obj);

M1_API int Mach1TranscodeCAPI_getInputNumChannels(void *M1obj);
M1_API int Mach1TranscodeCAPI_getOutputNumChannels(void *M1obj);
M1_API int Mach1TranscodeCAPI_getFormatFromString(void *M1obj, const char *str);
M1_API const char *Mach1TranscodeCAPI_getFormatName(void *M1obj, int fmt);

M1_API float Mach1TranscodeCAPI_processNormalization(void *M1obj, float **bufs, int numSamples);
M1_API void Mach1TranscodeCAPI_processMasterGain(void *M1obj, float **bufs, int numSamples, float masterGain);

M1_API float Mach1TranscodeCAPI_db2level(void *M1obj, float db);
M1_API float Mach1TranscodeCAPI_level2db(void *M1obj, float level);

M1_API void Mach1TranscodeCAPI_setLFESub(void *M1obj, int *subChannelIndices, int numChannels, int sampleRate);
M1_API void Mach1TranscodeCAPI_setSpatialDownmixer(void *M1obj, float corrThreshold);
M1_API bool Mach1TranscodeCAPI_getSpatialDownmixerPossibility(void *M1obj);
M1_API float *Mach1TranscodeCAPI_getAvgSamplesDiff(void *M1obj);

M1_API void Mach1TranscodeCAPI_setInputFormat(void *M1obj, int inFmt);
M1_API void Mach1TranscodeCAPI_setInputFormatCustomPointsJson(void *M1obj, char *inJson);
M1_API void Mach1TranscodeCAPI_setInputFormatCustomPoints(void *M1obj, struct Mach1Point3D *points, int count);

M1_API void Mach1TranscodeCAPI_setOutputFormat(void *M1obj, int outFmt);
M1_API void Mach1TranscodeCAPI_setOutputFormatCustomPointsJson(void *M1obj, char *outJson);
M1_API void Mach1TranscodeCAPI_setOutputFormatCustomPoints(void *M1obj, struct Mach1Point3D *points, int count);

M1_API void Mach1TranscodeCAPI_setCustomPointsSamplerCallback(void *M1obj, Mach1Point3D *(*callback)(long long, int &));

M1_API bool Mach1TranscodeCAPI_processConversionPath(void *M1obj);
M1_API void Mach1TranscodeCAPI_getMatrixConversion(void *M1obj, float *matrix);
M1_API void Mach1TranscodeCAPI_processConversion(void *M1obj, float **inBufs, float **outBufs, int numSamples);

M1_API int *Mach1TranscodeCAPI_getFormatConversionPath(void *M1obj, int *count);

#ifdef __cplusplus
}
#endif