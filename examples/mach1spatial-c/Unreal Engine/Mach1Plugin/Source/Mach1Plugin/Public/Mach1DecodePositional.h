//  Mach1 SDK
//  Copyright © 2017 Mach1. All rights reserved.
//
//  Header file

#pragma once

#ifndef Mach1DecodePositional_h
#define Mach1DecodePositional_h

#include <vector>
#include <cmath>
#include <chrono>
#include <string>
#include <algorithm>

#include "Mach1DecodePositionalCAPI.h"

//////////////

class Mach1DecodePositional
{
	void* M1obj;

public:
	
	Mach1DecodePositional();
	~Mach1DecodePositional();

	void setPlatformType(Mach1PlatformType platformType);
	void setDecodeAlgoType(Mach1DecodeAlgoType newAlgorithmType);

	// settings
	void setUseBlendMode(bool useBlendMode);
	void setIgnoreTopBottom(bool ignoreTopBottom);

	void setMuteWhenOutsideObject(bool muteWhenOutsideObject);
	void setMuteWhenInsideObject(bool muteWhenInsideObject);

	void setUseFalloff(bool useFalloff);
	void setFalloffCurve(float falloffCurve);
	void setFalloffCurveBlendMode(float falloffCurveBlendMode);

	void setUseClosestPointRotationMuteInside(bool useClosestPointRotationMuteInside);

	void setUseYawForRotation(bool useYawForRotation);
	void setUsePitchForRotation(bool usePitchForRotation);
	void setUseRollForRotation(bool useRollForRotation);

	// parameters for algo
	void setCameraPosition(Mach1Point3D point);
	void setCameraRotation(Mach1Point3D point);
	void setCameraRotationQuat(Mach1Point4D quat);
	void setDecoderAlgoPosition(Mach1Point3D point);
	void setDecoderAlgoRotation(Mach1Point3D point);
	void setDecoderAlgoRotationQuat(Mach1Point4D quat);
	void setDecoderAlgoScale(Mach1Point3D point);

	void evaluatePostionResults();
	void getVolumesWalls(float* result);
	void getVolumesRoom(float* result);
	float getDist();
	Mach1Point3D getCurrentAngle();
	Mach1Point3D getVolumeRotation();
	void setFilterSpeed(float filterSpeed);
};
  
#endif /* Mach1DecodePositional_h */
