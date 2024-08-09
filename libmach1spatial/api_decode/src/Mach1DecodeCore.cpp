//  Mach1 Spatial SDK
//  Copyright © 2017-2020 Mach1. All rights reserved.

/*
DISCLAIMER:
This file is not an example of use but an decoder that will require periodic
updates and should not be integrated in sections but remain as an update-able factored file.
*/

/*
Internal Orientation Implementation:
    -  Yaw[0]+ = rotate right 0-1 [Range: 0->360 | -180->180]
    -  Yaw[0]- = rotate left 0-1 [Range: 0->360 | -180->180]
    -  Pitch[1]+ = rotate up 0-1 [Range: -90->90]
    -  Pitch[1]- = rotate down 0-1 [Range: -90->90]
    -  Roll[2]+ = rotate up 0-1 [Range: -90->90]
    -  Roll[2]- = rotate down 0-1 [Range: -90->90]

Mach1DecodeCore normalizes all input ranges to an unsigned "0 to 1" range for Yaw, Pitch and Roll.
 */

#include "Mach1DecodeCore.h"
#include <string>

#ifndef __ANDROID__
// TODO: Remove this and figure out how to get ANDROID builds to accept newer stdlib 
#define copysign std::copysign
#endif

#ifndef DEG_TO_RAD
#    define DEG_TO_RAD (PI / 180.0)
#endif

#ifndef __FLT_EPSILON__
#    define __FLT_EPSILON__ 1.19209290e-07F
#endif

float Mach1DecodeCore::lerp(float x1, float x2, float t) {
    return x1 + (x2 - x1) * t;
}

float Mach1DecodeCore::mDegToRad(float degrees) {
    return (float)(degrees * DEG_TO_RAD);
}

//
// Map utility
//

float Mach1DecodeCore::mmap(float value, float inputMin, float inputMax, float outputMin, float outputMax, bool clamp) {

    if (fabs(inputMin - inputMax) < __FLT_EPSILON__) {
        return outputMin;
    } else {
        float outVal = ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin);

        if (clamp) {
            if (outputMax < outputMin) {
                if (outVal < outputMax)
                    outVal = outputMax;
                else if (outVal > outputMin)
                    outVal = outputMin;
            } else {
                if (outVal > outputMax)
                    outVal = outputMax;
                else if (outVal < outputMin)
                    outVal = outputMin;
            }
        }
        return outVal;
    }
}

float Mach1DecodeCore::clamp(float a, float min, float max) {
    return (a < min) ? min : ((a > max) ? max : a);
}

float Mach1DecodeCore::alignAngle(float a, float min, float max) {
    if (a > 5000 || a < -5000)
        return 0;

    while (a < min)
        a += 360;
    while (a > max)
        a -= 360;

    return a;
}

float Mach1DecodeCore::radialDistance(float angle1, float angle2) {
    if ((std::abs(angle2 - angle1)) > (std::abs(std::abs(angle2 - angle1) - 360))) {
        return std::abs(std::abs(angle2 - angle1) - 360);
    } else
        return std::abs(angle2 - angle1);
}

float Mach1DecodeCore::targetDirectionMultiplier(float angleCurrent, float angleTarget) {
    if (((std::abs(angleCurrent - angleTarget)) >
         (std::abs(angleCurrent - angleTarget + 360))) ||
        ((std::abs(angleCurrent - angleTarget)) >
         (std::abs(angleCurrent - angleTarget - 360)))) {
        if (angleCurrent < angleTarget) {
            return -1.;
        }
        if (angleCurrent > angleTarget) {
            return 1.;
        }
    } else {
        if (angleCurrent < angleTarget) {
            return 1.;
        }
        if (angleCurrent > angleTarget) {
            return -1.;
        }
    }
    return 0.;
}

// Note: Envelope follower feature is defined here, in updateAngles()
void Mach1DecodeCore::updateAngles() {
    targetYaw = fmod(targetYaw, 360);
    targetPitch = fmod(targetPitch, 360);
    targetRoll = fmod(targetRoll, 360);

    currentYaw = fmod(currentYaw, 360);
    currentPitch = fmod(currentPitch, 360);
    currentRoll = fmod(currentRoll, 360);

    if (filterSpeed >= 1.0f) {
        currentYaw = targetYaw;
        currentPitch = targetPitch;
        currentRoll = targetRoll;
    } else {
        float speedAngle = timeLastUpdate ? filterSpeed * 1.0f * (getCurrentTime() - timeLastUpdate) : 0;

        timeLastUpdate = getCurrentTime();

        float distanceYaw = radialDistance(targetYaw, currentYaw);
        float distancePitch = radialDistance(targetPitch, currentPitch);
        float distanceRoll = radialDistance(targetRoll, currentRoll);

        float distance = distanceYaw + distancePitch + distanceRoll;

        if (distanceYaw > speedAngle && distance < 360) {
            currentYaw += speedAngle * targetDirectionMultiplier(currentYaw, targetYaw);
        } else {
            currentYaw = targetYaw;
        }

        if (distancePitch > speedAngle && distance < 360) {
            currentPitch += speedAngle * targetDirectionMultiplier(currentPitch, targetPitch);
        } else {
            currentPitch = targetPitch;
        }

        if (distanceRoll > speedAngle && distance < 360) {
            currentRoll += speedAngle * targetDirectionMultiplier(currentRoll, targetRoll);
        } else {
            currentRoll = targetRoll;
        }
    }
}

