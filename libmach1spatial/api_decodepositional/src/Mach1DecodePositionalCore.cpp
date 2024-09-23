//  Mach1 Spatial SDK
//  Copyright © 2017 Mach1. All rights reserved.

/*
DISCLAIMER:
This file is not an example of use but an decoder that will require periodic
updates and should not be integrated in sections but remain as an update-able factored file.
*/

#include "Mach1DecodePositionalCore.h"

glm::vec3 Mach1DecodePositionalCore::QuaternionToEuler(glm::quat q) {
    glm::vec3 euler;

    // if the input quaternion is normalized, this is exactly one. Otherwise, this acts as a correction factor for the quaternion's not-normalizedness
    float unit = (q.x * q.x) + (q.y * q.y) + (q.z * q.z) + (q.w * q.w);

    // this will have a magnitude of 0.5 or greater if and only if this is a singularity case
    float test = q.x * q.w - q.y * q.z;

    if (test > 0.499999f * unit) // singularity at north pole
    {
        euler.y = -PI_F / 2;
        euler.x = -2.0f * atan2(q.y, q.x);
        euler.z = 0;
    } else if (test < -0.499999f * unit) // singularity at south pole
    {
        euler.y = PI_F / 2;
        euler.x = 2.0f * atan2(q.y, q.x);
        euler.z = 0;
    } else // no singularity - this is the majority of cases
    {
        euler.y = -asin(2.0f * (q.w * q.x - q.y * q.z));
        euler.x = atan2(2.0f * q.w * q.y + 2.0f * q.z * q.x, 1 - 2.0f * (q.x * q.x + q.y * q.y));
        euler.z = -atan2(2.0f * q.w * q.z + 2.0f * q.x * q.y, 1 - 2.0f * (q.z * q.z + q.x * q.x));
    }

    // ensure the degree values are between 0 and 2*PI
    euler.x = fmod(euler.x, 2 * PI_F);
    euler.y = fmod(euler.y, 2 * PI_F);
    euler.z = fmod(euler.z, 2 * PI_F);

    return euler;
}

glm::quat Mach1DecodePositionalCore::EulerToQuaternion(glm::vec3 euler) {
    float xOver2 = -euler.y * 0.5f;
    float yOver2 = euler.x * 0.5f;
    float zOver2 = -euler.z * 0.5f;

    float sinXOver2 = sin(xOver2);
    float cosXOver2 = cos(xOver2);
    float sinYOver2 = sin(yOver2);
    float cosYOver2 = cos(yOver2);
    float sinZOver2 = sin(zOver2);
    float cosZOver2 = cos(zOver2);

    glm::quat result;
    result.x = cosYOver2 * sinXOver2 * cosZOver2 + sinYOver2 * cosXOver2 * sinZOver2;
    result.y = sinYOver2 * cosXOver2 * cosZOver2 - cosYOver2 * sinXOver2 * sinZOver2;
    result.z = cosYOver2 * cosXOver2 * sinZOver2 - sinYOver2 * sinXOver2 * cosZOver2;
    result.w = cosYOver2 * cosXOver2 * cosZOver2 + sinYOver2 * sinXOver2 * sinZOver2;

    return result;
}

// Position settings functions
void Mach1DecodePositionalCore::ConvertPositionToMach1(Mach1PlatformType platformType, float *X, float *Y, float *Z) {
    float _x = 0, _y = 0, _z = 0;

    switch (platformType) {
    case Mach1PlatformDefault:
        // forward: z, right: x, up: y
        *X = *X;
        *Y = *Y;
        *Z = *Z;
        break;

    case Mach1PlatformUE:
        // forward: x, right: y, up: z
        _z = *X;
        _x = *Y;
        _y = *Z;
        *X = _x;
        *Y = _y;
        *Z = _z;
        break;

    case Mach1PlatformUnity:
        // forward: z, right: x, up: y
        *X = *X;
        *Y = *Y;
        *Z = *Z;
        break;

    default:
        break;
    }
}

void Mach1DecodePositionalCore::ConvertPositionToPlatform(Mach1PlatformType platformType, float *X, float *Y, float *Z) {
    float _x = 0, _y = 0, _z = 0;

    switch (platformType) {
    case Mach1PlatformDefault:
        // forward: z, right: x, up: y
        *X = *X;
        *Y = *Y;
        *Z = *Z;
        break;

    case Mach1PlatformUE:
        // forward: x, right: y, up: z
        _x = *X;
        _y = *Y;
        _z = *Z;
        *X = _z;
        *Y = _x;
        *Z = _y;
        break;

    case Mach1PlatformUnity:
        // forward: z, right: x, up: y
        // X Y Z -> X Z Y
        break;

    default:
        break;
    }
}

