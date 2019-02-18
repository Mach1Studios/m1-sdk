//  Mach1 SDK
//  Copyright © 2018 Mach1. All rights reserved.
//
//  Implementation

/*
DISCLAIMER:
This header file is not an example of use but an decoder that will require periodic
updates and should not be integrated in sections but remain as an update-able factored file.
*/

#include "Mach1DecodePositionalCore.h"

float Mach1DecodePositionalCore::ClosestPointOnBox(glm::vec3 point, glm::vec3 center, glm::vec3 axis0, glm::vec3 axis1, glm::vec3 axis2, glm::vec3 extents, glm::vec3 & closestPoint)
{
	glm::vec3 vector = point - center;
	float num = 0.0f;

	float num0 = glm::dot(vector, axis0);
	if (num0 < -extents.x)
	{
		num += powf(num0 + extents.x, 2);
		num0 = -extents.x;
	}
	else if (num0 > extents.x)
	{
		num += powf(num0 - extents.x, 2);
		num0 = extents.x;
	}

	float num1 = glm::dot(vector, axis1);
	if (num1 < -extents.y)
	{
		num += powf(num1 + extents.y, 2);
		num1 = -extents.y;
	}
	else if (num1 > extents.y)
	{
		num += powf(num1 - extents.y, 2);
		num1 = extents.y;
	}

	float num2 = glm::dot(vector, axis2);
	if (num2 < -extents.z)
	{
		num += powf(num2 + extents.z, 2);
		num2 = -extents.z;
	}
	else if (num2 > extents.z)
	{
		num += powf(num2 - extents.z, 2);
		num2 = extents.z;
	}
	closestPoint = center + num0 * axis0 + num1 * axis1 + num2 * axis2;

	return sqrt(num);
}

bool Mach1DecodePositionalCore::Clip(float denom, float numer, float & t0, float & t1)
{
	if ((double)denom > 0.0)
	{
		if ((double)numer > (double)denom * (double)t1)
			return false;
		if ((double)numer > (double)denom * (double)t0)
			t0 = numer / denom;
		return true;
	}
	if ((double)denom >= 0.0)
		return (double)numer <= 0.0;
	if ((double)numer > (double)denom * (double)t0)
		return false;
	if ((double)numer > (double)denom * (double)t1)
		t1 = numer / denom;
	return true;
}

int Mach1DecodePositionalCore::DoClipping(float t0, float t1, glm::vec3 origin, glm::vec3 direction, glm::vec3 center, glm::vec3 axis0, glm::vec3 axis1, glm::vec3 axis2, glm::vec3 extents, bool solid, glm::vec3 & point0, glm::vec3 & point1)
{
	glm::vec3 vector = origin - center;
	glm::vec3 vector2 = glm::vec3(glm::dot(vector, axis0), glm::dot(vector, axis1), glm::dot(vector, axis2));
	glm::vec3 vector3 = glm::vec3(glm::dot(direction, axis0), glm::dot(direction, axis1), glm::dot(direction, axis2));

	float num = t0;
	float num2 = t1;

	int quantity = 0;

	bool flag = Clip(vector3.x, -vector2.x - extents.x, t0, t1) && Clip(-vector3.x, vector2.x - extents.x, t0, t1) && Clip(vector3.y, -vector2.y - extents.y, t0, t1) && Clip(-vector3.y, vector2.y - extents.y, t0, t1) && Clip(vector3.z, -vector2.z - extents.z, t0, t1) && Clip(-vector3.z, vector2.z - extents.z, t0, t1);
	if (flag && (solid || t0 != num || t1 != num2))
	{
		if (t1 > t0)
		{
			quantity = 2;
			point0 = origin + t0 * direction;
			point1 = origin + t1 * direction;
		}
		else
		{

			quantity = 1;
			point0 = origin + t0 * direction;
			point1 = glm::vec3();
		}
	}
	else
	{
		quantity = 0;
		point0 = glm::vec3();
		point1 = glm::vec3();
	}

	return quantity;
}