float Mach1DecodeCore::_dot(const Mach1Point3D &p1, const Mach1Point3D &p2) const {
    return p1.x * p2.x + p1.y * p2.y + p1.z * p2.z;
}

bool Mach1DecodeCore::linePlaneIntersection(Mach1Point3D &contact, Mach1Point3D ray, Mach1Point3D rayOrigin, Mach1Point3D normal, Mach1Point3D coord) {
    float epsilon = 1e-6f;

    Mach1Point3D u = rayOrigin - ray;
    float dot = _dot(normal, u);

    if (fabs(dot) > epsilon) {
        Mach1Point3D w = ray - coord;
        float fac = -_dot(normal, w) / dot;
        if (fac >= 0.0 && fac <= 1.0) {
            u = u * fac;
            contact = ray + u;
            return true;
        }
    }
    return false;
}

void Mach1DecodeCore::spatialMultichannelAlgo(Mach1Point3D *channelPoints, int numChannelPoints, float Yaw, float Pitch, float Roll, float *result) {
    Mach1Point3D simulationAngles = { .x = Yaw, .y = Pitch, .z = Roll };

    Mach1Point3D faceVector1 = { sin(mDegToRad(simulationAngles[0])), cos(mDegToRad(simulationAngles[0])), 0 };
    faceVector1.normalize();
    Mach1Point3D faceVector11 = { sin(mDegToRad(simulationAngles[0] - 90)), cos(mDegToRad(simulationAngles[0] - 90)), 0 };
    faceVector11.normalize();

    Mach1Point3D faceVector2 = faceVector1.getRotated(-simulationAngles[1], faceVector11);
    Mach1Point3D faceVector21 = faceVector1.getRotated(-simulationAngles[1] - 90, faceVector11);

    Mach1Point3D faceVectorLeft = faceVector21.getRotated(simulationAngles[2] - 90, faceVector2);
    Mach1Point3D faceVectorRight = faceVector21.getRotated(simulationAngles[2] + 90, faceVector2);

    Mach1Point3D contactL = faceVectorLeft * 100 + faceVector2 * 100;
    Mach1Point3D contactR = faceVectorRight * 100 + faceVector2 * 100;

    float d = sqrtf(100 * 100 + 200 * 200);

    std::vector<float> vL(numChannelPoints);
    std::vector<float> vR(numChannelPoints);

    std::vector<float> vL_clamped(numChannelPoints);
    std::vector<float> vR_clamped(numChannelPoints);

    for (int i = 0; i < numChannelPoints; i++) {
        Mach1Point3D qL = (contactL - channelPoints[i]);
        Mach1Point3D qR = (contactR - channelPoints[i]);

        vL[i] = qL.length();
        vR[i] = qR.length();

        vL_clamped[i] = Mach1DecodeCore::clamp(Mach1DecodeCore::mmap(vL[i], 0, d, 1.f, 0.f, false), 0, 1);
        vR_clamped[i] = Mach1DecodeCore::clamp(Mach1DecodeCore::mmap(vR[i], 0, d, 1.f, 0.f, false), 0, 1);

        result[i * 2 + 0] = vL_clamped[i];
        result[i * 2 + 1] = vR_clamped[i];
    }

    // Gain normalizer v2.0
    float sumL = 0, sumR = 0;
    for (int i = 0; i < numChannelPoints; i++) {
        sumL += result[i * 2];
        sumR += result[i * 2 + 1];
    }
    for (int i = 0; i < numChannelPoints; i++) {
        result[i * 2 + 0] /= sumL;
        result[i * 2 + 1] /= sumR;
    }
    // if(sumL > 1.0 || sumR > 1.0) printf("%f - %f\r\n", sumL, sumR);
}

/*
 Defined multichannel spatial layouts

 Mach1 XYZ Coordinate Expectation:
 X (left -> right | where -X is left)
 Y (front -> back | where -Y is back)
 Z (top -> bottom | where -Z is bottom)
 
 */

void Mach1DecodeCore::spatialAlgoSample_8(float Yaw, float Pitch, float Roll, float *result) {
    const int numChannelPoints = 8;

    Mach1Point3D channelPoints[numChannelPoints] =
        {
            {-100, 100, 100},
            {100, 100, 100},
            {-100, -100, 100},
            {100, -100, 100},

            {-100, 100, -100},
            {100, 100, -100},
            {-100, -100, -100},
            {100, -100, -100},
        };

    spatialMultichannelAlgo(channelPoints, numChannelPoints, Yaw, Pitch, Roll, result);
}