float Mach1DecodePositionalCore::ClosestPointOnBox(glm::vec3 point, glm::vec3 center, glm::vec3 axis0, glm::vec3 axis1, glm::vec3 axis2, glm::vec3 extents, glm::vec3 &closestPoint) {
    glm::vec3 vector = point - center;
    float num = 0.0f;

    float num0 = glm::dot(vector, axis0);
    if (num0 < -extents.x) {
        num += powf(num0 + extents.x, 2);
        num0 = -extents.x;
    } else if (num0 > extents.x) {
        num += powf(num0 - extents.x, 2);
        num0 = extents.x;
    }

    float num1 = glm::dot(vector, axis1);
    if (num1 < -extents.y) {
        num += powf(num1 + extents.y, 2);
        num1 = -extents.y;
    } else if (num1 > extents.y) {
        num += powf(num1 - extents.y, 2);
        num1 = extents.y;
    }

    float num2 = glm::dot(vector, axis2);
    if (num2 < -extents.z) {
        num += powf(num2 + extents.z, 2);
        num2 = -extents.z;
    } else if (num2 > extents.z) {
        num += powf(num2 - extents.z, 2);
        num2 = extents.z;
    }
    closestPoint = center + num0 * axis0 + num1 * axis1 + num2 * axis2;

    return sqrt(num);
}

