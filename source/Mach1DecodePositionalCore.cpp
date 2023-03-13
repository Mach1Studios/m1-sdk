//  Mach1 Spatial SDK
//  Copyright © 2017-2020 Mach1. All rights reserved.

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
        euler.z = atan2(2.0f * q.w * q.z + 2.0f * q.x * q.y, 1 - 2.0f * (q.z * q.z + q.x * q.x));
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
    float zOver2 = euler.z * 0.5f;

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
    float _X = 0, _Y = 0, _Z = 0;

    switch (platformType) {
    case Mach1PlatformDefault:
        // forward: y, right: x, up: z
        *X = *X;
        *Y = *Y;
        *Z = *Z;
        break;

    case Mach1PlatformUE:
        // forward: x, right: y, up: z
        // Y X Z -> X Y Z
        _Y = *X;
        _X = *Y;
        _Z = *Z;
        *X = _X;
        *Y = _Y;
        *Z = _Z;
        break;

    case Mach1PlatformUnity:
        // forward: z, right: x, up: y
        // X Z Y -> X Y Z
       _X = *X;
       _Z = *Y;
       _Y = *Z;
       *X = _X;
       *Y = _Y;
       *Z = _Z;
        break;

    default:
        break;
    }
}

void Mach1DecodePositionalCore::ConvertPositionToPlatform(Mach1PlatformType platformType, float *X, float *Y, float *Z) {
    float _X = 0, _Y = 0, _Z = 0;

    switch (platformType) {
    case Mach1PlatformDefault:
        *X = *X;
        *Y = *Y;
        *Z = *Z;
        break;

    case Mach1PlatformUE:
        // forward: x, right: y, up: z
        // X Y Z -> Y X Z
        _X = *X;
        _Y = *Y;
        _Z = *Z;
        *X = _Y;
        *Y = _X;
        *Z = _Z;
        break;

    case Mach1PlatformUnity:
        // forward: z, right: x, up: y
        // X Y Z -> X Z Y
       _X = *X;
       _Y = *Y;
       _Z = *Z;
       *X = _X;
       *Y = _Z;
       *Z = _Y;
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
    falloffCurveBlendMode = 1;

    ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    timeLastCalculation = 0;

    setDecodeAlgoType(Mach1DecodeAlgoType::Mach1DecodeAlgoSpatial_8);
}

void Mach1DecodePositionalCore::setDecodeAlgoType(Mach1DecodeAlgoType type) {
    algorithmType = type;
    mach1Decode.setDecodeAlgoType(type);
}

void Mach1DecodePositionalCore::setPlatformType(Mach1PlatformType type) {
    platformType = type;
    mach1Decode.setPlatformType(Mach1PlatformType::Mach1PlatformDefault); // because rotation is already converted by positional
}

void Mach1DecodePositionalCore::setUseBlendMode(bool useBlendMode) {
    this->useBlendMode = useBlendMode;
}

void Mach1DecodePositionalCore::setAttenuationCurve(float attenuationCurve) {
    this->falloffCurve = attenuationCurve;
}

void Mach1DecodePositionalCore::setAttenuationCurveBlendMode(float attenuationCurveBlendMode) {
    this->falloffCurveBlendMode = attenuationCurveBlendMode;
}

void Mach1DecodePositionalCore::setIgnoreTopBottom(bool ignoreTopBottom) {
    this->ignoreTopBottom = ignoreTopBottom;
}

void Mach1DecodePositionalCore::setMuteWhenOutsideObject(bool muteWhenOutsideObject) {
    this->muteWhenOutsideObject = muteWhenOutsideObject;
}

void Mach1DecodePositionalCore::setMuteWhenInsideObject(bool muteWhenInsideObject) {
    this->muteWhenOutsideObject = muteWhenOutsideObject;
}

void Mach1DecodePositionalCore::setUseAttenuation(bool useAttenuation) {
    this->useFalloff = useAttenuation;
}

void Mach1DecodePositionalCore::setUsePlaneCalculation(bool usePlaneCalculation) {
    this->useClosestPointRotationMuteInside = usePlaneCalculation;
}

void Mach1DecodePositionalCore::setUseYawForRotation(bool useYawForRotation) {
    this->useYawForRotation = useYawForRotation;
}

void Mach1DecodePositionalCore::setUsePitchForRotation(bool usePitchForRotation) {
    this->usePitchForRotation = usePitchForRotation;
}

void Mach1DecodePositionalCore::setUseRollForRotation(bool useRollForRotation) {
    this->useRollForRotation = useRollForRotation;
}

void Mach1DecodePositionalCore::setListenerPosition(Mach1Point3DCore *pos) {
    ConvertPositionToMach1(platformType, &pos->x, &pos->y, &pos->z);
    cameraPosition = glm::vec3(pos->x, pos->y, pos->z);
}

void Mach1DecodePositionalCore::setListenerRotation(Mach1Point3DCore *euler) {
    Mach1Point3DCore angle(euler->x, euler->y, euler->z);
    Mach1DecodeCore::convertAnglesToMach1(platformType, &angle.x, &angle.y, &angle.z);
    cameraRotation = EulerToQuaternion(glm::vec3(angle.x, angle.y, angle.z) * DEG_TO_RAD_F);
}

void Mach1DecodePositionalCore::setListenerRotationQuat(Mach1Point4DCore *quat) {
    cameraRotation = glm::quat(quat->w, quat->x, quat->y, quat->z);
}

void Mach1DecodePositionalCore::setDecoderAlgoPosition(Mach1Point3DCore *pos) {
    ConvertPositionToMach1(platformType, &pos->x, &pos->y, &pos->z);
    soundPosition = glm::vec3(pos->x, pos->y, pos->z);
}

void Mach1DecodePositionalCore::setDecoderAlgoRotation(Mach1Point3DCore *euler) {
    Mach1Point3DCore angle(euler->x, euler->y, euler->z);
    Mach1DecodeCore::convertAnglesToMach1(platformType, &angle.x, &angle.y, &angle.z);
    soundRotation = EulerToQuaternion(glm::vec3(angle.x, angle.y, angle.z) * DEG_TO_RAD_F);
}

void Mach1DecodePositionalCore::setDecoderAlgoRotationQuat(Mach1Point4DCore *quat) {
    soundRotation = glm::quat(quat->w, quat->x, quat->y, quat->z);
}

void Mach1DecodePositionalCore::setDecoderAlgoScale(Mach1Point3DCore *scale) {
    soundScale = glm::vec3(scale->x, scale->y, scale->z);
}

void Mach1DecodePositionalCore::evaluatePositionResults() {
    long tStart = getCurrentTime();

    volumeWalls = 1.0f;
    volumeRoom = 0.0f;

    dist = 0;

    // Find closest point
    glm::vec3 point = soundPosition;

    glm::vec3 outsideClosestPoint;
    // glm::vec3 insidePoint0, insidePoint1;

    if (ignoreTopBottom && useBlendMode) {
        cameraPosition.y = soundPosition.y;
    }

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
            volumeWalls = volumeWalls * falloffCurve;
        }

    } else if (hasSoundInside && useBlendMode) // && DoClipping(0, float.MaxValue, cameraPosition, (cameraPosition - soundPosition).normalized, soundPosition, gameObject.transform.right, gameObject.transform.up, gameObject.transform.forward, gameObject.transform.localScale / 2, true, out insidePoint0, out insidePoint1) == 2)
    {
        glm::mat4 identity(1.0f); // construct identity matrix
        glm::mat4 translate = glm::translate(identity, soundPosition);
        glm::mat4 rotate = glm::toMat4(soundRotation);
        glm::mat4 scale = glm::scale(identity, soundScale);

        glm::mat4 mat = glm::inverse(translate * rotate * scale);

        glm::vec3 p0 = 2.0f * (mat * glm::vec4(cameraPosition, 1.0f)); // InverseTransformPoint

        dist = 1 - std::max(abs(p0.x), std::max(abs(p0.y), abs(p0.z)));

        if (useFalloff) {
            volumeWalls = volumeWalls * falloffCurveBlendMode;
        }

        volumeRoom = 1 - volumeWalls;
    } else if (hasSoundOutside || hasSoundInside) // useCenterPointRotation
    {
        dist = glm::distance(cameraPosition, point);

        if (useFalloff) {
            if (hasSoundOutside) {
                volumeWalls = volumeWalls * falloffCurve;
            }
            if (useBlendMode) {
                volumeWalls = volumeWalls * falloffCurveBlendMode;
            }
        }
    } else {
        volumeWalls = 0;
        volumeRoom = 0;
    }

    closestPointOnPlane = point;

    // mini test
    /*
    {
        glm::quat quat = EulerToQuaternion(glm::vec3(12, -10, 25) * DEG_TO_RAD_F);
        glm::vec3 quatEulerAngles = QuaternionToEuler(quat) * RAD_TO_DEG_F;
        glm::quat quat2 = EulerToQuaternion(quatEulerAngles * DEG_TO_RAD_F);

        glm::quat myquaternion = glm::quat(glm::vec3(12, -10, 25) * DEG_TO_RAD_F);
        glm::vec3 quatEulerAngles2 = glm::eulerAngles(myquaternion) * RAD_TO_DEG_F;
        glm::quat myquaternion2 = glm::quat(quatEulerAngles2 * DEG_TO_RAD_F);

        quatEulerAngles.x = quatEulerAngles.x;
    }
    //*/

    glm::vec3 dir = point - cameraPosition;

    if (glm::length(dir) > 0) {
        // Compute rotation for sound
        // http://www.aclockworkberry.com/world-coordinate-systems-in-3ds-max-unity-and-unreal-engine/
        glm::quat quat;
        quat = glm::quatLookAtLH(glm::normalize(dir), GetUpVector() * glm::inverse(soundRotation));

        glm::vec3 quatEulerAngles = QuaternionToEuler(glm::normalize(quat));

        bool useXForRotation = useYawForRotation;
        bool useYForRotation = usePitchForRotation;
        bool useZForRotation = useRollForRotation;

        quat = EulerToQuaternion(glm::vec3(useXForRotation ? quatEulerAngles.x : 0, useYForRotation ? quatEulerAngles.y : 0, useZForRotation ? quatEulerAngles.z : 0));
        eulerAnglesCube = QuaternionToEuler(glm::normalize(quat)) * RAD_TO_DEG_F;

        quat = glm::inverse(cameraRotation) * soundRotation;
        quat = glm::inverse(quat); // the last inversion, because it is not a rotation of the cube, but a pointer in the opposite direction

        eulerAngles = QuaternionToEuler(glm::normalize(quat)) * RAD_TO_DEG_F;

        // SoundAlgorithm
        mach1Decode.setRotationDegrees(Mach1Point3DCore{eulerAngles.x, eulerAngles.y, eulerAngles.z});
        mach1Decode.beginBuffer();
        volumes = mach1Decode.decodeCoeffs(0, 0);
        mach1Decode.endBuffer();
    } else {
        // Fixed zero distance
        eulerAngles = glm::vec3(0, 0, 0);

        for (int i = 0; i < volumes.size(); i++) {
            volumes[i] = 0;
        }

        volumeWalls = 0;
        volumeRoom = 0;
    }

    timeLastCalculation = getCurrentTime() - tStart;
}