std::vector<float> Mach1DecodeCore::spatialAlgoSample_8(float Yaw, float Pitch, float Roll) {
    const int numChannelPoints = 8;

    std::vector<float> result;
    result.resize(numChannelPoints * 2);

    Mach1Point3D channelPoints[numChannelPoints] =
        {
            {-100, 100, 100},
            {100, 100, 100},
            {-100, -100, 100},
            {100, -100, 100},

            {-100, 100, -100},
            {100, 100, -100},
            {-100, -100, -100},
            {100, -100, -100},
        };

    spatialMultichannelAlgo(channelPoints, numChannelPoints, Yaw, Pitch, Roll, result.data());

    return result;
}

void Mach1DecodeCore::spatialAlgoSample_12(float Yaw, float Pitch, float Roll, float *result) {
    const int numChannelPoints = 8 + 4;

    float diag = sqrtf(2 * 100 * 100);

    Mach1Point3D channelPoints[numChannelPoints] =
        {
            {-100, 100, 100},
            {100, 100, 100},
            {-100, -100, 100},
            {100, -100, 100},

            {-100, 100, -100},
            {100, 100, -100},
            {-100, -100, -100},
            {100, -100, -100},

            {0, diag, 0},
            {diag, 0, 0},
            {0, -diag, 0},
            {-diag, 0, 0},
        };

    spatialMultichannelAlgo(channelPoints, numChannelPoints, Yaw, Pitch, Roll, result);
}

std::vector<float> Mach1DecodeCore::spatialAlgoSample_12(float Yaw, float Pitch, float Roll) {
    const int numChannelPoints = 8 + 4;

    float diag = sqrtf(2 * 100 * 100);

    std::vector<float> result;
    result.resize(numChannelPoints * 2);

    Mach1Point3D channelPoints[numChannelPoints] =
        {
            {-100, 100, 100},
            {100, 100, 100},
            {-100, -100, 100},
            {100, -100, 100},

            {-100, 100, -100},
            {100, 100, -100},
            {-100, -100, -100},
            {100, -100, -100},

            {0, diag, 0},
            {diag, 0, 0},
            {0, -diag, 0},
            {-diag, 0, 0},
        };

    spatialMultichannelAlgo(channelPoints, numChannelPoints, Yaw, Pitch, Roll, result.data());

    return result;
}

void Mach1DecodeCore::spatialAlgoSample_14(float Yaw, float Pitch, float Roll, float *result) {
    const int numChannelPoints = 8 + 4 + 2;

    float diag = sqrtf(2 * 100 * 100);

    Mach1Point3D channelPoints[numChannelPoints] =
        {
            {-100, 100, 100},
            {100, 100, 100},
            {-100, -100, 100},
            {100, -100, 100},

            {-100, 100, -100},
            {100, 100, -100},
            {-100, -100, -100},
            {100, -100, -100},

            {0, diag, 0},
            {diag, 0, 0},
            {0, -diag, 0},
            {-diag, 0, 0},

            {0, 0, diag},
            {0, 0, -diag},
        };

    spatialMultichannelAlgo(channelPoints, numChannelPoints, Yaw, Pitch, Roll, result);
}

std::vector<float> Mach1DecodeCore::spatialAlgoSample_14(float Yaw, float Pitch, float Roll) {
    const int numChannelPoints = 8 + 4 + 2;

    float diag = sqrtf(2 * 100 * 100);

    std::vector<float> result;
    result.resize(numChannelPoints * 2);

    Mach1Point3D channelPoints[numChannelPoints] =
        {
            {-100, 100, 100},
            {100, 100, 100},
            {-100, -100, 100},
            {100, -100, 100},

            {-100, 100, -100},
            {100, 100, -100},
            {-100, -100, -100},
            {100, -100, -100},

            {0, diag, 0},
            {diag, 0, 0},
            {0, -diag, 0},
            {-diag, 0, 0},

            {0, 0, diag},
            {0, 0, -diag},
        };

    spatialMultichannelAlgo(channelPoints, numChannelPoints, Yaw, Pitch, Roll, result.data());

    return result;
}

