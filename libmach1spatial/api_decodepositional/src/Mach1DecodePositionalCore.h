//  Mach1 Spatial SDK
//  Copyright © 2017 Mach1. All rights reserved.

/*
DISCLAIMER:
This header file is not an example of use but an decoder that will require periodic
updates and should not be integrated in sections but remain as an update-able factored file.
*/

#pragma once

#include "Mach1DecodeCore.h"
#include <string>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#ifndef RAD_TO_DEG_F
#    define RAD_TO_DEG_F (180.0f / PI_F)
#endif

#ifndef DEG_TO_RAD_F
#    define DEG_TO_RAD_F (PI_F / 180.0f)
#endif

#ifndef PI_F
#    define PI_F 3.14159265358979323846f
#endif

class Mach1DecodePositionalCore {

  private:
    Mach1DecodeCore mach1Decode;

    bool useFalloff = false;
    float falloffCurve;

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

    static void ConvertPositionToMach1(Mach1PlatformType platformType, float *X, float *Y, float *Z);
    static void ConvertPositionToPlatform(Mach1PlatformType platformType, float *X, float *Y, float *Z);

    static float ClosestPointOnBox(glm::vec3 point, glm::vec3 center, glm::vec3 axis0, glm::vec3 axis1, glm::vec3 axis2, glm::vec3 extents, glm::vec3 &closestPoint);
    static bool Clip(float denom, float numer, float &t0, float &t1);
    static int DoClipping(float t0, float t1, glm::vec3 origin, glm::vec3 direction, glm::vec3 center, glm::vec3 axis0, glm::vec3 axis1, glm::vec3 axis2, glm::vec3 extents, bool solid, glm::vec3 &point0, glm::vec3 &point1);

    static glm::vec3 QuaternionToEuler(glm::quat q);
    static glm::quat EulerToQuaternion(glm::vec3 euler);

    Mach1PlatformType platformType;
    Mach1DecodeMode decodeMode;

    static glm::vec3 GetRightVector();
    static glm::vec3 GetUpVector();
    static glm::vec3 GetForwardVector();

    float gain;
    float dist;
    glm::vec3 eulerAngles;
    glm::vec3 eulerAnglesCube;

    std::vector<float> coeffs;

    milliseconds ms;
    long timeLastCalculation;

    glm::vec3 closestPointOnPlane;

  public:
    Mach1DecodePositionalCore();

    void setDecodeMode(Mach1DecodeMode mode);
    void setPlatformType(Mach1PlatformType type);

    // settings
    void setUseAttenuation(bool useAttenuation);
    void setAttenuationCurve(float attenuationCurve);

    void setMuteWhenOutsideObject(bool muteWhenOutsideObject);
    void setMuteWhenInsideObject(bool muteWhenInsideObject);

    void setUsePlaneCalculation(bool usePlaneCalculation);

    void setUseYawForRotation(bool useYawForRotation);
    void setUsePitchForRotation(bool usePitchForRotation);
    void setUseRollForRotation(bool useRollForRotation);

    // parameters for algo
    void setListenerPosition(Mach1Point3D *pos);
    void setListenerRotation(Mach1Point3D *euler);
    void setListenerRotationQuat(Mach1Point4D *quat);

    void setDecoderAlgoPosition(Mach1Point3D *pos);
    void setDecoderAlgoRotation(Mach1Point3D *euler);
    void setDecoderAlgoRotationQuat(Mach1Point4D *quat);
    void setDecoderAlgoScale(Mach1Point3D *scale);

    void evaluatePositionResults();

    void getCoefficients(float *result);

    float getDist();

    int getFormatChannelCount();
    int getFormatCoeffCount();

    Mach1Point3D getCurrentAngle();
    Mach1Point3D getCurrentAngleInternal();
    Mach1Point3D getPositionalRotation();

    Mach1Point3D getClosestPointOnPlane();

    void setFilterSpeed(float filterSpeed);

    long getCurrentTime();
    long getLastCalculationTime();
};
