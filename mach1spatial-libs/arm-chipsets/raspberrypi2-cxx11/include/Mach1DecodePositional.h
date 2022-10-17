//  Mach1 Spatial SDK
//  Copyright © 2017-2020 Mach1. All rights reserved.

#pragma once

#ifndef Mach1DecodePositional_h
#define Mach1DecodePositional_h

#include <vector>
#include <cmath>
#include <chrono>
#include <string>
#include <algorithm>

#include "Mach1DecodePositionalCAPI.h"

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

	void setUseAttenuation(bool useAttenuation);
	void setAttenuationCurve(float attenuationCurve);
	void setAttenuationCurveBlendMode(float attenuationCurveBlendMode);

	void setUsePlaneCalculation(bool usePlaneCalculation);

	void setUseYawForRotation(bool useYawForRotation);
	void setUsePitchForRotation(bool usePitchForRotation);
	void setUseRollForRotation(bool useRollForRotation);

	void setListenerPosition(Mach1Point3D point);
	void setListenerRotation(Mach1Point3D point);
	void setListenerRotationQuat(Mach1Point4D quat);
	void setDecoderAlgoPosition(Mach1Point3D point);
	void setDecoderAlgoRotation(Mach1Point3D point);
	void setDecoderAlgoRotationQuat(Mach1Point4D quat);
	void setDecoderAlgoScale(Mach1Point3D point);

	void evaluatePositionResults();

#ifndef  __EMSCRIPTEN__ 
	void getCoefficients(float* result);
	void getCoefficientsInterior(float* result);
#endif
	void getCoefficients(std::vector<float>& result);
	void getCoefficientsInterior(std::vector<float>& result);

	float getDist();
	Mach1Point3D getCurrentAngle();
	Mach1Point3D getCoefficientsRotation();
	void setFilterSpeed(float filterSpeed);

	Mach1Point3D getClosestPointOnPlane();
};
  
#endif /* Mach1DecodePositional_h */