// Angular settings functions
void Mach1DecodeCore::convertAnglesToMach1(Mach1PlatformType platformType, float *Y, float *P, float *R) {
    float _Y = 0, _P = 0, _R = 0;

    switch (platformType) {
    case Mach1PlatformDefault:
        *Y = *Y;
        *P = *P;
        *R = *R;
        break;

    case Mach1PlatformiOS:
        *Y = -*Y;
        *P = *P;
        *R = *R;
        break;

    case Mach1PlatformiOSTableTop_ZVertical:
        *Y = -*Y;
        *P = *P;
        *R = *R;
        break;

    case Mach1PlatformiOSPortraitHandheld_YVertical:
        // TODO: add this!
        break;

    case Mach1PlatformiOSPortrait_YawOnly:
        *Y = -*Y;
        *P = *P;
        *R = *R;
        if (*R < 0 && *Y < 0) {
            *Y = 360 - (-1 * (*Y + *R));
        } else {
            *Y = *Y + *R;
        }
        break;

    case Mach1PlatformUnity:
        // P Y R -> Y P R
        _P = *Y;
        _Y = *P;
        _R = *R;
        *Y = _Y;
        *P = -_P;
        *R = -_R;
        break;

    case Mach1PlatformUE:
        // R P Y -> Y P R
        _R = *Y;
        _P = *P;
        _Y = *R;
        *Y = _Y;
        *P = _P;
        *R = _R;
        break;

    case Mach1PlatformOfEasyCam:
        *Y = *Y;
        *P = -*P;
        *R = -*R;
        break;

    case Mach1PlatformAndroid:
        // P Y R -> Y -P R
        _P = *Y;
        _Y = *P;
        _R = *R;
        *Y = _Y;
        *P = -_P;
        *R = _R;
        break;

    default:
        break;
    }
}

void Mach1DecodeCore::convertAnglesToPlatform(Mach1PlatformType platformType, float *Y, float *P, float *R) {
    float _Y = 0, _P = 0, _R = 0;

    switch (platformType) {
    case Mach1PlatformDefault:
        *Y = *Y;
        *P = *P;
        *R = *R;
        break;

    case Mach1PlatformiOS:
        *Y = -*Y;
        *P = *P;
        *R = *R;
        break;

    case Mach1PlatformiOSTableTop_ZVertical:
        *Y = -*Y;
        *P = *P;
        *R = *R;
        break;

    case Mach1PlatformiOSPortraitHandheld_YVertical:
        // TODO: add this!
        break;

    case Mach1PlatformiOSPortrait_YawOnly:
        *Y = -*Y;
        *P = *P;
        *R = *R;
        if (*R < 0 && *Y < 0) {
            *Y = 360 - (-1 * (*Y + *R));
        } else {
            *Y = *Y + *R;
        }
        break;

    case Mach1PlatformUnity:
        // Y P R -> P Y R
        _Y = *Y;
        _P = -*P;
        _R = -*R;
        *Y = _P;
        *P = _Y;
        *R = _R;
        break;

    case Mach1PlatformUE:
        // Y P -R -> R P Y
        _Y = *Y;
        _P = *P;
        _R = *R;
        *Y = _R;
        *P = _P;
        *R = _Y;
        break;

    case Mach1PlatformOfEasyCam:
        *Y = *Y;
        *P = -*P;
        *R = -*R;
        break;

    case Mach1PlatformAndroid:
        *Y = -*Y;
        *P = -*P;
        *R = *R;
        std::swap(*P, *Y);
        break;

    default:
        break;
    }
}

Mach1Point3D Mach1DecodeCore::getCurrentAngle() {
    Mach1Point3D angle = {currentYaw, currentPitch, currentRoll};
    return angle;
}

void Mach1DecodeCore::addToLog(std::string str, int maxCount) {
    if (strLog.size() > maxCount)
        strLog.erase(strLog.begin() + maxCount, strLog.begin() + strLog.size() - 1);

    strLog.push_back(str);
}

char *Mach1DecodeCore::getLog() {
    std::vector<std::string> _strLog = strLog;
    strLog.clear();

    static char log[100000] = "";

    log[0] = '\0';
    /*
    for (int i = 0; i<_strLog.size(); i++) strcat(log, _strLog[i].c_str());
     */
    return log;
}

Mach1DecodeCore::Mach1DecodeCore() {
    currentYaw = 0;
    currentPitch = 0;
    currentRoll = 0;

    targetYaw = 0;
    targetPitch = 0;
    targetRoll = 0;

    previousYaw = 0;
    previousPitch = 0;
    previousRoll = 0;

    filterSpeed = 0.9f;
    timeLastUpdate = 0;
    timeLastCalculation = 0;

    platformType = Mach1PlatformDefault;
    algorithmType = Mach1DecodeAlgoSpatial_8;

    ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

    smoothAngles = true; // false

    strLog.resize(0);
}

long Mach1DecodeCore::getCurrentTime() {
    return (long)(duration_cast<milliseconds>(system_clock::now().time_since_epoch()) - ms).count();
}

long Mach1DecodeCore::getLastCalculationTime() {
    return timeLastCalculation;
}

void Mach1DecodeCore::setPlatformType(Mach1PlatformType type) {
    platformType = type;
}

Mach1PlatformType Mach1DecodeCore::getPlatformType() {
    return platformType;
}

