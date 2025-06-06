//  Mach1 Spatial SDK
//  Copyright © 2017 Mach1. All rights reserved.

/*
DISCLAIMER:
This file is not an example of use but an decoder that will require periodic
updates and should not be integrated in sections but remain as an update-able factored file.
*/

/*
Internal Orientation Implementation:
    -  Yaw   [0]+ = rotate right 0-1 [Range: 0->360 | -180->180]
    -  Yaw   [0]- = rotate left 0-1  [Range: 0->360 | -180->180]
    -  Pitch [1]+ = rotate up 0-1    [Range: -90->90]
    -  Pitch [1]- = rotate down 0-1  [Range: -90->90]
    -  Roll  [2]+ = rotate up 0-1    [Range: -90->90]
    -  Roll  [2]- = rotate down 0-1  [Range: -90->90]

M1DecodeCore normalizes all input ranges to an unsigned "0 to 1" range for Yaw, Pitch and Roll.
 */

#include "Mach1DecodeCore.h"
#include <string>

#ifndef __ANDROID__
// TODO: Remove this and figure out how to get ANDROID builds to accept newer stdlib
#    define copysign std::copysign
#endif

#ifndef DEG_TO_RAD
#    define DEG_TO_RAD (PI / 180.0)
#endif

#ifndef __FLT_EPSILON__
#    define __FLT_EPSILON__ 1.19209290e-07F
#endif

float M1DecodeCore::lerp(float x1, float x2, float t) {
    return x1 + (x2 - x1) * t;
}

float M1DecodeCore::mDegToRad(float degrees) {
    return (float)(degrees * DEG_TO_RAD);
}

//
// Map utility
//

