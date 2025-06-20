//  Mach1 Spatial SDK
//  Copyright © 2017 Mach1. All rights reserved.

#pragma once

#include "Mach1DecodeCAPI.h"

#if defined(Mach1DecodeCore_h) || defined(Mach1EncodeCore_h) || defined(Mach1PositionalCore_h) || defined(Mach1TranscodeCore_h) || defined(Mach1AudioTimelineCore_h)
#    ifndef M1_API
#        if defined(_WINDOWS) || defined(_WIN32)
#            define M1_API __declspec(dllexport)
#        else
#            define M1_API
#        endif
#    endif
#else
#    ifndef M1_API
#        if !defined(M1_STATIC) && (defined(_WINDOWS) || defined(_WIN32))
#            define M1_API __declspec(dllimport)
#        else
#            define M1_API
#        endif
#    endif
#endif

#ifdef __cplusplus
extern "C" {
#endif
M1_API void *Mach1DecodePositionalCAPI_create();
M1_API void Mach1DecodePositionalCAPI_delete(void *M1obj);

M1_API void Mach1DecodePositionalCAPI_setPlatformType(void *M1obj, enum Mach1PlatformType platformType);
M1_API void Mach1DecodePositionalCAPI_setDecodeMode(void *M1obj, enum Mach1DecodeMode mode);

M1_API void Mach1DecodePositionalCAPI_setMuteWhenOutsideObject(void *M1obj, bool muteWhenOutsideObject);
M1_API void Mach1DecodePositionalCAPI_setMuteWhenInsideObject(void *M1obj, bool muteWhenInsideObject);

M1_API void Mach1DecodePositionalCAPI_setUseAttenuation(void *M1obj, bool useAttenuation);
M1_API void Mach1DecodePositionalCAPI_setAttenuationCurve(void *M1obj, float attenuationCurve);

// Default uses `usePointCalculation`
M1_API void Mach1DecodePositionalCAPI_setUsePlaneCalculation(void *M1obj, bool usePlaneCalculation);

M1_API void Mach1DecodePositionalCAPI_setUseYawForRotation(void *M1obj, bool useYawForRotation);
M1_API void Mach1DecodePositionalCAPI_setUsePitchForRotation(void *M1obj, bool usePitchForRotation);
M1_API void Mach1DecodePositionalCAPI_setUseRollForRotation(void *M1obj, bool useRollForRotation);

M1_API void Mach1DecodePositionalCAPI_setListenerPosition(void *M1obj, Mach1Point3D point);
M1_API void Mach1DecodePositionalCAPI_setListenerRotation(void *M1obj, Mach1Point3D point);
M1_API void Mach1DecodePositionalCAPI_setListenerRotationQuat(void *M1obj, Mach1Point4D point);
M1_API void Mach1DecodePositionalCAPI_setDecoderAlgoPosition(void *M1obj, Mach1Point3D point);
M1_API void Mach1DecodePositionalCAPI_setDecoderAlgoRotation(void *M1obj, Mach1Point3D point);
M1_API void Mach1DecodePositionalCAPI_setDecoderAlgoRotationQuat(void *M1obj, Mach1Point4D point);
M1_API void Mach1DecodePositionalCAPI_setDecoderAlgoScale(void *M1obj, Mach1Point3D point);

M1_API void Mach1DecodePositionalCAPI_evaluatePositionResults(void *M1obj);
M1_API void Mach1DecodePositionalCAPI_getCoefficients(void *M1obj, float *result);
M1_API float Mach1DecodePositionalCAPI_getDist(void *M1obj);
M1_API int Mach1DecodePositionalCAPI_getFormatChannelCount(void *M1obj);
M1_API int Mach1DecodePositionalCAPI_getFormatCoeffCount(void *M1obj);
M1_API Mach1Point3D Mach1DecodePositionalCAPI_getCurrentAngle(void *M1obj);
M1_API Mach1Point3D Mach1DecodePositionalCAPI_getCurrentAngleInternal(void *M1obj);
M1_API Mach1Point3D Mach1DecodePositionalCAPI_getPositionalRotation(void *M1obj);
M1_API void Mach1DecodePositionalCAPI_setFilterSpeed(void *M1obj, float filterSpeed);

M1_API Mach1Point3D Mach1DecodePositionalCAPI_getClosestPointOnPlane(void *M1obj);

M1_API long Mach1DecodePositionalCAPI_getLastCalculationTime(void *M1obj);
#ifdef __cplusplus
}
#endif