int Mach1DecodeCore::getFormatChannelCount() {
    switch (algorithmType) {
    case Mach1DecodeAlgoSpatial_4:
        return 4;
    case Mach1DecodeAlgoSpatial_8:
        return 8;
    case Mach1DecodeAlgoSpatial_12:
        return 12;
    case Mach1DecodeAlgoSpatial_14:
        return 14;
    }
    return 0;
}

int Mach1DecodeCore::getFormatCoeffCount() {
    switch (algorithmType) {
    case Mach1DecodeAlgoSpatial_4:
        return (4 * 2);
    case Mach1DecodeAlgoSpatial_8:
        return (8 * 2);
    case Mach1DecodeAlgoSpatial_12:
        return (12 * 2);
    case Mach1DecodeAlgoSpatial_14:
        return (14 * 2);
    }
    return 0;
}

void Mach1DecodeCore::setRotation(Mach1Point3D newRotationFromMinusOnetoOne) {
    rotation = newRotationFromMinusOnetoOne * 360.0;
}

void Mach1DecodeCore::setRotationDegrees(Mach1Point3D newRotationDegrees) {
    rotation = newRotationDegrees;
}

void Mach1DecodeCore::setRotationRadians(Mach1Point3D newRotationRadians) {
    rotation = newRotationRadians * (180.0 / PI);
}

void Mach1DecodeCore::setRotationQuat(Mach1Point4D newRotationQuat) {
    Mach1Point3D angles;

    // roll (x-axis rotation)
    float sinr_cosp = 2 * (newRotationQuat.w * newRotationQuat.x + newRotationQuat.y * newRotationQuat.z);
    float cosr_cosp = 1 - 2 * (newRotationQuat.x * newRotationQuat.x + newRotationQuat.y * newRotationQuat.y);
    angles.x = atan2f(sinr_cosp, cosr_cosp);

    // pitch (y-axis rotation)
    float sinp = 2 * (newRotationQuat.w * newRotationQuat.y - newRotationQuat.z * newRotationQuat.x);
    if (std::abs(sinp) >= 1) {
        angles.y = copysignf(PI / 2, sinp); // use 90 degrees if out of range
    } else {
        angles.y = asinf(sinp);
    }

    // yaw (z-axis rotation)
    float siny_cosp = 2 * (newRotationQuat.w * newRotationQuat.z + newRotationQuat.x * newRotationQuat.y);
    float cosy_cosp = 1 - 2 * (newRotationQuat.y * newRotationQuat.y + newRotationQuat.z * newRotationQuat.z);
    angles.z = atan2f(siny_cosp, cosy_cosp);

    setRotationRadians(angles);
}

void Mach1DecodeCore::setFilterSpeed(float newFilterSpeed) {
    filterSpeed = newFilterSpeed;
}

//--------------------------------------------------

void Mach1DecodeCore::setDecodeAlgoType(Mach1DecodeAlgoType newAlgorithmType) {
    algorithmType = newAlgorithmType;
}

Mach1DecodeAlgoType Mach1DecodeCore::getDecodeAlgoType() {
    return algorithmType;
}

std::vector<float> Mach1DecodeCore::decode(float Yaw, float Pitch, float Roll, int bufferSize, int sampleIndex) {
    setRotationDegrees({Yaw, Pitch, Roll});
    return decodeCoeffs(bufferSize, sampleIndex);
}

std::vector<float> Mach1DecodeCore::decodeCoeffs(int bufferSize, int sampleIndex) {
    long tStart = getCurrentTime();
    std::vector<float> res;

    float Yaw = fmod(rotation.x, 360.0); // protect a 360 cycle
    float Pitch = fmod(rotation.y, 360.0);
    float Roll = fmod(rotation.z, 360.0);

    switch (algorithmType) {
        case Mach1DecodeAlgoSpatial_4:
            res = spatialAlgo_4(Yaw, Pitch, Roll, bufferSize, sampleIndex);
            break;

        case Mach1DecodeAlgoSpatial_8:
            res = spatialAlgo_8(Yaw, Pitch, Roll, bufferSize, sampleIndex);
            break;

        case Mach1DecodeAlgoSpatial_12:
            res = spatialAlgo_12(Yaw, Pitch, Roll, bufferSize, sampleIndex);
            break;

        case Mach1DecodeAlgoSpatial_14:
            res = spatialAlgo_14(Yaw, Pitch, Roll, bufferSize, sampleIndex);
            break;

        default:
            break;
    }

    timeLastCalculation = getCurrentTime() - tStart;
    return res;
}

