//  Mach1 Spatial SDK
//  Copyright © 2017 Mach1. All rights reserved.

#include "Mach1DecodeCAPI.h"
#include "Mach1DecodeCore.h"

void *Mach1DecodeCAPI_create() {
    return new M1DecodeCore();
}

void Mach1DecodeCAPI_delete(void *M1obj) {
    if (M1obj != nullptr) {
        delete (M1DecodeCore *)M1obj;
        M1obj = nullptr;
    }
}

void Mach1DecodeCAPI_setDecodeMode(void *M1obj, enum Mach1DecodeMode mode) {
    ((M1DecodeCore *)M1obj)->setDecodeMode(mode);
}

void Mach1DecodeCAPI_setPlatformType(void *M1obj, enum Mach1PlatformType type) {
    ((M1DecodeCore *)M1obj)->setPlatformType(type);
}

Mach1DecodeMode Mach1DecodeCAPI_getDecodeMode(void *M1obj) {
    return ((M1DecodeCore *)M1obj)->getDecodeMode();
}

Mach1PlatformType Mach1DecodeCAPI_getPlatformType(void *M1obj) {
    return ((M1DecodeCore *)M1obj)->getPlatformType();
}

void Mach1DecodeCAPI_decode(void *M1obj, float Yaw, float Pitch, float Roll, float *result, int bufferSize, int sampleIndex) {
    ((M1DecodeCore *)M1obj)->decode(Yaw, Pitch, Roll, result, bufferSize, sampleIndex);
}

void Mach1DecodeCAPI_decodeCoeffs(void *M1obj, float *result, int bufferSize, int sampleIndex) {
    ((M1DecodeCore *)M1obj)->decodeCoeffs(result, bufferSize, sampleIndex);
}

void Mach1DecodeCAPI_decodePannedCoeffs(void *M1obj, float *result, int bufferSize, int sampleIndex, bool applyPanLaw) {
    ((M1DecodeCore *)M1obj)->decodePannedCoeffs(result, bufferSize, sampleIndex, applyPanLaw);
}

void Mach1DecodeCAPI_decodeCoeffsUsingTranscodeMatrix(void *M1obj, float *matrix, int channels, float *result, int bufferSize, int sampleIndex) {
    ((M1DecodeCore *)M1obj)->decodeCoeffsUsingTranscodeMatrix(M1obj, matrix, channels, result, bufferSize, sampleIndex);
}

void Mach1DecodeCAPI_setFilterSpeed(void *M1obj, float filterSpeed) {
    ((M1DecodeCore *)M1obj)->setFilterSpeed(filterSpeed);
}

int Mach1DecodeCAPI_getFormatChannelCount(void *M1obj) {
    return ((M1DecodeCore *)M1obj)->getFormatChannelCount();
}

int Mach1DecodeCAPI_getFormatCoeffCount(void *M1obj) {
    return ((M1DecodeCore *)M1obj)->getFormatCoeffCount();
}

void Mach1DecodeCAPI_setRotation(void *M1obj, Mach1Point3D newRotationFromMinusOnetoOne) {
    ((M1DecodeCore *)M1obj)->setRotation({newRotationFromMinusOnetoOne.x, newRotationFromMinusOnetoOne.y, newRotationFromMinusOnetoOne.z});
}

void Mach1DecodeCAPI_setRotationDegrees(void *M1obj, Mach1Point3D newRotationDegrees) {
    ((M1DecodeCore *)M1obj)->setRotationDegrees({newRotationDegrees.x, newRotationDegrees.y, newRotationDegrees.z});
}

void Mach1DecodeCAPI_setRotationRadians(void *M1obj, Mach1Point3D rotation) {
    ((M1DecodeCore *)M1obj)->setRotationRadians({rotation.x, rotation.y, rotation.z});
}

void Mach1DecodeCAPI_setRotationQuat(void *M1obj, Mach1Point4D newRotationQuat) {
    ((M1DecodeCore *)M1obj)->setRotationQuat({newRotationQuat.x, newRotationQuat.y, newRotationQuat.z, newRotationQuat.w});
}

long Mach1DecodeCAPI_getCurrentTime(void *M1obj) {
    return ((M1DecodeCore *)M1obj)->getCurrentTime();
}

long Mach1DecodeCAPI_getLastCalculationTime(void *M1obj) {
    return ((M1DecodeCore *)M1obj)->getLastCalculationTime();
}

char *Mach1DecodeCAPI_getLog(void *M1obj) {
    return ((M1DecodeCore *)M1obj)->getLog();
}

Mach1Point3D Mach1DecodeCAPI_getCurrentAngle(void *M1obj) {
    Mach1Point3D angle = ((M1DecodeCore *)M1obj)->getCurrentAngle();
    return Mach1Point3D{angle.x, angle.y, angle.z};
}
