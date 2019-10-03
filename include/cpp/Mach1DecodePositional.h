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

/* DEPRECATED START */
	[[deprecated("setUseFalloff is deprecated, please use setUseAttenuation instead")]]
	void setUseFalloff(bool useFalloff);
	[[deprecated("setFalloffCurve is deprecated, please use setAttenuationCurve instead")]]
	void setFalloffCurve(float falloffCurve);
	[[deprecated("setFalloffCurveBlendMode is deprecated, please use setAttenuationCurveBlendMode instead")]]
	void setFalloffCurveBlendMode(float falloffCurveBlendMode);
	[[deprecated("setUseClosestPointRotationMuteInside is deprecated, please use setUsePlaneCalculation instead")]]
	void setUseClosestPointRotationMuteInside(bool useClosestPointRotationMuteInside);
	[[deprecated("setCameraPosition is deprecated, please use setListenerPosition instead")]]
	void setCameraPosition(Mach1Point3D point);
	[[deprecated("setCameraRotation is deprecated, please use setListenerRotation instead")]]
	void setCameraRotation(Mach1Point3D point);
	[[deprecated("setCameraRotationQuat is deprecated, please use setListenerRotationQuat instead")]]
	void setCameraRotationQuat(Mach1Point4D quat);

#ifndef  __EMSCRIPTEN__ 
	[[deprecated("getVolumesWalls is deprecated, please use getCoefficients instead")]]
	void getVolumesWalls(float* result);
	[[deprecated("getVolumesRoom is deprecated, please use getCoefficientsInterior instead")]]
	void getVolumesRoom(float* result);
#endif
	[[deprecated("getVolumesWalls is deprecated, please use getCoefficients instead")]]
	void getVolumesWalls(std::vector<float>& result);
	[[deprecated("getVolumesRoom is deprecated, please use getCoefficientsInterior instead")]]
	void getVolumesRoom(std::vector<float>& result);

	[[deprecated("getVolumeRotation is depracted, please use getCoefficientsRotation instead")]]
	Mach1Point3D getVolumeRotation();
/* DEPRECATED END */
};
  
#endif /* Mach1DecodePositional_h */
