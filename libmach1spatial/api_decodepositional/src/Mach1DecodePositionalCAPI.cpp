//  Mach1 Spatial SDK
//  Copyright © 2017 Mach1. All rights reserved.

#include "Mach1DecodePositionalCAPI.h"
#include "Mach1DecodePositionalCore.h"

void *Mach1DecodePositionalCAPI_create() {
    return new Mach1DecodePositionalCore();
}

void Mach1DecodePositionalCAPI_delete(void *M1obj) {
    if (M1obj != nullptr) {
        delete (Mach1DecodePositionalCore *)M1obj;
        M1obj = nullptr;
    }
}

void Mach1DecodePositionalCAPI_setPlatformType(void *M1obj, Mach1PlatformType type) {
    if (M1obj != nullptr) {
        ((Mach1DecodePositionalCore *)M1obj)->setPlatformType(type);
    }
}

void Mach1DecodePositionalCAPI_setDecodeMode(void *M1obj, enum Mach1DecodeMode mode) {
    if (M1obj != nullptr) {
        ((Mach1DecodePositionalCore *)M1obj)->setDecodeMode(mode);
    }
}

void Mach1DecodePositionalCAPI_setMuteWhenOutsideObject(void *M1obj, bool muteWhenOutsideObject) {
    ((Mach1DecodePositionalCore *)M1obj)->setMuteWhenOutsideObject(muteWhenOutsideObject);
}

void Mach1DecodePositionalCAPI_setMuteWhenInsideObject(void *M1obj, bool muteWhenInsideObject) {
    ((Mach1DecodePositionalCore *)M1obj)->setMuteWhenInsideObject(muteWhenInsideObject);
}

void Mach1DecodePositionalCAPI_setUseAttenuation(void *M1obj, bool useAttenuation) {
    ((Mach1DecodePositionalCore *)M1obj)->setUseAttenuation(useAttenuation);
}

void Mach1DecodePositionalCAPI_setAttenuationCurve(void *M1obj, float attenuationCurve) {
    ((Mach1DecodePositionalCore *)M1obj)->setAttenuationCurve(attenuationCurve);
}

void Mach1DecodePositionalCAPI_setUsePlaneCalculation(void *M1obj, bool usePlaneCalculation) {
    ((Mach1DecodePositionalCore *)M1obj)->setUsePlaneCalculation(usePlaneCalculation);
}

void Mach1DecodePositionalCAPI_setUseYawForRotation(void *M1obj, bool useYawForRotation) {
    ((Mach1DecodePositionalCore *)M1obj)->setUseYawForRotation(useYawForRotation);
}

void Mach1DecodePositionalCAPI_setUsePitchForRotation(void *M1obj, bool usePitchForRotation) {
    ((Mach1DecodePositionalCore *)M1obj)->setUsePitchForRotation(usePitchForRotation);
}

void Mach1DecodePositionalCAPI_setUseRollForRotation(void *M1obj, bool useRollForRotation) {
    ((Mach1DecodePositionalCore *)M1obj)->setUseRollForRotation(useRollForRotation);
}

void Mach1DecodePositionalCAPI_setListenerPosition(void *M1obj, Mach1Point3D point) {
    Mach1Point3D pnt{point.x, point.y, point.z};
    ((Mach1DecodePositionalCore *)M1obj)->setListenerPosition(&pnt);
}

void Mach1DecodePositionalCAPI_setListenerRotation(void *M1obj, Mach1Point3D point) {
    Mach1Point3D pnt{point.x, point.y, point.z};
    ((Mach1DecodePositionalCore *)M1obj)->setListenerRotation(&pnt);
}

void Mach1DecodePositionalCAPI_setListenerRotationQuat(void *M1obj, Mach1Point4D point) {
    Mach1Point4D pnt{point.x, point.y, point.z, point.w};
    ((Mach1DecodePositionalCore *)M1obj)->setListenerRotationQuat(&pnt);
}

void Mach1DecodePositionalCAPI_setDecoderAlgoPosition(void *M1obj, Mach1Point3D point) {
    Mach1Point3D pnt{point.x, point.y, point.z};
    ((Mach1DecodePositionalCore *)M1obj)->setDecoderAlgoPosition(&pnt);
}

void Mach1DecodePositionalCAPI_setDecoderAlgoRotation(void *M1obj, Mach1Point3D point) {
    Mach1Point3D pnt{point.x, point.y, point.z};
    ((Mach1DecodePositionalCore *)M1obj)->setDecoderAlgoRotation(&pnt);
}

void Mach1DecodePositionalCAPI_setDecoderAlgoRotationQuat(void *M1obj, Mach1Point4D point) {
    Mach1Point4D pnt{point.x, point.y, point.z, point.w};
    ((Mach1DecodePositionalCore *)M1obj)->setDecoderAlgoRotationQuat(&pnt);
}

void Mach1DecodePositionalCAPI_setDecoderAlgoScale(void *M1obj, Mach1Point3D point) {
    Mach1Point3D pnt{point.x, point.y, point.z};
    ((Mach1DecodePositionalCore *)M1obj)->setDecoderAlgoScale(&pnt);
}

void Mach1DecodePositionalCAPI_evaluatePositionResults(void *M1obj) {
    ((Mach1DecodePositionalCore *)M1obj)->evaluatePositionResults();
}

void Mach1DecodePositionalCAPI_getCoefficients(void *M1obj, float *result) {
    ((Mach1DecodePositionalCore *)M1obj)->getCoefficients(result);
}

float Mach1DecodePositionalCAPI_getDist(void *M1obj) {
    return ((Mach1DecodePositionalCore *)M1obj)->getDist();
}

int Mach1DecodePositionalCAPI_getFormatChannelCount(void *M1obj) {
    return ((Mach1DecodePositionalCore *)M1obj)->getFormatChannelCount();
}

int Mach1DecodePositionalCAPI_getFormatCoeffCount(void *M1obj) {
    return ((Mach1DecodePositionalCore *)M1obj)->getFormatCoeffCount();
}

Mach1Point3D Mach1DecodePositionalCAPI_getCurrentAngle(void *M1obj) {
    Mach1Point3D angle = ((Mach1DecodePositionalCore *)M1obj)->getCurrentAngle();
    return Mach1Point3D{angle.x, angle.y, angle.z};
}

Mach1Point3D Mach1DecodePositionalCAPI_getCurrentAngleInternal(void *M1obj) {
    Mach1Point3D angle = ((Mach1DecodePositionalCore *)M1obj)->getCurrentAngleInternal();
    return Mach1Point3D{angle.x, angle.y, angle.z};
}

Mach1Point3D Mach1DecodePositionalCAPI_getPositionalRotation(void *M1obj) {
    Mach1Point3D angle = ((Mach1DecodePositionalCore *)M1obj)->getPositionalRotation();
    return Mach1Point3D{angle.x, angle.y, angle.z};
}

void Mach1DecodePositionalCAPI_setFilterSpeed(void *M1obj, float filterSpeed) {
    ((Mach1DecodePositionalCore *)M1obj)->setFilterSpeed(filterSpeed);
}

Mach1Point3D Mach1DecodePositionalCAPI_getClosestPointOnPlane(void *M1obj) {
    Mach1Point3D p = ((Mach1DecodePositionalCore *)M1obj)->getClosestPointOnPlane();
    return Mach1Point3D{p.x, p.y, p.z};
}

long Mach1DecodePositionalCAPI_getLastCalculationTime(void *M1obj) {
    return ((Mach1DecodePositionalCore *)M1obj)->getLastCalculationTime();
}
