//  Mach1 Spatial SDK
//  Copyright © 2017-2020 Mach1. All rights reserved.

#include "Mach1DecodeMINCore.h"
#include "Mach1DecodeMINCAPI.h"

void* Mach1DecodeCAPI_create()
{
	return new Mach1DecodeCore();
}

void Mach1DecodeCAPI_delete(void* M1obj)
{
	if (M1obj != nullptr) {
		delete (Mach1DecodeCore*)M1obj;
		M1obj = nullptr;
	}
}

void Mach1DecodeCAPI_setDecodeAlgoType(void* M1obj, enum Mach1DecodeAlgoType newAlgorithmType)
{
	((Mach1DecodeCore*)M1obj)->setDecodeAlgoType(newAlgorithmType);
}

Mach1DecodeAlgoType Mach1DecodeCAPI_getDecodeAlgoType(void* M1obj)
{
	return ((Mach1DecodeCore*)M1obj)->getDecodeAlgoType();
}

void Mach1DecodeCAPI_decode(void * M1obj, float Yaw, float Pitch, float Roll, float * result, int bufferSize, int sampleIndex)
{
	((Mach1DecodeCore*)M1obj)->decode(Yaw, Pitch, Roll, result, bufferSize, sampleIndex);
}

void Mach1DecodeCAPI_decodeCoeffs(void* M1obj, float* result, int bufferSize, int sampleIndex)
{
	((Mach1DecodeCore*)M1obj)->decodeCoeffs(result, bufferSize, sampleIndex);
}

void Mach1DecodeCAPI_decodeCoeffsUsingTranscodeMatrix(void * M1obj, float * matrix, int channels, float * result, int bufferSize, int sampleIndex)
{
	((Mach1DecodeCore*)M1obj)->decodeCoeffsUsingTranscodeMatrix(M1obj, matrix, channels, result, bufferSize, sampleIndex);
}

void Mach1DecodeCAPI_beginBuffer(void* M1obj)
{
	((Mach1DecodeCore*)M1obj)->beginBuffer();
}

void Mach1DecodeCAPI_endBuffer(void* M1obj)
{
	((Mach1DecodeCore*)M1obj)->endBuffer();
}

int Mach1DecodeCAPI_getFormatChannelCount(void* M1obj)
{
	return ((Mach1DecodeCore*)M1obj)->getFormatChannelCount();
}

void Mach1DecodeCAPI_setRotation(void* M1obj, Mach1Point3D newRotationFromMinusOnetoOne)
{
	((Mach1DecodeCore*)M1obj)->setRotation({ newRotationFromMinusOnetoOne.x, newRotationFromMinusOnetoOne.y, newRotationFromMinusOnetoOne.z });
}

void Mach1DecodeCAPI_setRotationDegrees(void* M1obj, Mach1Point3D newRotationDegrees)
{
	((Mach1DecodeCore*)M1obj)->setRotationDegrees({ newRotationDegrees.x, newRotationDegrees.y, newRotationDegrees.z });
}

void Mach1DecodeCAPI_setRotationRadians(void* M1obj, Mach1Point3D rotation)
{
	((Mach1DecodeCore*)M1obj)->setRotationRadians({ rotation.x, rotation.y, rotation.z });
}

void Mach1DecodeCAPI_setRotationQuat(void* M1obj, Mach1Point4D newRotationQuat)
{
	((Mach1DecodeCore*)M1obj)->setRotationQuat({ newRotationQuat.x, newRotationQuat.y, newRotationQuat.z, newRotationQuat.w });
}

Mach1Point3D Mach1DecodeCAPI_getCurrentAngle(void* M1obj)
{
	Mach1Point3DCore angle = ((Mach1DecodeCore*)M1obj)->getCurrentAngle();
	return Mach1Point3D { angle.x, angle.y, angle.z };
}
