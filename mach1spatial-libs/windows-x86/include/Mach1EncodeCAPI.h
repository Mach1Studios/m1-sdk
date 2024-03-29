//  Mach1 Spatial SDK
//  Copyright © 2017-2020 Mach1. All rights reserved.

#pragma once

#include "Mach1DecodeCAPI.h"
#include "Mach1Point3D.h"
#include "Mach1Point4D.h"

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

enum Mach1EncodeInputModeType {
    Mach1EncodeInputModeMono = (int)0,
    Mach1EncodeInputModeStereo,
    Mach1EncodeInputModeLCR,
    Mach1EncodeInputModeQuad,
    Mach1EncodeInputModeLCRS,
    Mach1EncodeInputModeAFormat,
    Mach1EncodeInputMode5dot0,
    Mach1EncodeInputMode5dot1Film,
    Mach1EncodeInputMode5dot1DTS,
    Mach1EncodeInputMode5dot1SMTPE,
    Mach1EncodeInputModeBFOAACN,
    Mach1EncodeInputModeBFOAFUMA,
    Mach1EncodeInputModeB2OAACN,
    Mach1EncodeInputModeB2OAFUMA,
    Mach1EncodeInputModeB3OAACN,
    Mach1EncodeInputModeB3OAFUMA
};

enum Mach1EncodeOutputModeType {
    Mach1EncodeOutputModeM1Spatial_8 = (int)0,
    Mach1EncodeOutputModeM1Horizon_4,
    Mach1EncodeOutputModeM1Spatial_12,
    Mach1EncodeOutputModeM1Spatial_14,
};

enum Mach1EncodePannerModeType {
    Mach1EncodePannerModeIsotropicLinear = (int)0,
    Mach1EncodePannerModeIsotropicEqualPower,
    Mach1EncodePannerModePeriphonicLinear
};

#ifdef __cplusplus
extern "C" {
#endif
M1_API void *Mach1EncodeCAPI_create();
M1_API void Mach1EncodeCAPI_copy(void *M1str, void *M1dst);
M1_API void Mach1EncodeCAPI_delete(void *M1obj);

M1_API void *Mach1EncodeCAPI_getPoints(void *M1obj);
M1_API void *Mach1EncodeCAPI_getGains(void *M1obj);
M1_API void *Mach1EncodeCAPI_getPointsNames(void *M1obj);
M1_API void *Mach1EncodeCAPI_getGainsForInputChannelNamed(void *M1obj, char *pointName);

M1_API void Mach1EncodeCAPI_generatePointResults(void *M1obj);
M1_API int Mach1EncodeCAPI_getPointsCount(void *M1obj);

M1_API void *Mach1EncodeCAPI_getResultingCoeffsDecoded(void *M1obj, enum Mach1DecodeAlgoType decodeType, float *decodeResult);

M1_API enum Mach1EncodeInputModeType Mach1EncodeCAPI_getInputMode(void *M1obj);
M1_API enum Mach1EncodeOutputModeType Mach1EncodeCAPI_getOutputMode(void *M1obj);
M1_API enum Mach1EncodePannerModeType Mach1EncodeCAPI_getPannerMode(void *M1obj);
M1_API bool Mach1EncodeCAPI_getAutoOrbit(void *M1obj);
M1_API int Mach1EncodeCAPI_getInputChannelsCount(void *M1obj);
M1_API int Mach1EncodeCAPI_getOutputChannelsCount(void *M1obj);

M1_API void Mach1EncodeCAPI_setInputMode(void *M1obj, enum Mach1EncodeInputModeType inputMode);
M1_API void Mach1EncodeCAPI_setOutputMode(void *M1obj, enum Mach1EncodeOutputModeType outputMode);

M1_API void Mach1EncodeCAPI_setAzimuth(void *M1obj, float azimuthFromMinus1To1);
M1_API void Mach1EncodeCAPI_setAzimuthDegrees(void *M1obj, float azimuthDegrees);
M1_API void Mach1EncodeCAPI_setAzimuthRadians(void *M1obj, float azimuthRadians);

M1_API void Mach1EncodeCAPI_setDiverge(void *M1obj, float divergeFromMinus1To1);

M1_API void Mach1EncodeCAPI_setElevation(void *M1obj, float elevationFromMinus1to1);
M1_API void Mach1EncodeCAPI_setElevationDegrees(void *M1obj, float elevationFromMinus90to90);
M1_API void Mach1EncodeCAPI_setElevationRadians(void *M1obj, float elevationFromMinusHalfPItoHalfPI);

M1_API void Mach1EncodeCAPI_setPannerMode(void *M1obj, enum Mach1EncodePannerModeType pannerMode);
M1_API void Mach1EncodeCAPI_setFrontSurroundPerspective(void *M1obj, bool frontSurroundPerspective);
M1_API void Mach1EncodeCAPI_setOutputGain(void *M1obj, float outputGainMultipler, bool isDecibel);

M1_API void Mach1EncodeCAPI_setOrbitRotation(void *M1obj, float orbitRotationFromMinusOnetoOne);
M1_API void Mach1EncodeCAPI_setOrbitRotationDegrees(void *M1obj, float orbitRotationDegrees);
M1_API void Mach1EncodeCAPI_setOrbitRotationRadians(void *M1obj, float orbitRotationRadians);

M1_API void Mach1EncodeCAPI_setStereoSpread(void *M1obj, float sSpreadFrom0to1);
M1_API void Mach1EncodeCAPI_setAutoOrbit(void *M1obj, bool autoOrbit);

M1_API long Mach1EncodeCAPI_getLastCalculationTime(void *M1obj);

#ifdef __cplusplus
}
#endif