glm::vec3 Mach1DecodePositionalCore::GetEuler(glm::quat q1)
{
	float test = q1.x * q1.y + q1.z * q1.w;
	if (test > 0.499) // singularity at north pole
	{
		return glm::vec3(
			0,
			2 * atan2(q1.x, q1.w),
			PI / 2
		) * RAD_TO_DEG_F;
	}
	if (test < -0.499) // singularity at south pole
	{
		return glm::vec3(
			0,
			-2 * atan2(q1.x, q1.w),
			-PI / 2
		) * RAD_TO_DEG_F;
	}
	float sqx = q1.x * q1.x;
	float sqy = q1.y * q1.y;
	float sqz = q1.z * q1.z;

	return glm::vec3(
		atan2(2.0f * q1.x * q1.w - 2 * q1.y * q1.z, 1.0f - 2.0f * sqx - 2.0f * sqz),
		atan2(2.0f * q1.y * q1.w - 2 * q1.x * q1.z, 1.0f - 2.0f * sqy - 2.0f * sqz),
		sin(2.0f * test)
	) * RAD_TO_DEG_F;
}

glm::vec3 Mach1DecodePositionalCore::GetRightVector()
{
	switch (platformType)
	{
	case Mach1PlatformUnity:
		return glm::vec3(1, 0, 0);
	case Mach1PlatformUE:
		return glm::vec3(0, 1, 0);
	default:
		return glm::vec3(1, 0, 0);
	}
}

glm::vec3 Mach1DecodePositionalCore::GetUpVector()
{
	switch (platformType)
	{
	case Mach1PlatformUnity:
		return glm::vec3(0, 1, 0);
	case Mach1PlatformUE:
		return glm::vec3(0, 0, 1);
	default:
		return glm::vec3(0, 1, 0);
	}
}

glm::vec3 Mach1DecodePositionalCore::GetForwardVector()
{
	switch (platformType)
	{
	case Mach1PlatformUnity:
		return glm::vec3(0, 0, 1);
	case Mach1PlatformUE:
		return glm::vec3(1, 0, 0);
	default:
		return glm::vec3(0, 0, 1);
	}
}

Mach1DecodePositionalCore::Mach1DecodePositionalCore()
{
	falloffCurve = 1;
	falloffCurveBlendMode = 1;

	setDecodeAlgoType(Mach1DecodeAlgoType::Mach1DecodeAlgoSpatial);
}

void Mach1DecodePositionalCore::setDecodeAlgoType(Mach1DecodeAlgoType type)
{
	algorithmType = type;
	mach1Decode.setDecodeAlgoType(type);
}

void Mach1DecodePositionalCore::setPlatformType(Mach1PlatformType type)
{
	platformType = type;
	mach1Decode.setPlatformType(type);
}

void Mach1DecodePositionalCore::setUseBlendMode(bool useBlendMode)
{
	this->useBlendMode = useBlendMode;
}

void Mach1DecodePositionalCore::setFalloffCurve(float falloffCurve)
{
	this->falloffCurve = falloffCurve;
}

void Mach1DecodePositionalCore::setFalloffCurveBlendMode(float falloffCurveBlendMode)
{
	this->falloffCurveBlendMode = falloffCurveBlendMode;
}

void Mach1DecodePositionalCore::setIgnoreTopBottom(bool ignoreTopBottom)
{
	this->ignoreTopBottom = ignoreTopBottom;
}

void Mach1DecodePositionalCore::setMuteWhenOutsideObject(bool muteWhenOutsideObject)
{
	this->muteWhenOutsideObject = muteWhenOutsideObject;
}

void Mach1DecodePositionalCore::setMuteWhenInsideObject(bool muteWhenInsideObject)
{
	this->muteWhenOutsideObject = muteWhenOutsideObject;
}

void Mach1DecodePositionalCore::setUseFalloff(bool useFalloff)
{
	this->useFalloff = useFalloff;
}

void Mach1DecodePositionalCore::setUseClosestPointRotationMuteInside(bool useClosestPointRotationMuteInside)
{
	this->useClosestPointRotationMuteInside = useClosestPointRotationMuteInside;
}

void Mach1DecodePositionalCore::setUseYawForRotation(bool useYawForRotation)
{
	this->useYawForRotation = useYawForRotation;
}

void Mach1DecodePositionalCore::setUsePitchForRotation(bool usePitchForRotation)
{
	this->usePitchForRotation = usePitchForRotation;
}

