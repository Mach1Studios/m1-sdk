//  Mach1 Spatial SDK
//  Copyright © 2017-2020 Mach1. All rights reserved.

#include "Mach1DecodePositionalCore.h"
#include "Mach1DecodePositionalCAPI.h"

void* Mach1DecodePositionalCAPI_create()
{
	return new Mach1DecodePositionalCore();
}

void Mach1DecodePositionalCAPI_delete(void * M1obj)
{
	if (M1obj != nullptr) {
		delete (Mach1DecodePositionalCore*)M1obj;
		M1obj = nullptr;
	}
}

void Mach1DecodePositionalCAPI_setPlatformType(void * M1obj, Mach1PlatformType type)
{
	if (M1obj != nullptr) {
		((Mach1DecodePositionalCore*)M1obj)->setPlatformType(type);
	}
}

void Mach1DecodePositionalCAPI_setDecodeAlgoType(void * M1obj, Mach1DecodeAlgoType newAlgorithmType)
{
	if (M1obj != nullptr) {
		((Mach1DecodePositionalCore*)M1obj)->setDecodeAlgoType(newAlgorithmType);
	}
}

void Mach1DecodePositionalCAPI_setMuteWhenOutsideObject(void * M1obj, bool muteWhenOutsideObject)
{
	((Mach1DecodePositionalCore*)M1obj)->setMuteWhenOutsideObject(muteWhenOutsideObject);
}

void Mach1DecodePositionalCAPI_setMuteWhenInsideObject(void * M1obj, bool muteWhenInsideObject)
{
	((Mach1DecodePositionalCore*)M1obj)->setMuteWhenInsideObject(muteWhenInsideObject);
}

void Mach1DecodePositionalCAPI_setUseAttenuation(void * M1obj, bool useAttenuation)
{
	((Mach1DecodePositionalCore*)M1obj)->setUseAttenuation(useAttenuation);
}

void Mach1DecodePositionalCAPI_setAttenuationCurve(void * M1obj, float attenuationCurve)
{
	((Mach1DecodePositionalCore*)M1obj)->setAttenuationCurve(attenuationCurve);
}

void Mach1DecodePositionalCAPI_setUsePlaneCalculation(void * M1obj, bool usePlaneCalculation)
{
	((Mach1DecodePositionalCore*)M1obj)->setUsePlaneCalculation(usePlaneCalculation);
}

void Mach1DecodePositionalCAPI_setUseYawForRotation(void * M1obj, bool useYawForRotation)
{
	((Mach1DecodePositionalCore*)M1obj)->setUseYawForRotation(useYawForRotation);
}

void Mach1DecodePositionalCAPI_setUsePitchForRotation(void * M1obj, bool usePitchForRotation)
{
	((Mach1DecodePositionalCore*)M1obj)->setUsePitchForRotation(usePitchForRotation);
}

void Mach1DecodePositionalCAPI_setUseRollForRotation(void * M1obj, bool useRollForRotation)
{
	((Mach1DecodePositionalCore*)M1obj)->setUseRollForRotation(useRollForRotation);
}

void Mach1DecodePositionalCAPI_setListenerPosition(void * M1obj, Mach1Point3D point)
{
	Mach1Point3DCore pnt{ point.x, point.y, point.z };
	((Mach1DecodePositionalCore*)M1obj)->setListenerPosition(&pnt);
}

void Mach1DecodePositionalCAPI_setListenerRotation(void * M1obj, Mach1Point3D point)
{
	Mach1Point3DCore pnt{ point.x, point.y, point.z };
	((Mach1DecodePositionalCore*)M1obj)->setListenerRotation(&pnt);
}

void Mach1DecodePositionalCAPI_setListenerRotationQuat(void * M1obj, Mach1Point4D point)
{
	Mach1Point4DCore pnt{ point.x, point.y, point.z, point.w };
	((Mach1DecodePositionalCore*)M1obj)->setListenerRotationQuat(&pnt);
}