// Return decode Coeffs for audio players that support pan & gain functions to reduce verbosity of the client side spatial mixer
std::vector<float> Mach1DecodeCore::decodePannedCoeffs(int bufferSize, int sampleIndex, bool applyPanLaw) {
    std::vector<float> coeffs = decodeCoeffs(bufferSize, sampleIndex);

    bool panLaw = applyPanLaw;
    std::vector<float> pannedCoeffs;

    for (int i = 0; i < (coeffs.size() - 2) / 2; i++) {
        float l = coeffs[i * 2];
        float r = coeffs[i * 2 + 1];
        float gain = 0.0;
        float pan = 0.0;

        if (l > r) {
            pan = -(1 - r / l);
            if (panLaw) {
                gain = l * (sqrt(2) / 2);
            } else {
                gain = l;
            }
        } else {
            pan = (1 - l / r);
            if (panLaw) {
                gain = r * (sqrt(2) / 2);
            } else {
                gain = r;
            }
        }
        // if gain is 0 reset pan
        if (gain == 0) {
            pan = 0;
        }
        // if pan is NaN reset pan
        if (std::isnan(pan)) {
            pan = 0.0;
        }
        pannedCoeffs.push_back(gain);
        pannedCoeffs.push_back(pan);
    }
    return pannedCoeffs;
}

// Decode using the current algorithm type in a more efficient way
void Mach1DecodeCore::decode(float Yaw, float Pitch, float Roll, float *result, int bufferSize, int sampleIndex) {
    setRotationDegrees({Yaw, Pitch, Roll});
    decodeCoeffs(result, bufferSize, sampleIndex);
}

void Mach1DecodeCore::decodeCoeffs(float *result, int bufferSize, int sampleIndex) {
    long tStart = getCurrentTime();

    float Yaw = fmod(rotation.x, 360.0); // protect a 360 cycle
    float Pitch = fmod(rotation.y, 360.0);
    float Roll = fmod(rotation.z, 360.0);

    switch (algorithmType) {
        case Mach1DecodeAlgoSpatial_4:
            spatialAlgo_4(Yaw, Pitch, Roll, result, bufferSize, sampleIndex);
            break;

        case Mach1DecodeAlgoSpatial_8:
            spatialAlgo_8(Yaw, Pitch, Roll, result, bufferSize, sampleIndex);
            break;

        case Mach1DecodeAlgoSpatial_12:
            spatialAlgo_12(Yaw, Pitch, Roll, result, bufferSize, sampleIndex);
            break;

        case Mach1DecodeAlgoSpatial_14:
            spatialAlgo_14(Yaw, Pitch, Roll, result, bufferSize, sampleIndex);
            break;

        default:
            break;
    }

    timeLastCalculation = getCurrentTime() - tStart;
}

void Mach1DecodeCore::decodePannedCoeffs(float *result, int bufferSize, int sampleIndex, bool applyPanLaw) {
    std::vector<float> coeffs = decodeCoeffs(bufferSize, sampleIndex);

    bool panLaw = applyPanLaw;

    for (int i = 0; i < (coeffs.size() - 2) / 2; i++) {
        float l = coeffs[i * 2];
        float r = coeffs[i * 2 + 1];
        float gain = 0.0;
        float pan = 0.0;

        if (l > r) {
            pan = -(1 - r / l);
            if (panLaw) {
                gain = l * (sqrt(2) / 2);
            } else {
                gain = l;
            }
        } else {
            pan = (1 - l / r);
            if (panLaw) {
                gain = r * (sqrt(2) / 2);
            } else {
                gain = r;
            }
        }
        // if gain is 0 reset pan
        if (gain == 0) {
            pan = 0;
        }
        // if pan is NaN reset pan
        if (std::isnan(pan)) {
            pan = 0.0;
        }
        result[i * 2] = gain;
        result[i * 2 + 1] = pan;
    }
}

void Mach1DecodeCore::decodeCoeffsUsingTranscodeMatrix(void *M1obj, float *matrix, int channels, float *result, int bufferSize, int sampleIndex) {
    std::vector<float> coeffs = decodeCoeffs(bufferSize, sampleIndex);

    int inChans = channels;
    int outChans = (getFormatChannelCount() - 1) / 2;
    int stereoChans = 2;

    for (int i = 0; i < inChans; i++) {
        for (int j = 0; j < stereoChans; j++) {
            result[i * stereoChans + j] = 0;
            for (int k = 0; k < outChans; k++) {
                result[i * stereoChans + j] += matrix[k * inChans + i] * coeffs[k * stereoChans + j];
            }
        }
    }
}