bool Mach1DecodePositionalCore::Clip(float denom, float numer, float &t0, float &t1) {
    if ((double)denom > 0.0) {
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

int Mach1DecodePositionalCore::DoClipping(float t0, float t1, glm::vec3 origin, glm::vec3 direction, glm::vec3 center, glm::vec3 axis0, glm::vec3 axis1, glm::vec3 axis2, glm::vec3 extents, bool solid, glm::vec3 &point0, glm::vec3 &point1) {
    glm::vec3 vector = origin - center;
    glm::vec3 vector2 = glm::vec3(glm::dot(vector, axis0), glm::dot(vector, axis1), glm::dot(vector, axis2));
    glm::vec3 vector3 = glm::vec3(glm::dot(direction, axis0), glm::dot(direction, axis1), glm::dot(direction, axis2));

    float num = t0;
    float num2 = t1;

    int quantity = 0;

    bool flag = Clip(vector3.x, -vector2.x - extents.x, t0, t1) && Clip(-vector3.x, vector2.x - extents.x, t0, t1) && Clip(vector3.y, -vector2.y - extents.y, t0, t1) && Clip(-vector3.y, vector2.y - extents.y, t0, t1) && Clip(vector3.z, -vector2.z - extents.z, t0, t1) && Clip(-vector3.z, vector2.z - extents.z, t0, t1);
    if (flag && (solid || t0 != num || t1 != num2)) {
        if (t1 > t0) {
            quantity = 2;
            point0 = origin + t0 * direction;
            point1 = origin + t1 * direction;
        } else {

            quantity = 1;
            point0 = origin + t0 * direction;
            point1 = glm::vec3();
        }
    } else {
        quantity = 0;
        point0 = glm::vec3();
        point1 = glm::vec3();
    }

    return quantity;
}

glm::vec3 Mach1DecodePositionalCore::GetRightVector() {
    return glm::vec3(1, 0, 0);
}

glm::vec3 Mach1DecodePositionalCore::GetUpVector() {
    return glm::vec3(0, 1, 0);
}

glm::vec3 Mach1DecodePositionalCore::GetForwardVector() {
    return glm::vec3(0, 0, 1);
}

Mach1DecodePositionalCore::Mach1DecodePositionalCore() {
    falloffCurve = 1;

    ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    timeLastCalculation = 0;

    setDecodeMode(Mach1DecodeMode::M1DecodeSpatial_8);
}

void Mach1DecodePositionalCore::setDecodeMode(Mach1DecodeMode mode) {
    decodeMode = mode;
    mach1Decode.setDecodeMode(decodeMode);
}

void Mach1DecodePositionalCore::setPlatformType(Mach1PlatformType type) {
    platformType = type;
    mach1Decode.setPlatformType(Mach1PlatformType::Mach1PlatformDefault); // because rotation is already converted by positional
}

void Mach1DecodePositionalCore::setAttenuationCurve(float attenuationCurve) {
    this->falloffCurve = attenuationCurve;
}

void Mach1DecodePositionalCore::setMuteWhenOutsideObject(bool _muteWhenOutsideObject) {
    this->muteWhenOutsideObject = _muteWhenOutsideObject;
}

void Mach1DecodePositionalCore::setMuteWhenInsideObject(bool _muteWhenInsideObject) {
    this->muteWhenOutsideObject = _muteWhenInsideObject;
}

void Mach1DecodePositionalCore::setUseAttenuation(bool useAttenuation) {
    this->useFalloff = useAttenuation;
}

void Mach1DecodePositionalCore::setUsePlaneCalculation(bool usePlaneCalculation) {
    this->useClosestPointRotationMuteInside = usePlaneCalculation;
}

void Mach1DecodePositionalCore::setUseYawForRotation(bool _useYawForRotation) {
    this->useYawForRotation = _useYawForRotation;
}

void Mach1DecodePositionalCore::setUsePitchForRotation(bool _usePitchForRotation) {
    this->usePitchForRotation = _usePitchForRotation;
}

void Mach1DecodePositionalCore::setUseRollForRotation(bool _useRollForRotation) {
    this->useRollForRotation = _useRollForRotation;
}

void Mach1DecodePositionalCore::setListenerPosition(Mach1Point3D *pos) {
    ConvertPositionToMach1(platformType, &pos->x, &pos->y, &pos->z);
    cameraPosition = glm::vec3(pos->x, pos->y, pos->z);
}

void Mach1DecodePositionalCore::setListenerRotation(Mach1Point3D *euler) {
    Mach1Point3D angle = {euler->x, euler->y, euler->z};
    Mach1DecodeCore::convertAnglesToMach1(platformType, &angle.x, &angle.y, &angle.z);
    cameraRotation = EulerToQuaternion(glm::vec3(angle.x, angle.y, angle.z) * DEG_TO_RAD_F);
}

void Mach1DecodePositionalCore::setListenerRotationQuat(Mach1Point4D *quat) {
    cameraRotation = glm::quat(quat->w, quat->x, quat->y, quat->z);
}

void Mach1DecodePositionalCore::setDecoderAlgoPosition(Mach1Point3D *pos) {
    ConvertPositionToMach1(platformType, &pos->x, &pos->y, &pos->z);
    soundPosition = glm::vec3(pos->x, pos->y, pos->z);
}

void Mach1DecodePositionalCore::setDecoderAlgoRotation(Mach1Point3D *euler) {
    Mach1Point3D angle = {euler->x, euler->y, euler->z};
    Mach1DecodeCore::convertAnglesToMach1(platformType, &angle.x, &angle.y, &angle.z);
    soundRotation = EulerToQuaternion(glm::vec3(angle.x, angle.y, angle.z) * DEG_TO_RAD_F);
}

void Mach1DecodePositionalCore::setDecoderAlgoRotationQuat(Mach1Point4D *quat) {
    soundRotation = glm::quat(quat->w, quat->x, quat->y, quat->z);
}

void Mach1DecodePositionalCore::setDecoderAlgoScale(Mach1Point3D *scale) {
    soundScale = glm::vec3(scale->x, scale->y, scale->z);
}

void Mach1DecodePositionalCore::evaluatePositionResults() {
    long tStart = getCurrentTime();

    gain = 1.0f;
    dist = 0;

    // Find closest point
    glm::vec3 point = soundPosition;

    glm::vec3 outsideClosestPoint;
    // glm::vec3 insidePoint0, insidePoint1;

    glm::vec3 soundRightVector = soundRotation * GetRightVector();     // glm::vec3(1, 0, 0);// GetRightVector(); // right
    glm::vec3 soundUpVector = soundRotation * GetUpVector();           // glm::vec3(0, 1, 0); // up
    glm::vec3 soundForwardVector = soundRotation * GetForwardVector(); // glm::vec3(0, 0, 1); // forward

    bool isOutside = (ClosestPointOnBox(cameraPosition, soundPosition, soundRightVector, soundUpVector, soundForwardVector, soundScale / 2.0f, outsideClosestPoint) > 0);
    bool hasSoundOutside = isOutside && !muteWhenOutsideObject;
    bool hasSoundInside = !isOutside && !muteWhenInsideObject;

    if (hasSoundOutside && useClosestPointRotationMuteInside) // useClosestPointRotation
    {
        point = outsideClosestPoint;

        dist = glm::distance(cameraPosition, point);

        if (useFalloff) {
            gain = gain * falloffCurve;
        }
    } else if (hasSoundOutside || hasSoundInside) // useCenterPointRotation
    {
        dist = glm::distance(cameraPosition, point);

        if (useFalloff) {
            if (hasSoundOutside) {
                gain = gain * falloffCurve;
            }
        }
    } else {
        gain = 0;
    }

    closestPointOnPlane = point;
    glm::vec3 dir = point - cameraPosition;

    if (glm::length(dir) > 0) {
        // Compute rotation for sound
        // http://www.aclockworkberry.com/world-coordinate-systems-in-3ds-max-unity-and-unreal-engine/
        glm::quat quat;
        quat = glm::quatLookAtLH(glm::normalize(dir), GetUpVector()) * glm::inverse(soundRotation);

        glm::vec3 quatEulerAngles = QuaternionToEuler(glm::normalize(quat));

        bool useXForRotation = useYawForRotation;
        bool useYForRotation = usePitchForRotation;
        bool useZForRotation = useRollForRotation;

        quat = EulerToQuaternion(glm::vec3(useXForRotation ? quatEulerAngles.x : 0, useYForRotation ? quatEulerAngles.y : 0, useZForRotation ? quatEulerAngles.z : 0));
        eulerAnglesCube = QuaternionToEuler(glm::normalize(quat)) * RAD_TO_DEG_F;

        quat = glm::inverse(quat) * cameraRotation; // * glm::inverse(soundRotation);
        eulerAngles = QuaternionToEuler(glm::normalize(quat)) * RAD_TO_DEG_F;

        // SoundAlgorithm
        mach1Decode.setRotationDegrees(Mach1Point3D{eulerAngles.x, eulerAngles.y, eulerAngles.z});
        coeffs = mach1Decode.decodeCoeffs(0, 0);
    } else {
        // Fixed zero distance
        eulerAngles = glm::vec3(0, 0, 0);

        for (int i = 0; i < coeffs.size(); i++) {
            coeffs[i] = 0;
        }
        gain = 0;
    }
    timeLastCalculation = getCurrentTime() - tStart;
}

void Mach1DecodePositionalCore::getCoefficients(float *result) {
    for (int i = 0; i < coeffs.size(); i++) {
        result[i] = gain * coeffs[i];
    }
}

int Mach1DecodePositionalCore::getFormatChannelCount() {
    return mach1Decode.getFormatChannelCount();
}

int Mach1DecodePositionalCore::getFormatCoeffCount() {
    return mach1Decode.getFormatCoeffCount();
}

Mach1Point3D Mach1DecodePositionalCore::getCurrentAngle() {
    glm::vec3 angle = eulerAngles;
    Mach1DecodeCore::convertAnglesToPlatform(platformType, &angle.x, &angle.y, &angle.z);
    return Mach1Point3D{angle.x, angle.y, angle.z};
}

Mach1Point3D Mach1DecodePositionalCore::getCurrentAngleInternal() {
    return Mach1Point3D{eulerAngles.x, eulerAngles.y, eulerAngles.z};
}

Mach1Point3D Mach1DecodePositionalCore::getPositionalRotation() {
    glm::vec3 angle = eulerAnglesCube;
    Mach1DecodeCore::convertAnglesToPlatform(platformType, &angle.x, &angle.y, &angle.z);
    return Mach1Point3D{angle.x, angle.y, angle.z};
}

Mach1Point3D Mach1DecodePositionalCore::getClosestPointOnPlane() {
    glm::vec3 p = closestPointOnPlane;
    ConvertPositionToPlatform(platformType, &p.x, &p.y, &p.z);
    return Mach1Point3D{p.x, p.y, p.z};
}

float Mach1DecodePositionalCore::getDist() {
    return dist;
}

void Mach1DecodePositionalCore::setFilterSpeed(float filterSpeed) {
    mach1Decode.setFilterSpeed(filterSpeed);
}

long Mach1DecodePositionalCore::getCurrentTime() {
    return (long)(duration_cast<milliseconds>(system_clock::now().time_since_epoch()) - ms).count();
}

long Mach1DecodePositionalCore::getLastCalculationTime() {
    return timeLastCalculation;
}