void Mach1DecodePositionalCore::setUseRollForRotation(bool useRollForRotation)
{
	this->useRollForRotation = useRollForRotation;
}

void Mach1DecodePositionalCore::setCameraPosition(Mach1Point3DCore * pos) {
	cameraPosition = glm::vec3(pos->x, pos->y, pos->z);
}

void Mach1DecodePositionalCore::setCameraRotation(Mach1Point3DCore * euler) {
	/*
		Mach1Point3DCore angle(euler->x, euler->y, euler->z);
		mach1Decode.convertAnglesToMach1(&angle.x, &angle.y, &angle.z);
		cameraRotation = glm::quat(glm::vec3(angle.x * DEG_TO_RAD_F, angle.y * DEG_TO_RAD_F, angle.z * DEG_TO_RAD_F));
	*/
	cameraRotation = glm::quat(glm::vec3(euler->x * DEG_TO_RAD_F, euler->y * DEG_TO_RAD_F, euler->z * DEG_TO_RAD_F));
}

void Mach1DecodePositionalCore::setCameraRotationQuat(Mach1Point4DCore * quat)
{
	cameraRotation = glm::quat(quat->w, quat->x, quat->y, quat->z);
}

void Mach1DecodePositionalCore::setDecoderAlgoPosition(Mach1Point3DCore * pos) {
	soundPosition = glm::vec3(pos->x, pos->y, pos->z);
}

void Mach1DecodePositionalCore::setDecoderAlgoRotation(Mach1Point3DCore * euler) {
	/*
		Mach1Point3DCore angle(euler->x, euler->y, euler->z);
		mach1Decode.convertAnglesToMach1(&angle.x, &angle.y, &angle.z);
		soundRotation = glm::quat(glm::vec3(angle.x * DEG_TO_RAD_F, angle.y * DEG_TO_RAD_F, angle.z * DEG_TO_RAD_F));
	*/
	soundRotation = glm::quat(glm::vec3(euler->x * DEG_TO_RAD_F, euler->y * DEG_TO_RAD_F, euler->z * DEG_TO_RAD_F));
}

void Mach1DecodePositionalCore::setDecoderAlgoRotationQuat(Mach1Point4DCore * quat)
{
	soundRotation = glm::quat(quat->w, quat->x, quat->y, quat->z);
}

void Mach1DecodePositionalCore::setDecoderAlgoScale(Mach1Point3DCore * scale) {
	soundScale = glm::vec3(scale->x, scale->y, scale->z);
}

void Mach1DecodePositionalCore::setAttenuationCurve(float * curve) {
	// dummy
}

