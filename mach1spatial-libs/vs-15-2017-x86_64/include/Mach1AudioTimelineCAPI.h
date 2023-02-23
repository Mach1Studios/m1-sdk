//  Mach1 Spatial SDK
//  Copyright © 2017-2020 Mach1. All rights reserved.

#pragma once

#include "Mach1KeyPoint.h"
#include "Mach1Point3D.h"

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

#ifdef __cplusplus
extern "C" {
#endif
M1_API void *Mach1AudioObjectCAPI_create();
M1_API void Mach1AudioObjectCAPI_delete(void *M1obj);

M1_API const char *Mach1AudioObjectCAPI_getName(void *M1obj);
M1_API int Mach1AudioObjectCAPI_getKeyPointsCount(void *M1obj);
M1_API Mach1KeyPoint Mach1AudioObjectCAPI_getKeyPoint(void *M1obj, int n);
M1_API Mach1KeyPoint *Mach1AudioObjectCAPI_getKeyPoints(void *M1obj);

M1_API void Mach1AudioObjectCAPI_setName(void *M1obj, const char *name);
M1_API void Mach1AudioObjectCAPI_setKeyPoints(void *M1obj, Mach1KeyPoint *points, int n);

M1_API void *Mach1AudioTimelineCAPI_create();
M1_API void Mach1AudioTimelineCAPI_delete(void *M1obj);

M1_API int Mach1AudioTimelineCAPI_getAudioObjectCount(void *M1obj);
M1_API void *Mach1AudioTimelineCAPI_getAudioObject(void *M1obj, int n);

M1_API void Mach1AudioTimelineCAPI_parseADM(void *M1obj, char *inXml);
M1_API void Mach1AudioTimelineCAPI_parseAtmos(void *M1obj, char *inDotAtmos, char *inDotAtmosDotMetadata);

#ifdef __cplusplus
}
#endif