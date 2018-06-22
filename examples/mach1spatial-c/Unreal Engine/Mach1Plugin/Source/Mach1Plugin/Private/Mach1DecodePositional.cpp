//  Mach1 SDK
//  Copyright © 2017 Mach1. All rights reserved.
//
//  Implementation

/*
DISCLAIMER:
This header file is not an example of use but an decoder that will require periodic
updates and should not be integrated in sections but remain as an update-able factored file.
*/


#include "Mach1DecodePositional.h"

#ifndef DEG_TO_RAD
#define DEG_TO_RAD (PI/180.0)
#endif

#ifndef __FLT_EPSILON__
#define __FLT_EPSILON__ 1.19209290e-07F
#endif
 
 
 
 
Mach1DecodePositional::Mach1DecodePositional()
{
	M1obj = Mach1DecodePositionalCAPI_create();
}

Mach1DecodePositional::~Mach1DecodePositional()
{
	Mach1DecodePositionalCAPI_delete(M1obj);
} 

void Mach1DecodePositional::setPlatformType(Mach1PlatformType type)
{
	Mach1DecodePositionalCAPI_setPlatformType(M1obj, type);
}

void Mach1DecodePositional::setDecodeAlgoType(Mach1DecodeAlgoType newAlgorithmType)
{
	Mach1DecodePositionalCAPI_setDecodeAlgoType(M1obj, newAlgorithmType);
}

// settings

void Mach1DecodePositional::setUseBlendMode(bool useBlendMode)
{
	Mach1DecodePositionalCAPI_setUseBlendMode(M1obj, useBlendMode);
}

void Mach1DecodePositional::setIgnoreTopBottom(bool ignoreTopBottom)
{
	Mach1DecodePositionalCAPI_setIgnoreTopBottom(M1obj, ignoreTopBottom);
}

void Mach1DecodePositional::setMuteWhenOutsideObject(bool muteWhenOutsideObject)
{
	Mach1DecodePositionalCAPI_setMuteWhenOutsideObject(M1obj, muteWhenOutsideObject);
}

void Mach1DecodePositional::setMuteWhenInsideObject(bool muteWhenInsideObject)
{
	Mach1DecodePositionalCAPI_setMuteWhenInsideObject(M1obj, muteWhenInsideObject);
}

void Mach1DecodePositional::setUseFalloff(bool useFalloff)
{
	Mach1DecodePositionalCAPI_setUseFalloff(M1obj, useFalloff);
}

void Mach1DecodePositional::setFalloffCurve(float falloffCurve)
{
	Mach1DecodePositionalCAPI_setFalloffCurve(M1obj, falloffCurve);
}

void Mach1DecodePositional::setFalloffCurveBlendMode(float falloffCurveBlendMode)
{
	Mach1DecodePositionalCAPI_setFalloffCurveBlendMode(M1obj, falloffCurveBlendMode);
}

void Mach1DecodePositional::setUseClosestPointRotationMuteInside(bool useClosestPointRotationMuteInside)
{
	Mach1DecodePositionalCAPI_setUseClosestPointRotationMuteInside(M1obj, useClosestPointRotationMuteInside);
}

void Mach1DecodePositional::setUseYawForRotation(bool useYawForRotation)
{
	Mach1DecodePositionalCAPI_setUseYawForRotation(M1obj, useYawForRotation);
}

void Mach1DecodePositional::setUsePitchForRotation(bool usePitchForRotation)
{
	Mach1DecodePositionalCAPI_setUsePitchForRotation(M1obj, usePitchForRotation);
}

void Mach1DecodePositional::setUseRollForRotation(bool useRollForRotation)
{
	Mach1DecodePositionalCAPI_setUseRollForRotation(M1obj, useRollForRotation);
}

// parameters for algo

void Mach1DecodePositional::setCameraPosition(Mach1Point3D point)
{
	Mach1DecodePositionalCAPI_setCameraPosition(M1obj, point);
}

void Mach1DecodePositional::setCameraRotation(Mach1Point3D point)
{
	Mach1DecodePositionalCAPI_setCameraRotation(M1obj, point);
}

void Mach1DecodePositional::setDecoderAlgoPosition(Mach1Point3D point)
{
	Mach1DecodePositionalCAPI_setDecoderAlgoPosition(M1obj, point);
}

void Mach1DecodePositional::setDecoderAlgoRotation(Mach1Point3D point)
{
	Mach1DecodePositionalCAPI_setDecoderAlgoRotation(M1obj, point);
}

void Mach1DecodePositional::setDecoderAlgoScale(Mach1Point3D point)
{
	Mach1DecodePositionalCAPI_setDecoderAlgoScale(M1obj, point);
}

void Mach1DecodePositional::evaluatePostionResults()
{
	Mach1DecodePositionalCAPI_evaluatePostionResults(M1obj);
}

void Mach1DecodePositional::getVolumesWalls(float * result)
{
	Mach1DecodePositionalCAPI_getVolumesWalls(M1obj, result);
}

void Mach1DecodePositional::getVolumesRoom(float * result)
{
	Mach1DecodePositionalCAPI_getVolumesRoom(M1obj, result);
}

float Mach1DecodePositional::getDist()
{
	return Mach1DecodePositionalCAPI_getDist(M1obj);
}

Mach1Point3D Mach1DecodePositional::getCurrentAngle()
{
	return Mach1DecodePositionalCAPI_getCurrentAngle(M1obj);
}

Mach1Point3D Mach1DecodePositional::getVolumeRotation()
{
	return Mach1DecodePositionalCAPI_getVolumeRotation(M1obj);
}

void Mach1DecodePositional::setFilterSpeed(float filterSpeed)
{
	Mach1DecodePositionalCAPI_setFilterSpeed(M1obj, filterSpeed);
}
 