void Mach1DecodePositionalCore::evaluatePositionResults() {

	volumeWalls = 1.0f;
	volumeRoom = 0.0f;

	dist = 0;

	// Find closest point
	glm::vec3 point = soundPosition;

	glm::vec3 outsideClosestPoint;
	//glm::vec3 insidePoint0, insidePoint1;


	if (ignoreTopBottom && useBlendMode)
	{
		if (platformType == Mach1PlatformUE)
		{
			cameraPosition.z = soundPosition.z;
		}
		else // if (platformType == Mach1PlatformUnity)
		{
			cameraPosition.y = soundPosition.y;
		}

	}

	glm::vec3 soundRightVector = soundRotation * glm::vec3(1, 0, 0);// GetRightVector(); // right
	glm::vec3 soundUpVector = soundRotation * glm::vec3(0, 1, 0); // up
	glm::vec3 soundForwardVector = soundRotation * glm::vec3(0, 0, 1); // forward

	bool isOutside = (ClosestPointOnBox(cameraPosition, soundPosition, soundRightVector, soundUpVector, soundForwardVector, soundScale / 2.0f, outsideClosestPoint) > 0);
	bool hasSoundOutside = isOutside && !muteWhenOutsideObject;
	bool hasSoundInside = !isOutside && !muteWhenInsideObject;

	if (hasSoundOutside && useClosestPointRotationMuteInside) // useClosestPointRotation
	{
		point = outsideClosestPoint;

		dist = glm::distance(cameraPosition, point);

		if (useFalloff)
		{
			volumeWalls = volumeWalls * falloffCurve;
		}

	}
	else if (hasSoundInside && useBlendMode) // && DoClipping(0, float.MaxValue, cameraPosition, (cameraPosition - soundPosition).normalized, soundPosition, gameObject.transform.right, gameObject.transform.up, gameObject.transform.forward, gameObject.transform.localScale / 2, true, out insidePoint0, out insidePoint1) == 2)
	{
		glm::mat4 identity(1.0f); // construct identity matrix
		glm::mat4 translate = glm::translate(identity, soundPosition);
		glm::mat4 rotate = glm::toMat4(soundRotation);
		glm::mat4 scale = glm::scale(identity, soundScale);

		glm::mat4 mat = glm::inverse(translate * rotate * scale);

		glm::vec3 p0 = 2.0f * (mat * glm::vec4(cameraPosition, 1.0f)); // InverseTransformPoint

		dist = 1 - std::max(abs(p0.x), std::max(abs(p0.y), abs(p0.z)));

		if (useFalloff)
		{
			volumeWalls = volumeWalls * falloffCurveBlendMode;
		}

		volumeRoom = 1 - volumeWalls;
	}
	else if (hasSoundOutside || hasSoundInside) // useCenterPointRotation
	{
		dist = glm::distance(cameraPosition, point);

		if (useFalloff)
		{
			if (hasSoundOutside)
			{
				volumeWalls = volumeWalls * falloffCurve;
			}
			if (useBlendMode)
			{
				volumeWalls = volumeWalls * falloffCurveBlendMode;
			}
		}
	}
	else
	{
		volumeWalls = 0;
		volumeRoom = 0;
	}

	glm::vec3 dir = point - cameraPosition;

	// Compute rotation for sound
	// http://www.aclockworkberry.com/world-coordinate-systems-in-3ds-max-unity-and-unreal-engine/
	glm::quat quat;
	if (platformType == Mach1PlatformUE)
	{
		quat = glm::quatLookAtRH(glm::normalize(dir), GetUpVector());
	}
	else if (platformType == Mach1PlatformUnity)
	{
		quat = glm::quatLookAtLH(glm::normalize(dir), GetUpVector());
	}
	quat = glm::inverse(quat); 
	quat = quat * soundRotation;

	glm::vec3 quatEulerAngles = glm::eulerAngles(quat);

	bool useXForRotation = true;
	bool useYForRotation = true;
	bool useZForRotation = true;

	if (platformType == Mach1PlatformUE)
	{
		useXForRotation = useRollForRotation;
		useYForRotation = usePitchForRotation;
		useZForRotation = useYawForRotation;
	}
	else if (platformType == Mach1PlatformUnity)
	{
		useXForRotation = usePitchForRotation;
		useYForRotation = useYawForRotation;
		useZForRotation = useRollForRotation;
	}

	quat = glm::quat(glm::vec3(useXForRotation ? quatEulerAngles.x : 0, useYForRotation ? quatEulerAngles.y : 0, useZForRotation ? quatEulerAngles.z : 0));
	quat *= cameraRotation;

	eulerAngles = GetEuler(quat);

	// SoundAlgorithm
	volumes = mach1Decode.decode(eulerAngles.x, eulerAngles.y, eulerAngles.z);
	mach1Decode.beginBuffer();
}

void Mach1DecodePositionalCore::getVolumesWalls(float *result)
{
	for (int i = 0; i < volumes.size(); i++)
	{
		result[i] = volumeWalls * volumes[i];
	}
}

void Mach1DecodePositionalCore::getVolumesRoom(float *result)
{
	for (int i = 0; i < volumes.size(); i++)
	{
		result[i] = volumeRoom * volumes[i];
	}
}

Mach1Point3DCore Mach1DecodePositionalCore::getVolumeRotation()
{
	return Mach1Point3DCore{ eulerAngles.x , eulerAngles.y, eulerAngles.z };
}

float Mach1DecodePositionalCore::getDist()
{
	return dist;
}

Mach1Point3DCore Mach1DecodePositionalCore::getCurrentAngle()
{
	Mach1Point3DCore angle = mach1Decode.getCurrentAngle();
	mach1Decode.convertAnglesToPlatform(&angle.x, &angle.y, &angle.z);
	return angle;
}

void Mach1DecodePositionalCore::setFilterSpeed(float filterSpeed)
{
	mach1Decode.setFilterSpeed(filterSpeed);
}