void Mach1DecodePositionalCore::getCoefficients(float *result) {
    for (int i = 0; i < volumes.size(); i++) {
        result[i] = volumeWalls * volumes[i];
    }
}

void Mach1DecodePositionalCore::getCoefficientsInterior(float *result) {
    for (int i = 0; i < volumes.size(); i++) {
        result[i] = volumeRoom * volumes[i];
    }
}

Mach1Point3DCore Mach1DecodePositionalCore::getCurrentAngle() {
    glm::vec3 angle = eulerAngles;
    Mach1DecodeCore::convertAnglesToPlatform(platformType, &angle.x, &angle.y, &angle.z);
    return Mach1Point3DCore{angle.x, angle.y, angle.z};
}

Mach1Point3DCore Mach1DecodePositionalCore::getCurrentAngleInternal() {
    return Mach1Point3DCore{eulerAngles.x, eulerAngles.y, eulerAngles.z};
}

Mach1Point3DCore Mach1DecodePositionalCore::getPositionalRotation() {
    glm::vec3 angle = eulerAnglesCube;
    Mach1DecodeCore::convertAnglesToPlatform(platformType, &angle.x, &angle.y, &angle.z);
    return Mach1Point3DCore{angle.x, angle.y, angle.z};
}

Mach1Point3DCore Mach1DecodePositionalCore::getClosestPointOnPlane() {
    glm::vec3 p = closestPointOnPlane;
    ConvertPositionToPlatform(platformType, &p.x, &p.y, &p.z);
    return Mach1Point3DCore{p.x, p.y, p.z};
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