float M1DecodeCore::mmap(float value, float inputMin, float inputMax, float outputMin, float outputMax, bool clamp) {

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

float M1DecodeCore::clamp(float a, float min, float max) {
    return (a < min) ? min : ((a > max) ? max : a);
}

float M1DecodeCore::alignAngle(float a, float min, float max) {
    if (a > 5000 || a < -5000)
        return 0;

    while (a < min)
        a += 360;
    while (a > max)
        a -= 360;

    return a;
}

float M1DecodeCore::radialDistance(float angle1, float angle2) {
    if ((std::abs(angle2 - angle1)) > (std::abs(std::abs(angle2 - angle1) - 360))) {
        return std::abs(std::abs(angle2 - angle1) - 360);
    } else
        return std::abs(angle2 - angle1);
}

float M1DecodeCore::targetDirectionMultiplier(float angleCurrent, float angleTarget) {
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
void M1DecodeCore::updateAngles() {
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

float M1DecodeCore::_dot(const Mach1Point3D &p1, const Mach1Point3D &p2) const {
    return p1.x * p2.x + p1.y * p2.y + p1.z * p2.z;
}

bool M1DecodeCore::linePlaneIntersection(Mach1Point3D &contact, Mach1Point3D ray, Mach1Point3D rayOrigin, Mach1Point3D normal, Mach1Point3D coord) {
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

void M1DecodeCore::spatialMultichannelAlgo(Mach1Point3D *channelPoints, int numChannelPoints, float Yaw, float Pitch, float Roll, float *result) {
    
    if (filterSpeed <= 1.0f && filterSpeed > 0.0f) { // filter and lerp the input angles for smoothing
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
    
    Mach1Point3D simulationAngles;
    simulationAngles.x = Yaw;
    simulationAngles.y = Pitch;
    simulationAngles.z = Roll;

    Mach1Point3D fVec_1a = {sin(mDegToRad(simulationAngles[0])), cos(mDegToRad(simulationAngles[0])), 0};
    fVec_1a.normalize();
    Mach1Point3D fVec_1b = {sin(mDegToRad(simulationAngles[0] - 90)), cos(mDegToRad(simulationAngles[0] - 90)), 0};
    fVec_1b.normalize();

    Mach1Point3D fVec_2a = fVec_1a.getRotated(-simulationAngles[1], fVec_1b);
    Mach1Point3D fVec_2b = fVec_1a.getRotated(-simulationAngles[1] - 90, fVec_1b);

    Mach1Point3D fVecL = fVec_2b.getRotated(simulationAngles[2] - 90, fVec_2a);
    Mach1Point3D fVecR = fVec_2b.getRotated(simulationAngles[2] + 90, fVec_2a);

    Mach1Point3D contactL = fVecL + fVec_2a;
    Mach1Point3D contactR = fVecR + fVec_2a;

    float d = sqrtf(5); // 100*100+200*200

    std::vector<float> vL(numChannelPoints);
    std::vector<float> vR(numChannelPoints);

    std::vector<float> vL_clamped(numChannelPoints);
    std::vector<float> vR_clamped(numChannelPoints);

    // Calculate pitch influence (0 = horizontal plane, 1 = directly up, -1 = directly down)
    float pitchInfluence = sin(mDegToRad(Pitch));  // -1 to +1

    for (int i = 0; i < numChannelPoints; i++) {
        // Calculate vertical attenuation
        // When looking up (+pitch), attenuate lower channels
        // When looking down (-pitch), attenuate upper channels
        float verticalAttenuation;
        if (pitchInfluence >= 0) {
            // Looking up - attenuate lower channels
            verticalAttenuation = channelPoints[i].z < 0 ? (1.0f - pitchInfluence) : 1.0f;
        } else {
            // Looking down - attenuate upper channels
            verticalAttenuation = channelPoints[i].z > 0 ? (1.0f + pitchInfluence) : 1.0f;
        }

        Mach1Point3D qL = (contactL - channelPoints[i]);
        Mach1Point3D qR = (contactR - channelPoints[i]);

        vL[i] = qL.length();
        vR[i] = qR.length();

        vL_clamped[i] = M1DecodeCore::clamp(M1DecodeCore::mmap(vL[i], 0, d, 1.f, 0.f, false), 0, 1);
        vR_clamped[i] = M1DecodeCore::clamp(M1DecodeCore::mmap(vR[i], 0, d, 1.f, 0.f, false), 0, 1);

        // Combine horizontal and vertical influences
        result[i * 2 + 0] = vL_clamped[i] * verticalAttenuation;
        result[i * 2 + 1] = vR_clamped[i] * verticalAttenuation;
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
}

/*
 Defined multichannel spatial layouts

 Mach1 XYZ Coordinate Expectation:
 X (left -> right | where -X is left)
 Y (front -> back | where -Y is back)
 Z (top -> bottom | where -Z is bottom)

 */

void M1DecodeCore::spatialAlgo_4(float Yaw, float Pitch, float Roll, float *result) {
    const int numChannelPoints = 4;

    // Ideally Z should be 0, but this results in unexpected results from a lack of a 3D shape
    Mach1Point3D channelPoints[numChannelPoints] =
        {
            {-1, 1, 1},
            {1, 1, 1},
            {-1, -1, 1},
            {1, -1, 1},
        };

    spatialMultichannelAlgo(channelPoints, numChannelPoints, Yaw, Pitch, Roll, result);
}

std::vector<float> M1DecodeCore::spatialAlgo_4(float Yaw, float Pitch, float Roll) {
    const int numChannelPoints = 4;

    std::vector<float> result;
    result.resize(numChannelPoints * 2);

    // Ideally Z should be 0, but this results in unexpected results from a lack of a 3D shape
    Mach1Point3D channelPoints[numChannelPoints] =
        {
            {-1, 1, 1},
            {1, 1, 1},
            {-1, -1, 1},
            {1, -1, 1},
        };

    spatialMultichannelAlgo(channelPoints, numChannelPoints, Yaw, Pitch, Roll, result.data());

    return result;
}

void M1DecodeCore::spatialAlgo_8(float Yaw, float Pitch, float Roll, float *result) {
    const int numChannelPoints = 8;

    Mach1Point3D channelPoints[numChannelPoints] =
        {
            {-1, 1, 1},
            {1, 1, 1},
            {-1, -1, 1},
            {1, -1, 1},

            {-1, 1, -1},
            {1, 1, -1},
            {-1, -1, -1},
            {1, -1, -1},
        };

    spatialMultichannelAlgo(channelPoints, numChannelPoints, Yaw, Pitch, Roll, result);
}

std::vector<float> M1DecodeCore::spatialAlgo_8(float Yaw, float Pitch, float Roll) {
    const int numChannelPoints = 8;

    std::vector<float> result;
    result.resize(numChannelPoints * 2);

    Mach1Point3D channelPoints[numChannelPoints] =
        {
            {-1, 1, 1},
            {1, 1, 1},
            {-1, -1, 1},
            {1, -1, 1},

            {-1, 1, -1},
            {1, 1, -1},
            {-1, -1, -1},
            {1, -1, -1},
        };

    spatialMultichannelAlgo(channelPoints, numChannelPoints, Yaw, Pitch, Roll, result.data());

    return result;
}

void M1DecodeCore::spatialAlgo_14(float Yaw, float Pitch, float Roll, float *result) {
    const int numChannelPoints = 8 + 4 + 2;

    float diag = sqrtf(2);

    Mach1Point3D channelPoints[numChannelPoints] =
        {
            {-1, 1, 1},
            {1, 1, 1},
            {-1, -1, 1},
            {1, -1, 1},

            {-1, 1, -1},
            {1, 1, -1},
            {-1, -1, -1},
            {1, -1, -1},

            {0, diag, 0},
            {diag, 0, 0},
            {0, -diag, 0},
            {-diag, 0, 0},

            {0, 0, diag},
            {0, 0, -diag},
        };

    spatialMultichannelAlgo(channelPoints, numChannelPoints, Yaw, Pitch, Roll, result);
}

std::vector<float> M1DecodeCore::spatialAlgo_14(float Yaw, float Pitch, float Roll) {
    const int numChannelPoints = 8 + 4 + 2;

    float diag = sqrtf(2);

    std::vector<float> result;
    result.resize(numChannelPoints * 2);

    Mach1Point3D channelPoints[numChannelPoints] =
        {
            {-1, 1, 1},
            {1, 1, 1},
            {-1, -1, 1},
            {1, -1, 1},

            {-1, 1, -1},
            {1, 1, -1},
            {-1, -1, -1},
            {1, -1, -1},

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
void M1DecodeCore::convertAnglesToMach1(Mach1PlatformType platformType, float *Y, float *P, float *R) {
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

void M1DecodeCore::convertAnglesToPlatform(Mach1PlatformType platformType, float *Y, float *P, float *R) {
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

Mach1Point3D M1DecodeCore::getCurrentAngle() {
    Mach1Point3D angle = {currentYaw, currentPitch, currentRoll};
    return angle;
}

void M1DecodeCore::addToLog(std::string str, int maxCount) {
    if (strLog.size() > maxCount)
        strLog.erase(strLog.begin() + maxCount, strLog.begin() + strLog.size() - 1);

    strLog.push_back(str);
}

char *M1DecodeCore::getLog() {
    std::vector<std::string> _strLog = strLog;
    strLog.clear();

    static char log[100000] = "";

    log[0] = '\0';
    /*
    for (int i = 0; i<_strLog.size(); i++) strcat(log, _strLog[i].c_str());
     */
    return log;
}

M1DecodeCore::M1DecodeCore() {
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
    decodeMode = M1DecodeSpatial_8;

    ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

    strLog.resize(0);
}

long M1DecodeCore::getCurrentTime() {
    return (long)(duration_cast<milliseconds>(system_clock::now().time_since_epoch()) - ms).count();
}

long M1DecodeCore::getLastCalculationTime() {
    return timeLastCalculation;
}

void M1DecodeCore::setPlatformType(Mach1PlatformType type) {
    platformType = type;
}

Mach1PlatformType M1DecodeCore::getPlatformType() {
    return platformType;
}

int M1DecodeCore::getFormatChannelCount() {
    switch (decodeMode) {
    case M1DecodeSpatial_4:
        return 4;
    case M1DecodeSpatial_8:
        return 8;
    case M1DecodeSpatial_14:
        return 14;
    }
    return 0;
}

int M1DecodeCore::getFormatCoeffCount() {
    switch (decodeMode) {
    case M1DecodeSpatial_4:
        return (4 * 2);
    case M1DecodeSpatial_8:
        return (8 * 2);
    case M1DecodeSpatial_14:
        return (14 * 2);
    }
    return 0;
}

void M1DecodeCore::setRotation(Mach1Point3D newRotationFromMinusOnetoOne) {
    rotation = newRotationFromMinusOnetoOne * 360.0;
}

void M1DecodeCore::setRotationDegrees(Mach1Point3D newRotationDegrees) {
    rotation = newRotationDegrees;
}

void M1DecodeCore::setRotationRadians(Mach1Point3D newRotationRadians) {
    rotation = newRotationRadians * (180.0 / PI);
}

void M1DecodeCore::setRotationQuat(Mach1Point4D newRotationQuat) {
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

void M1DecodeCore::setFilterSpeed(float newFilterSpeed) {
    filterSpeed = newFilterSpeed;
}

//--------------------------------------------------

void M1DecodeCore::setDecodeMode(Mach1DecodeMode mode) {
    decodeMode = mode;
}

Mach1DecodeMode M1DecodeCore::getDecodeMode() {
    return decodeMode;
}

std::vector<float> M1DecodeCore::decode(float Yaw, float Pitch, float Roll, int bufferSize, int sampleIndex) {
    setRotationDegrees({Yaw, Pitch, Roll});
    return decodeCoeffs(bufferSize, sampleIndex);
}

std::vector<float> M1DecodeCore::decodeCoeffs(int bufferSize, int sampleIndex) {
    long tStart = getCurrentTime();
    std::vector<float> coeffs;

    float yaw = fmod(rotation.x, 360.0); // protect a 360 cycle
    float pitch = fmod(rotation.y, 360.0);
    float roll = fmod(rotation.z, 360.0);
    
    switch (decodeMode) {
    case M1DecodeSpatial_4:
        coeffs = processSample(&M1DecodeCore::spatialAlgo_4, yaw, pitch, roll, bufferSize, sampleIndex);
        break;

    case M1DecodeSpatial_8:
        coeffs = processSample(&M1DecodeCore::spatialAlgo_8, yaw, pitch, roll, bufferSize, sampleIndex);
        break;

    case M1DecodeSpatial_14:
        coeffs = processSample(&M1DecodeCore::spatialAlgo_14, yaw, pitch, roll, bufferSize, sampleIndex);
        break;

    default:
        break;
    }
    
    timeLastCalculation = getCurrentTime() - tStart;
    return coeffs;
}

// Return decode Coeffs for audio players that support pan & gain functions to reduce verbosity of the client side spatial mixer
std::vector<float> M1DecodeCore::decodePannedCoeffs(int bufferSize, int sampleIndex, bool applyPanLaw) {
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
void M1DecodeCore::decode(float Yaw, float Pitch, float Roll, float *result, int bufferSize, int sampleIndex) {
    setRotationDegrees({Yaw, Pitch, Roll});
    decodeCoeffs(result, bufferSize, sampleIndex);
}

void M1DecodeCore::decodeCoeffs(float *result, int bufferSize, int sampleIndex) {
    long tStart = getCurrentTime();

    float Yaw = fmod(rotation.x, 360.0); // protect a 360 cycle
    float Pitch = fmod(rotation.y, 360.0);
    float Roll = fmod(rotation.z, 360.0);

    switch (decodeMode) {
    case M1DecodeSpatial_4:
        processSample(&M1DecodeCore::spatialAlgo_4, Yaw, Pitch, Roll, result, bufferSize, sampleIndex);
        break;

    case M1DecodeSpatial_8:
        processSample(&M1DecodeCore::spatialAlgo_8, Yaw, Pitch, Roll, result, bufferSize, sampleIndex);
        break;

    case M1DecodeSpatial_14:
        processSample(&M1DecodeCore::spatialAlgo_14, Yaw, Pitch, Roll, result, bufferSize, sampleIndex);
        break;

    default:
        break;
    }
    
    timeLastCalculation = getCurrentTime() - tStart;
}

void M1DecodeCore::decodePannedCoeffs(float *result, int bufferSize, int sampleIndex, bool applyPanLaw) {
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

void M1DecodeCore::decodeCoeffsUsingTranscodeMatrix(void *M1obj, float *matrix, int channels, float *result, int bufferSize, int sampleIndex) {
    std::vector<float> coeffs = decodeCoeffs(bufferSize, sampleIndex);

    int inChans = channels;
    int outChans = getFormatCoeffCount();
    int outStereoDecodeChans = 2;

    for (int i = 0; i < inChans; i++) {
        for (int j = 0; j < outStereoDecodeChans; j++) {
            result[i * outStereoDecodeChans + j] = 0;
            for (int k = 0; k < outChans; k++) {
                result[i * outStereoDecodeChans + j] += matrix[k * inChans + i] * coeffs[k * outStereoDecodeChans + j];
            }
        }
    }
}

void M1DecodeCore::processSample(processSampleForMultichannelPtr _processSampleForMultichannelPtr, float Yaw, float Pitch, float Roll, float *result, int bufferSize, int sampleIndex) {
    convertAnglesToMach1(platformType, &Yaw, &Pitch, &Roll);
    
    targetYaw = Yaw;
    targetPitch = Pitch;
    targetRoll = Roll;

    if (bufferSize > 0) {
        // we're in per sample mode
        // returning values from right here!

        std::vector<float> gainsL(getFormatCoeffCount());
        std::vector<float> gainsR(getFormatCoeffCount());
        (this->*_processSampleForMultichannelPtr)(currentYaw, currentPitch, currentRoll, gainsL.data());
        (this->*_processSampleForMultichannelPtr)(currentYaw, currentPitch, currentRoll, gainsR.data());
        float phase = (float)sampleIndex / (float)bufferSize;

        std::vector<float> gains_lerp(getFormatCoeffCount());
        for (int i = 0; i < gains_lerp.size(); i++) {
            gains_lerp[i] = gainsL[i] * (1 - phase) + gainsR[i] * phase;
            result[i] = gains_lerp[i];
        }
        return;
    } else {
        // Filtering per-buffer
        if (filterSpeed <= 1.0f && filterSpeed > 0.0f) { // filter and lerp the input angles for smoothing
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
    }

    // printf("%f, %f, %f\r\n", Yaw, Pitch, Roll);

    (this->*_processSampleForMultichannelPtr)(Yaw, Pitch, Roll, result);
}

std::vector<float> M1DecodeCore::processSample(processSampleForMultichannel _processSampleForMultichannel, float Yaw, float Pitch, float Roll, int bufferSize, int sampleIndex) {
    convertAnglesToMach1(platformType, &Yaw, &Pitch, &Roll);
    
    targetYaw = Yaw;
    targetPitch = Pitch;
    targetRoll = Roll;

    if (bufferSize > 0) {
        // we're in per sample mode
        // returning values from right here!
        std::vector<float> gainsL = (this->*_processSampleForMultichannel)(currentYaw, currentPitch, currentRoll);
        std::vector<float> gainsR = (this->*_processSampleForMultichannel)(currentYaw, currentPitch, currentRoll);
        float phase = (float)sampleIndex / (float)bufferSize;
        std::vector<float> gains_lerp;
        gains_lerp.resize(gainsL.size());
        for (int i = 0; i < gainsL.size(); i++) {
            gains_lerp[i] = gainsL[i] * (1 - phase) + gainsR[i] * phase;
        }
        return gains_lerp;
    } else {
        // Filtering per-buffer
        if (filterSpeed <= 1.0f && filterSpeed > 0.0f) { // filter and lerp the input angles for smoothing
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
    }
    return (this->*_processSampleForMultichannel)(Yaw, Pitch, Roll);
}
