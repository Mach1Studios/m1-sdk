//  Mach1 Spatial SDK
//  Copyright © 2017 Mach1. All rights reserved.

#pragma once

#include <string>
#include <vector>

#include "Mach1DecodePositionalCAPI.h"

class Mach1DecodePositional {
    void *M1obj;

  public:
    Mach1DecodePositional();
    ~Mach1DecodePositional();

    void setPlatformType(Mach1PlatformType platformType);
    void setDecodeMode(Mach1DecodeMode mode);

    // settings
    void setMuteWhenOutsideObject(bool muteWhenOutsideObject);
    void setMuteWhenInsideObject(bool muteWhenInsideObject);

    void setUseAttenuation(bool useAttenuation);
    void setAttenuationCurve(float attenuationCurve);

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

#ifndef __EMSCRIPTEN__
    void getCoefficients(float *result);
#endif
    void getCoefficients(std::vector<float> &result);

    float getDist();
    int getFormatChannelCount();
    int getFormatCoeffCount();
    Mach1Point3D getCurrentAngle();
    Mach1Point3D getCurrentAngleInternal();
    Mach1Point3D getPositionalRotation();
    void setFilterSpeed(float filterSpeed);

    Mach1Point3D getClosestPointOnPlane();
};
