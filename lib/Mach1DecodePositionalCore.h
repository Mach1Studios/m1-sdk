//  Mach1 SDK
//  Copyright © 2017 Mach1. All rights reserved.
//
//  Header file

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

#ifndef PI_F
#define PI_F       3.14159265358979323846f
#endif 

class Mach1DecodePositionalCore : public Mach1DecodeCore {

private:

	class AnimationCurve
	{
	public:
		float Evaluate(float p)
		{
			// dummy
			return 1.0;
		}
	};

	bool useFalloff = false;
	AnimationCurve falloffCurve;
	AnimationCurve blendModeFalloffCurve;

	bool useBlendMode = false;
	bool ignoreTopBottom = true;


	bool muteWhenInsideObject = false;
	bool muteWhenOutsideObject = false;
	bool useClosestPointRotationMuteInside = false;


	bool useYawForRotation = true;
	bool usePitchForRotation = true;
	bool useRollForRotation = true;


	bool debug = true;

	glm::vec3 cameraPosition;
	glm::quat cameraRotation;

	glm::vec3 soundPosition;
	glm::quat soundRotation;
	glm::vec3 soundScale;

	static float ClosestPointOnBox(glm::vec3 point, glm::vec3 center, glm::vec3 axis0, glm::vec3 axis1, glm::vec3 axis2, glm::vec3 extents, glm::vec3& closestPoint);
	static bool Clip(float denom, float numer, float& t0, float& t1);
	static int DoClipping(float t0, float t1, glm::vec3 origin, glm::vec3 direction, glm::vec3 center, glm::vec3 axis0, glm::vec3 axis1, glm::vec3 axis2, glm::vec3 extents, bool solid, glm::vec3& point0, glm::vec3& point1);
	static glm::vec3 GetEuler(glm::quat q1);

public:

	void setCameraPosition(Mach1Point3DCore* pos);
	void setCameraRotation(Mach1Point3DCore* euler);
	void setDecoderAlgoPosition(Mach1Point3DCore* pos);
	void setDecoderAlgoRotation(Mach1Point3DCore* euler);
	void setDecoderAlgoScale(Mach1Point3DCore* scale);
	void setAttenuationCurve(float* curve);

	std::vector<float> getPostionResults();
};
