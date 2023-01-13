//  Mach1 Spatial SDK
//  Copyright © 2017-2020 Mach1. All rights reserved.

/*
DISCLAIMER:
This header file is not an example of use but an decoder that will require periodic
updates and should not be integrated in sections but remain as an update-able factored file.
*/

#pragma once

#include "Mach1DecodeCore.h"

#include <iostream>
#include <string>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/vec3.hpp>  
#include <glm/vec4.hpp> 
#include <glm/mat4x4.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#ifndef RAD_TO_DEG_F
#define RAD_TO_DEG_F (180.0f/PI_F)
#endif

#ifndef DEG_TO_RAD_F
#define DEG_TO_RAD_F (PI_F/180.0f)
#endif

#ifndef PI_F
#define PI_F       3.14159265358979323846f
#endif 

#ifndef PI
#define PI       3.14159265358979323846
#endif 

class Mach1DecodePositionalCore {

private:
	Mach1DecodeCore mach1Decode;

	bool useFalloff = false;
	float falloffCurve;
	float falloffCurveBlendMode;

	bool useBlendMode = false;
	bool ignoreTopBottom = true;

	bool muteWhenInsideObject = false;
	bool muteWhenOutsideObject = false;
	bool useClosestPointRotationMuteInside = false;


	bool useYawForRotation = true;
	bool usePitchForRotation = true;
	bool useRollForRotation = true;

	glm::vec3 cameraPosition;
	glm::quat cameraRotation;

	glm::vec3 soundPosition;
	glm::quat soundRotation;
	glm::vec3 soundScale;

	static void convertPositionToMach1(Mach1PlatformType platformType, float* X, float* Y, float* Z);
	static void convertPositionToPlatform(Mach1PlatformType platformType, float* X, float* Y, float* Z);

	static float ClosestPointOnBox(glm::vec3 point, glm::vec3 center, glm::vec3 axis0, glm::vec3 axis1, glm::vec3 axis2, glm::vec3 extents, glm::vec3& closestPoint);
	static bool Clip(float denom, float numer, float& t0, float& t1);
	static int DoClipping(float t0, float t1, glm::vec3 origin, glm::vec3 direction, glm::vec3 center, glm::vec3 axis0, glm::vec3 axis1, glm::vec3 axis2, glm::vec3 extents, bool solid, glm::vec3& point0, glm::vec3& point1);
	static glm::vec3 GetEuler(glm::quat q1);

	Mach1PlatformType platformType;
	Mach1DecodeAlgoType algorithmType;

	glm::vec3 GetRightVector();
	glm::vec3 GetUpVector();
	glm::vec3 GetForwardVector();

	float volumeWalls;
	float volumeRoom;
	float dist;
	glm::vec3 eulerAngles;

	std::vector<float> volumes;

	milliseconds ms;
	long timeLastCalculation;

	glm::vec3 closestPointOnPlane;

public:
	Mach1DecodePositionalCore();

	void setDecodeAlgoType(Mach1DecodeAlgoType type);
	void setPlatformType(Mach1PlatformType type);

	// settings
	void setUseBlendMode(bool useBlendMode);

	void setUseAttenuation(bool useAttenuation);
	void setAttenuationCurve(float attenuationCurve);
	void setAttenuationCurveBlendMode(float attenuationCurveBlendMode);

	void setIgnoreTopBottom(bool ignoreTopBottom);

	void setMuteWhenOutsideObject(bool muteWhenOutsideObject);
	void setMuteWhenInsideObject(bool muteWhenInsideObject);

	void setUsePlaneCalculation(bool usePlaneCalculation);

	void setUseYawForRotation(bool useYawForRotation);
	void setUsePitchForRotation(bool usePitchForRotation);
	void setUseRollForRotation(bool useRollForRotation);

	// parameters for algo
	void setListenerPosition(Mach1Point3DCore* pos);
	void setListenerRotation(Mach1Point3DCore* euler);
	void setListenerRotationQuat(Mach1Point4DCore* quat);

	void setDecoderAlgoPosition(Mach1Point3DCore* pos);
	void setDecoderAlgoRotation(Mach1Point3DCore* euler);
	void setDecoderAlgoRotationQuat(Mach1Point4DCore* quat);
	void setDecoderAlgoScale(Mach1Point3DCore* scale);

	void evaluatePositionResults();

	void getCoefficients(float *result);
	void getCoefficientsInterior(float *result);

	float getDist();

	Mach1Point3DCore getCurrentAngle();
	Mach1Point3DCore getCoefficientsRotation();
	Mach1Point3DCore getCoefficientsRotationInternal();

	Mach1Point3DCore getClosestPointOnPlane();

	void setFilterSpeed(float filterSpeed);

	long getCurrentTime();
	long getLastCalculationTime();
};