void Mach1DecodeCore::processSample(functionAlgoSampleHP funcAlgoSampleHP, float Yaw, float Pitch, float Roll, float *result, int bufferSize, int sampleIndex) {
    convertAnglesToMach1(platformType, &Yaw, &Pitch, &Roll);
    if (smoothAngles) {
        targetYaw = Yaw;
        targetPitch = Pitch;
        targetRoll = Roll;

        if (bufferSize > 0) {
            // we're in per sample mode
            // returning values from right here!

            std::vector<float> volumes1(getFormatCoeffCount());
            std::vector<float> volumes2(getFormatCoeffCount());
            (this->*funcAlgoSampleHP)(previousYaw, previousPitch, previousRoll, volumes1.data());
            (this->*funcAlgoSampleHP)(currentYaw, currentPitch, currentRoll, volumes2.data());
            float phase = (float)sampleIndex / (float)bufferSize;

            std::vector<float> volumes_lerp(getFormatCoeffCount());
            for (int i = 0; i < 16; i++) {
                volumes_lerp[i] = volumes1[i] * (1 - phase) + volumes2[i] * phase;
                result[i] = volumes_lerp[i];
            }
            return;
        } else {
            // Filtering per-buffer
            if (filterSpeed >= 1.0) {
                currentYaw = Yaw;
                currentPitch = Pitch;
                currentRoll = Roll;

                previousYaw = currentYaw;
                previousPitch = currentPitch;
                previousRoll = currentRoll;

            } else {
                Yaw = currentYaw;
                Pitch = currentPitch;
                Roll = currentRoll;

                previousYaw = currentYaw;
                previousPitch = currentPitch;
                previousRoll = currentRoll;
            }
        }
    } else {
        // for test purpose only!
        targetYaw = Yaw;
        targetPitch = Pitch;
        targetRoll = Roll;

        currentYaw = Yaw;
        currentPitch = Pitch;
        currentRoll = Roll;

        previousYaw = currentYaw;
        previousPitch = currentPitch;
        previousRoll = currentRoll;
    }

    // printf("%f, %f, %f\r\n", Yaw, Pitch, Roll);

    (this->*funcAlgoSampleHP)(Yaw, Pitch, Roll, result);
}

std::vector<float> Mach1DecodeCore::processSample(functionAlgoSample funcAlgoSample, float Yaw, float Pitch, float Roll, int bufferSize, int sampleIndex) {
    convertAnglesToMach1(platformType, &Yaw, &Pitch, &Roll);
    if (smoothAngles) {
        targetYaw = Yaw;
        targetPitch = Pitch;
        targetRoll = Roll;

        if (bufferSize > 0) {
            // we're in per sample mode
            // returning values from right here!
            std::vector<float> volumes1 = (this->*funcAlgoSample)(previousYaw, previousPitch, previousRoll);
            std::vector<float> volumes2 = (this->*funcAlgoSample)(currentYaw, currentPitch, currentRoll);
            float phase = (float)sampleIndex / (float)bufferSize;
            std::vector<float> volumes_lerp;
            volumes_lerp.resize(volumes1.size());
            for (int i = 0; i < volumes1.size(); i++) {
                volumes_lerp[i] = volumes1[i] * (1 - phase) + volumes2[i] * phase;
            }
            return volumes_lerp;
        } else {
            // Filtering per-buffer
            if (filterSpeed >= 1.0) {
                currentYaw = Yaw;
                currentPitch = Pitch;
                currentRoll = Roll;

                previousYaw = currentYaw;
                previousPitch = currentPitch;
                previousRoll = currentRoll;

            } else {
                Yaw = currentYaw;
                Pitch = currentPitch;
                Roll = currentRoll;

                previousYaw = currentYaw;
                previousPitch = currentPitch;
                previousRoll = currentRoll;
            }
        }
    } else {
        targetYaw = Yaw;
        targetPitch = Pitch;
        targetRoll = Roll;

        currentYaw = Yaw;
        currentPitch = Pitch;
        currentRoll = Roll;

        previousYaw = currentYaw;
        previousPitch = currentPitch;
        previousRoll = currentRoll;
    }
    return (this->*funcAlgoSample)(Yaw, Pitch, Roll);
}

//
//  Four channel audio format
//
//  Order of input angles:
//  Y = Yaw in degrees
//  P = Pitch in degrees
//  R = Roll in degrees
//

std::vector<float> Mach1DecodeCore::spatialAlgo_4(float Yaw, float Pitch, float Roll, int bufferSize, int sampleIndex) {

    const int numChannelPoints = 4;
    convertAnglesToMach1(platformType, &Yaw, &Pitch, &Roll);

    if (smoothAngles) {
        targetYaw = Yaw;
        targetPitch = Pitch;
        targetRoll = Roll;

        updateAngles();

        Yaw = currentYaw;
        Pitch = currentPitch;
        Roll = currentRoll;
    } else {
        targetYaw = Yaw;
        targetPitch = Pitch;
        targetRoll = Roll;

        currentYaw = Yaw;
        currentPitch = Pitch;
        currentRoll = Roll;

        previousYaw = currentYaw;
        previousPitch = currentPitch;
        previousRoll = currentRoll;
    }

    // Orientation input safety clamps/alignment
    Yaw = alignAngle(Yaw, 0, 360);

    float coefficients[numChannelPoints];
    coefficients[0] = 1.f - std::min(1.f, std::min(360.f - Yaw, Yaw) / 90.f);
    coefficients[1] = 1.f - std::min(1.f, std::abs(90.f - Yaw) / 90.f);
    coefficients[2] = 1.f - std::min(1.f, std::abs(180.f - Yaw) / 90.f);
    coefficients[3] = 1.f - std::min(1.f, std::abs(270.f - Yaw) / 90.f);

    std::vector<float> result;
    result.resize(numChannelPoints * 2);
    result[0] = coefficients[0]; // 1 left
    result[1] = coefficients[3]; //   right
    result[2] = coefficients[1]; // 2 left
    result[3] = coefficients[0]; //   right
    result[4] = coefficients[3]; // 3 left
    result[5] = coefficients[2]; //   right
    result[6] = coefficients[2]; // 4 left
    result[7] = coefficients[1]; //   right
    return result;
}