void Mach1DecodePositionalCAPI_setDecoderAlgoPosition(void * M1obj, Mach1Point3D point)
{
	Mach1Point3DCore pnt{ point.x, point.y, point.z };
	((Mach1DecodePositionalCore*)M1obj)->setDecoderAlgoPosition(&pnt);
}

void Mach1DecodePositionalCAPI_setDecoderAlgoRotation(void * M1obj, Mach1Point3D point)
{
	Mach1Point3DCore pnt{ point.x, point.y, point.z };
	((Mach1DecodePositionalCore*)M1obj)->setDecoderAlgoRotation(&pnt);
}

void Mach1DecodePositionalCAPI_setDecoderAlgoRotationQuat(void * M1obj, Mach1Point4D point)
{
	Mach1Point4DCore pnt{ point.x, point.y, point.z, point.w };
	((Mach1DecodePositionalCore*)M1obj)->setDecoderAlgoRotationQuat(&pnt);
}

void Mach1DecodePositionalCAPI_setDecoderAlgoScale(void * M1obj, Mach1Point3D point)
{
	Mach1Point3DCore pnt{ point.x, point.y, point.z };
	((Mach1DecodePositionalCore*)M1obj)->setDecoderAlgoScale(&pnt);
}

void Mach1DecodePositionalCAPI_evaluatePositionResults(void * M1obj)
{
	((Mach1DecodePositionalCore*)M1obj)->evaluatePositionResults();
}

void Mach1DecodePositionalCAPI_getCoefficients(void * M1obj, float* result)
{
	((Mach1DecodePositionalCore*)M1obj)->getCoefficients(result);
}

float Mach1DecodePositionalCAPI_getDist(void * M1obj)
{
	return ((Mach1DecodePositionalCore*)M1obj)->getDist();
}

Mach1Point3D Mach1DecodePositionalCAPI_getCurrentAngle(void * M1obj)
{
	Mach1Point3DCore angle = ((Mach1DecodePositionalCore*)M1obj)->getCurrentAngle();
	return Mach1Point3D{ angle.x, angle.y, angle.z };
}

Mach1Point3D Mach1DecodePositionalCAPI_getCoefficientsRotation(void * M1obj)
{
	Mach1Point3DCore angle = ((Mach1DecodePositionalCore*)M1obj)->getCoefficientsRotation();
	return Mach1Point3D{ angle.x, angle.y, angle.z };
}

void Mach1DecodePositionalCAPI_setFilterSpeed(void * M1obj, float filterSpeed)
{
	((Mach1DecodePositionalCore*)M1obj)->setFilterSpeed(filterSpeed);
}

Mach1Point3D Mach1DecodePositionalCAPI_getClosestPointOnPlane(void * M1obj)
{
	Mach1Point3DCore p = ((Mach1DecodePositionalCore*)M1obj)->getClosestPointOnPlane();
	return Mach1Point3D{ p.x, p.y, p.z };
}

long Mach1DecodePositionalCAPI_getLastCalculationTime(void * M1obj)
{
	return ((Mach1DecodePositionalCore*)M1obj)->getLastCalculationTime();
}

/* Experimental Functions/Features */
void Mach1DecodePositionalCAPI_setUseBlendMode(void * M1obj, bool useBlendMode)
{
	((Mach1DecodePositionalCore*)M1obj)->setUseBlendMode(useBlendMode);
}
void Mach1DecodePositionalCAPI_setIgnoreTopBottom(void * M1obj, bool ignoreTopBottom)
{
	((Mach1DecodePositionalCore*)M1obj)->setIgnoreTopBottom(ignoreTopBottom);
}
void Mach1DecodePositionalCAPI_setAttenuationCurveBlendMode(void * M1obj, float attenuationCurveBlendMode)
{
	((Mach1DecodePositionalCore*)M1obj)->setAttenuationCurveBlendMode(attenuationCurveBlendMode);
}
void Mach1DecodePositionalCAPI_getCoefficientsInterior(void * M1obj, float* result)
{
	((Mach1DecodePositionalCore*)M1obj)->getCoefficientsInterior(result);
}