void Mach1DecodeCore::spatialAlgo_4(float Yaw, float Pitch, float Roll, float *result, int bufferSize, int sampleIndex) {

    const int numChannelPoints = 4;
    convertAnglesToMach1(platformType, &Yaw, &Pitch, &Roll);

    if (smoothAngles) {
        targetYaw = Yaw;
        targetPitch = Pitch;
        targetRoll = Roll;

        updateAngles();

        Yaw = currentYaw;
        Pitch = currentPitch;
        Roll = currentRoll;
    } else {
        targetYaw = Yaw;
        targetPitch = Pitch;
        targetRoll = Roll;

        currentYaw = Yaw;
        currentPitch = Pitch;
        currentRoll = Roll;

        previousYaw = currentYaw;
        previousPitch = currentPitch;
        previousRoll = currentRoll;
    }

    // Orientation input safety clamps/alignment
    Yaw = alignAngle(Yaw, 0, 360);

    float coefficients[numChannelPoints];
    coefficients[0] = 1.f - std::min(1.f, std::min(360.f - Yaw, Yaw) / 90.f);
    coefficients[1] = 1.f - std::min(1.f, std::abs(90.f - Yaw) / 90.f);
    coefficients[2] = 1.f - std::min(1.f, std::abs(180.f - Yaw) / 90.f);
    coefficients[3] = 1.f - std::min(1.f, std::abs(270.f - Yaw) / 90.f);

    //result.resize(numChannelPoints * 2);
    result[0] = coefficients[0]; // 1 left
    result[1] = coefficients[3]; //   right
    result[2] = coefficients[1]; // 2 left
    result[3] = coefficients[0]; //   right
    result[4] = coefficients[3]; // 3 left
    result[5] = coefficients[2]; //   right
    result[6] = coefficients[2]; // 4 left
    result[7] = coefficients[1]; //   right
}

// ------------------------------------------------------------------

//
//  Eight channel audio layout (isotropic version).
//
//  Order of input angles:
//  Y = Yaw in degrees
//  P = Pitch in degrees
//  R = Roll in degrees
//

std::vector<float> Mach1DecodeCore::spatialAlgo_8(float Yaw, float Pitch, float Roll,
                                                  int bufferSize, int sampleIndex) {

    return processSample(&Mach1DecodeCore::spatialAlgoSample_8, Yaw, Pitch, Roll, bufferSize, sampleIndex);
}

void Mach1DecodeCore::spatialAlgo_8(float Yaw, float Pitch, float Roll, float *result, int bufferSize, int sampleIndex) {
    processSample(&Mach1DecodeCore::spatialAlgoSample_8, Yaw, Pitch, Roll, result, bufferSize, sampleIndex);
}

// ------------------------------------------------------------------

//
//  Additional channel audio layouts (isotropic version).
//
//  Order of input angles:
//  Y = Yaw in degrees
//  P = Pitch in degrees
//  R = Roll in degrees
//

std::vector<float> Mach1DecodeCore::spatialAlgo_12(float Yaw, float Pitch, float Roll,
                                                   int bufferSize, int sampleIndex) {

    return processSample(&Mach1DecodeCore::spatialAlgoSample_12, Yaw, Pitch, Roll, bufferSize, sampleIndex);
}

void Mach1DecodeCore::spatialAlgo_12(float Yaw, float Pitch, float Roll, float *result,
                                     int bufferSize, int sampleIndex) {

    return processSample(&Mach1DecodeCore::spatialAlgoSample_12, Yaw, Pitch, Roll, result, bufferSize, sampleIndex);
}

// ------------------------------------------------------------------

std::vector<float> Mach1DecodeCore::spatialAlgo_14(float Yaw, float Pitch, float Roll,
                                                   int bufferSize, int sampleIndex) {

    return processSample(&Mach1DecodeCore::spatialAlgoSample_14, Yaw, Pitch, Roll, bufferSize, sampleIndex);
}

void Mach1DecodeCore::spatialAlgo_14(float Yaw, float Pitch, float Roll, float *result,
                                     int bufferSize, int sampleIndex) {

    return processSample(&Mach1DecodeCore::spatialAlgoSample_14, Yaw, Pitch, Roll, result, bufferSize, sampleIndex);
}

// ------------------------------------------------------------------
