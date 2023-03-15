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

#include <algorithm>
#include <chrono>
#include <cmath>
#include <string>

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

// Envelope follower feature is defined here, in updateAngles()
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

float Mach1DecodeCore::_dot(const Mach1Point3DCore &p1, const Mach1Point3DCore &p2) const {
    return p1.x * p2.x + p1.y * p2.y + p1.z * p2.z;
}

bool Mach1DecodeCore::linePlaneIntersection(Mach1Point3DCore &contact, Mach1Point3DCore ray, Mach1Point3DCore rayOrigin, Mach1Point3DCore normal, Mach1Point3DCore coord) {
    float epsilon = 1e-6f;

    Mach1Point3DCore u = rayOrigin - ray;
    float dot = _dot(normal, u);

    if (fabs(dot) > epsilon) {
        Mach1Point3DCore w = ray - coord;
        float fac = -_dot(normal, w) / dot;
        if (fac >= 0.0 && fac <= 1.0) {
            u = u * fac;
            contact = ray + u;
            return true;
        }
    }

    return false;
}

void Mach1DecodeCore::horizonPairsAlgoSample(float Yaw, float Pitch, float Roll, float *result) {
    // Orientation input safety clamps/alignment
    Yaw = alignAngle(Yaw, 0, 360);

    result[0] = 1.f - std::min(1.f, std::min((float)360.f - Yaw, Yaw) / 90.f);
    result[1] = 1.f - std::min(1.f, std::min((float)360.f - Yaw, Yaw) / 90.f);
    result[2] = 1.f - std::min(1.f, std::abs((float)90.f - Yaw) / 90.f);
    result[3] = 1.f - std::min(1.f, std::abs((float)90.f - Yaw) / 90.f);
    result[4] = 1.f - std::min(1.f, std::abs((float)180.f - Yaw) / 90.f);
    result[5] = 1.f - std::min(1.f, std::abs((float)180.f - Yaw) / 90.f);
    result[6] = 1.f - std::min(1.f, std::abs((float)270.f - Yaw) / 90.f);
    result[7] = 1.f - std::min(1.f, std::abs((float)270.f - Yaw) / 90.f);
}

std::vector<float> Mach1DecodeCore::horizonPairsAlgoSample(float Yaw, float Pitch, float Roll) {
    // Orientation input safety clamps/alignment
    Yaw = alignAngle(Yaw, 0, 360);

    float volumes[8];
    volumes[0] = 1.f - std::min(1.f, std::min((float)360.f - Yaw, Yaw) / 90.f);
    volumes[1] = 1.f - std::min(1.f, std::min((float)360.f - Yaw, Yaw) / 90.f);
    volumes[2] = 1.f - std::min(1.f, std::abs((float)90.f - Yaw) / 90.f);
    volumes[3] = 1.f - std::min(1.f, std::abs((float)90.f - Yaw) / 90.f);
    volumes[4] = 1.f - std::min(1.f, std::abs((float)180.f - Yaw) / 90.f);
    volumes[5] = 1.f - std::min(1.f, std::abs((float)180.f - Yaw) / 90.f);
    volumes[6] = 1.f - std::min(1.f, std::abs((float)270.f - Yaw) / 90.f);
    volumes[7] = 1.f - std::min(1.f, std::abs((float)270.f - Yaw) / 90.f);

    std::vector<float> result;
    result.push_back(volumes[0]);
    result.push_back(volumes[1]);
    result.push_back(volumes[2]);
    result.push_back(volumes[3]);
    result.push_back(volumes[4]);
    result.push_back(volumes[5]);
    result.push_back(volumes[6]);
    result.push_back(volumes[7]);

    return result;
}

void Mach1DecodeCore::spatialMultichannelAlgo(Mach1Point3DCore *channelPoints, int numChannelPoints, float Yaw, float Pitch, float Roll, float *result) {
    // TODO: change this so it can be scaled
    Mach1Point3DCore simulationAngles = Mach1Point3DCore(Yaw, Pitch, Roll);

    Mach1Point3DCore faceVector1 = Mach1Point3DCore(
                                       sin(mDegToRad(simulationAngles[0])),
                                       cos(mDegToRad(simulationAngles[0])))
                                       .normalize();

    Mach1Point3DCore faceVector11 = Mach1Point3DCore(
                                        sin(mDegToRad(simulationAngles[0] - 90)),
                                        cos(mDegToRad(simulationAngles[0] - 90)))
                                        .normalize();

    Mach1Point3DCore faceVector2 = faceVector1.getRotated(-simulationAngles[1], faceVector11);
    Mach1Point3DCore faceVector21 = faceVector1.getRotated(-simulationAngles[1] - 90, faceVector11);

    Mach1Point3DCore faceVectorLeft = faceVector21.getRotated(simulationAngles[2] - 90, faceVector2);
    Mach1Point3DCore faceVectorRight = faceVector21.getRotated(simulationAngles[2] + 90, faceVector2);

    Mach1Point3DCore planes[8][2] =
        {
            {Mach1Point3DCore(1, 0, 0), Mach1Point3DCore(100, 0, 0)},
            {Mach1Point3DCore(-1, 0, 0), Mach1Point3DCore(-100, 0, 0)},
            {Mach1Point3DCore(0, 1, 0), Mach1Point3DCore(0, 100, 0)},
            {Mach1Point3DCore(0, -1, 0), Mach1Point3DCore(0, -100, 0)},
            {Mach1Point3DCore(0, 0, 1), Mach1Point3DCore(0, 0, 100)},
            {Mach1Point3DCore(0, 0, -1), Mach1Point3DCore(0, 0, -100)}};

    Mach1Point3DCore contactL = faceVectorLeft * 100 + faceVector2 * 100;
    Mach1Point3DCore contactR = faceVectorRight * 100 + faceVector2 * 100;

    // check for intersection with cube
    for (int j = 0; j < 8; j++) {
        linePlaneIntersection(contactL, Mach1Point3DCore(0, 0, 0), faceVectorLeft * 100 + faceVector2 * 100, planes[j][0], planes[j][1]);
        linePlaneIntersection(contactR, Mach1Point3DCore(0, 0, 0), faceVectorRight * 100 + faceVector2 * 100, planes[j][0], planes[j][1]);
    }

    float d = sqrtf(100 * 100 + 200 * 200);

    std::vector<float> vL(numChannelPoints);
    std::vector<float> vR(numChannelPoints);

    std::vector<float> vL_clamped(numChannelPoints);
    std::vector<float> vR_clamped(numChannelPoints);

    for (int i = 0; i < numChannelPoints; i++) {
        Mach1Point3DCore qL = (contactL - channelPoints[i]);
        Mach1Point3DCore qR = (contactR - channelPoints[i]);

        vL[i] = qL.length();
        vR[i] = qR.length();

        vL_clamped[i] = Mach1DecodeCore::clamp(Mach1DecodeCore::mmap(vL[i], 0, d, 1.f, 0.f, false), 0, 1);
        vR_clamped[i] = Mach1DecodeCore::clamp(Mach1DecodeCore::mmap(vR[i], 0, d, 1.f, 0.f, false), 0, 1);

        result[i * 2 + 0] = vL_clamped[i];
        result[i * 2 + 1] = vR_clamped[i];
    }

    // Volume Balancer v2.0
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

    result[numChannelPoints * 2 + 0] = 1.0f; // static stereo L
    result[numChannelPoints * 2 + 1] = 1.0f; // static stereo R
}

void Mach1DecodeCore::spatialAlgoSample_8(float Yaw, float Pitch, float Roll, float *result) {
    const int numChannelPoints = 8;

    Mach1Point3DCore channelPoints[numChannelPoints] =
        {
            /*
            Mach1 XYZ Coordinate Expectation:
                X (left -> right | where -X is left)
                Y (front -> back | where -Y is back)
                Z (top -> bottom | where -Z is bottom)
             */
            Mach1Point3DCore(-100, 100, 100),
            Mach1Point3DCore(100, 100, 100),
            Mach1Point3DCore(-100, -100, 100),
            Mach1Point3DCore(100, -100, 100),

            Mach1Point3DCore(-100, 100, -100),
            Mach1Point3DCore(100, 100, -100),
            Mach1Point3DCore(-100, -100, -100),
            Mach1Point3DCore(100, -100, -100),
        };

    spatialMultichannelAlgo(channelPoints, numChannelPoints, Yaw, Pitch, Roll, result);
}

std::vector<float> Mach1DecodeCore::spatialAlgoSample_8(float Yaw, float Pitch, float Roll) {
    const int numChannelPoints = 8;

    std::vector<float> result;
    result.resize(numChannelPoints * 2 + 2);

    Mach1Point3DCore channelPoints[numChannelPoints] =
        {
            Mach1Point3DCore(-100, 100, 100),
            Mach1Point3DCore(100, 100, 100),
            Mach1Point3DCore(-100, -100, 100),
            Mach1Point3DCore(100, -100, 100),

            Mach1Point3DCore(-100, 100, -100),
            Mach1Point3DCore(100, 100, -100),
            Mach1Point3DCore(-100, -100, -100),
            Mach1Point3DCore(100, -100, -100),
        };

    spatialMultichannelAlgo(channelPoints, numChannelPoints, Yaw, Pitch, Roll, result.data());

    return result;
}

void Mach1DecodeCore::spatialAlgoSample_12(float Yaw, float Pitch, float Roll, float *result) {
    const int numChannelPoints = 8 + 4;

    float diag = sqrtf(2 * 100 * 100);

    Mach1Point3DCore channelPoints[numChannelPoints] =
        {
            Mach1Point3DCore(-100, 100, 100),
            Mach1Point3DCore(100, 100, 100),
            Mach1Point3DCore(-100, -100, 100),
            Mach1Point3DCore(100, -100, 100),

            Mach1Point3DCore(-100, 100, -100),
            Mach1Point3DCore(100, 100, -100),
            Mach1Point3DCore(-100, -100, -100),
            Mach1Point3DCore(100, -100, -100),

            Mach1Point3DCore(diag, 0, 0),
            Mach1Point3DCore(-diag, 0, 0),
            Mach1Point3DCore(0, diag, 0),
            Mach1Point3DCore(0, -diag, 0),
        };

    spatialMultichannelAlgo(channelPoints, numChannelPoints, Yaw, Pitch, Roll, result);
}

std::vector<float> Mach1DecodeCore::spatialAlgoSample_12(float Yaw, float Pitch, float Roll) {
    const int numChannelPoints = 8 + 4;

    float diag = sqrtf(2 * 100 * 100);

    std::vector<float> result;
    result.resize(numChannelPoints * 2 + 2);

    Mach1Point3DCore channelPoints[numChannelPoints] =
        {
            Mach1Point3DCore(-100, 100, 100),
            Mach1Point3DCore(100, 100, 100),
            Mach1Point3DCore(-100, -100, 100),
            Mach1Point3DCore(100, -100, 100),

            Mach1Point3DCore(-100, 100, -100),
            Mach1Point3DCore(100, 100, -100),
            Mach1Point3DCore(-100, -100, -100),
            Mach1Point3DCore(100, -100, -100),

            Mach1Point3DCore(diag, 0, 0),
            Mach1Point3DCore(-diag, 0, 0),
            Mach1Point3DCore(0, diag, 0),
            Mach1Point3DCore(0, -diag, 0),
        };

    spatialMultichannelAlgo(channelPoints, numChannelPoints, Yaw, Pitch, Roll, result.data());

    return result;
}

void Mach1DecodeCore::spatialAlgoSample_14(float Yaw, float Pitch, float Roll, float *result) {
    const int numChannelPoints = 8 + 4 + 2;

    float diag = sqrtf(2 * 100 * 100);

    Mach1Point3DCore channelPoints[numChannelPoints] =
        {
            Mach1Point3DCore(-100, 100, 100),
            Mach1Point3DCore(100, 100, 100),
            Mach1Point3DCore(-100, -100, 100),
            Mach1Point3DCore(100, -100, 100),

            Mach1Point3DCore(-100, 100, -100),
            Mach1Point3DCore(100, 100, -100),
            Mach1Point3DCore(-100, -100, -100),
            Mach1Point3DCore(100, -100, -100),

            Mach1Point3DCore(diag, 0, 0),
            Mach1Point3DCore(-diag, 0, 0),
            Mach1Point3DCore(0, diag, 0),
            Mach1Point3DCore(0, -diag, 0),

            Mach1Point3DCore(0, 0, diag),
            Mach1Point3DCore(0, 0, -diag),
        };

    spatialMultichannelAlgo(channelPoints, numChannelPoints, Yaw, Pitch, Roll, result);
}

std::vector<float> Mach1DecodeCore::spatialAlgoSample_14(float Yaw, float Pitch, float Roll) {
    const int numChannelPoints = 8 + 4 + 2;

    float diag = sqrtf(2 * 100 * 100);

    std::vector<float> result;
    result.resize(numChannelPoints * 2 + 2);

    Mach1Point3DCore channelPoints[numChannelPoints] =
        {
            Mach1Point3DCore(-100, 100, 100),
            Mach1Point3DCore(100, 100, 100),
            Mach1Point3DCore(-100, -100, 100),
            Mach1Point3DCore(100, -100, 100),

            Mach1Point3DCore(-100, 100, -100),
            Mach1Point3DCore(100, 100, -100),
            Mach1Point3DCore(-100, -100, -100),
            Mach1Point3DCore(100, -100, -100),

            Mach1Point3DCore(diag, 0, 0),
            Mach1Point3DCore(-diag, 0, 0),
            Mach1Point3DCore(0, diag, 0),
            Mach1Point3DCore(0, -diag, 0),

            Mach1Point3DCore(0, 0, diag),
            Mach1Point3DCore(0, 0, -diag),
        };

    spatialMultichannelAlgo(channelPoints, numChannelPoints, Yaw, Pitch, Roll, result.data());

    return result;
}

void Mach1DecodeCore::spatialAlgoSample_32(float Yaw, float Pitch, float Roll, float *result) {
    const int numChannelPoints = 32;

    Mach1Point3DCore channelPoints[numChannelPoints] =
        {
            Mach1Point3DCore(0.00000, 1.32048, 0.50689),
            Mach1Point3DCore(-0.74953, 1.19950, 0.00000),
            Mach1Point3DCore(0.00000, 1.32048, -0.50689),
            Mach1Point3DCore(0.74953, 1.19950, 0.00000),
            Mach1Point3DCore(0.00000, 0.74953, 1.19950),
            Mach1Point3DCore(-0.81928, 0.81928, 0.81128),
            Mach1Point3DCore(-1.32048, 0.50689, 0.00000),
            Mach1Point3DCore(-0.81928, 0.81928, -0.81128),
            Mach1Point3DCore(0.00000, 0.74953, -1.19950),
            Mach1Point3DCore(0.81928, 0.81928, -0.81128),
            Mach1Point3DCore(1.32048, 0.50689, 0.00000),
            Mach1Point3DCore(0.81928, 0.81928, 0.81128),
            Mach1Point3DCore(-0.50681, -0.00885, 1.32048),
            Mach1Point3DCore(-1.19950, -0.00000, 0.74953),
            Mach1Point3DCore(-1.21240, -0.00000, -0.72848),
            Mach1Point3DCore(-0.50681, 0.00885, -1.32048),
            Mach1Point3DCore(0.00000, -1.32048, 0.50689),
            Mach1Point3DCore(0.74953, -1.19950, 0.00000),
            Mach1Point3DCore(0.00000, -1.32048, -0.50689),
            Mach1Point3DCore(-0.74953, -1.19950, 0.00000),
            Mach1Point3DCore(0.00000, -0.74953, 1.19950),
            Mach1Point3DCore(0.81928, -0.81928, 0.81128),
            Mach1Point3DCore(1.32048, -0.50689, 0.00000),
            Mach1Point3DCore(0.81928, -0.81928, -0.81128),
            Mach1Point3DCore(0.00000, -0.74953, -1.19950),
            Mach1Point3DCore(-0.81928, -0.81928, -0.81128),
            Mach1Point3DCore(-1.32048, -0.50689, 0.00000),
            Mach1Point3DCore(-0.81928, -0.81928, 0.81128),
            Mach1Point3DCore(0.50681, -0.00885, 1.32048),
            Mach1Point3DCore(1.19950, 0.00000, 0.74953),
            Mach1Point3DCore(1.19950, 0.00000, -0.74953),
            Mach1Point3DCore(0.50681, 0.00885, -1.32048),
        };

    spatialMultichannelAlgo(channelPoints, numChannelPoints, Yaw, Pitch, Roll, result);
}

std::vector<float> Mach1DecodeCore::spatialAlgoSample_32(float Yaw, float Pitch, float Roll) {
    const int numChannelPoints = 32;

    std::vector<float> result;
    result.resize(numChannelPoints * 2 + 2);

    Mach1Point3DCore channelPoints[numChannelPoints] =
        {
            Mach1Point3DCore(0.00000, 1.32048, 0.50689),
            Mach1Point3DCore(-0.74953, 1.19950, 0.00000),
            Mach1Point3DCore(0.00000, 1.32048, -0.50689),
            Mach1Point3DCore(0.74953, 1.19950, 0.00000),
            Mach1Point3DCore(0.00000, 0.74953, 1.19950),
            Mach1Point3DCore(-0.81928, 0.81928, 0.81128),
            Mach1Point3DCore(-1.32048, 0.50689, 0.00000),
            Mach1Point3DCore(-0.81928, 0.81928, -0.81128),
            Mach1Point3DCore(0.00000, 0.74953, -1.19950),
            Mach1Point3DCore(0.81928, 0.81928, -0.81128),
            Mach1Point3DCore(1.32048, 0.50689, 0.00000),
            Mach1Point3DCore(0.81928, 0.81928, 0.81128),
            Mach1Point3DCore(-0.50681, -0.00885, 1.32048),
            Mach1Point3DCore(-1.19950, -0.00000, 0.74953),
            Mach1Point3DCore(-1.21240, -0.00000, -0.72848),
            Mach1Point3DCore(-0.50681, 0.00885, -1.32048),
            Mach1Point3DCore(0.00000, -1.32048, 0.50689),
            Mach1Point3DCore(0.74953, -1.19950, 0.00000),
            Mach1Point3DCore(0.00000, -1.32048, -0.50689),
            Mach1Point3DCore(-0.74953, -1.19950, 0.00000),
            Mach1Point3DCore(0.00000, -0.74953, 1.19950),
            Mach1Point3DCore(0.81928, -0.81928, 0.81128),
            Mach1Point3DCore(1.32048, -0.50689, 0.00000),
            Mach1Point3DCore(0.81928, -0.81928, -0.81128),
            Mach1Point3DCore(0.00000, -0.74953, -1.19950),
            Mach1Point3DCore(-0.81928, -0.81928, -0.81128),
            Mach1Point3DCore(-1.32048, -0.50689, 0.00000),
            Mach1Point3DCore(-0.81928, -0.81928, 0.81128),
            Mach1Point3DCore(0.50681, -0.00885, 1.32048),
            Mach1Point3DCore(1.19950, 0.00000, 0.74953),
            Mach1Point3DCore(1.19950, 0.00000, -0.74953),
            Mach1Point3DCore(0.50681, 0.00885, -1.32048),
        };

    spatialMultichannelAlgo(channelPoints, numChannelPoints, Yaw, Pitch, Roll, result.data());

    return result;
}

void Mach1DecodeCore::spatialAlgoSample_36(float Yaw, float Pitch, float Roll, float *result) {
    const int numChannelPoints = 36;

    Mach1Point3DCore channelPoints[numChannelPoints] =
        {
            Mach1Point3DCore(-0.43310, 0.71779, 1.13922),
            Mach1Point3DCore(1.13922, -0.43310, 0.71779),
            Mach1Point3DCore(0.43310, -0.71779, 1.13922),
            Mach1Point3DCore(0.71779, 1.13922, -0.43310),
            Mach1Point3DCore(1.13922, 0.43310, -0.71779),
            Mach1Point3DCore(-0.71779, 1.13922, 0.43310),
            Mach1Point3DCore(-1.13922, 0.43310, 0.71779),
            Mach1Point3DCore(-0.71779, -1.13922, -0.43310),
            Mach1Point3DCore(-1.13922, -0.43310, -0.71779),
            Mach1Point3DCore(0.71779, -1.13922, 0.43310),
            Mach1Point3DCore(0.43310, 0.71779, -1.13922),
            Mach1Point3DCore(-0.43310, -0.71779, -1.13922),
            Mach1Point3DCore(-0.34445, 0.88595, -1.04740),
            Mach1Point3DCore(-1.04740, -0.34445, 0.88595),
            Mach1Point3DCore(0.34445, -0.88595, -1.04740),
            Mach1Point3DCore(0.88595, -1.04740, -0.34445),
            Mach1Point3DCore(-1.04740, 0.34445, -0.88595),
            Mach1Point3DCore(-0.88595, -1.04740, 0.34445),
            Mach1Point3DCore(1.04740, 0.34445, 0.88595),
            Mach1Point3DCore(-0.88595, 1.04740, -0.34445),
            Mach1Point3DCore(1.04740, -0.34445, -0.88595),
            Mach1Point3DCore(0.88595, 1.04740, 0.34445),
            Mach1Point3DCore(0.34445, 0.88595, 1.04740),
            Mach1Point3DCore(-0.34445, -0.88595, 1.04740),
            Mach1Point3DCore(1.35378, -0.40488, -0.06298),
            Mach1Point3DCore(-0.06298, 1.35378, -0.40488),
            Mach1Point3DCore(-1.35378, 0.40488, -0.06298),
            Mach1Point3DCore(-0.40488, -0.06298, 1.35378),
            Mach1Point3DCore(-0.06298, -1.35378, 0.40488),
            Mach1Point3DCore(0.40488, -0.06298, -1.35378),
            Mach1Point3DCore(0.06298, -1.35378, -0.40488),
            Mach1Point3DCore(0.40488, 0.06298, 1.35378),
            Mach1Point3DCore(0.06298, 1.35378, 0.40488),
            Mach1Point3DCore(-0.40488, 0.06298, -1.35378),
            Mach1Point3DCore(-1.35378, -0.40488, 0.06298),
            Mach1Point3DCore(1.35378, 0.40488, 0.06298),
        };

    spatialMultichannelAlgo(channelPoints, numChannelPoints, Yaw, Pitch, Roll, result);
}

std::vector<float> Mach1DecodeCore::spatialAlgoSample_36(float Yaw, float Pitch, float Roll) {
    const int numChannelPoints = 36;

    std::vector<float> result;
    result.resize(numChannelPoints * 2 + 2);

    Mach1Point3DCore channelPoints[numChannelPoints] =
        {
            Mach1Point3DCore(-0.43310, 0.71779, 1.13922),
            Mach1Point3DCore(1.13922, -0.43310, 0.71779),
            Mach1Point3DCore(0.43310, -0.71779, 1.13922),
            Mach1Point3DCore(0.71779, 1.13922, -0.43310),
            Mach1Point3DCore(1.13922, 0.43310, -0.71779),
            Mach1Point3DCore(-0.71779, 1.13922, 0.43310),
            Mach1Point3DCore(-1.13922, 0.43310, 0.71779),
            Mach1Point3DCore(-0.71779, -1.13922, -0.43310),
            Mach1Point3DCore(-1.13922, -0.43310, -0.71779),
            Mach1Point3DCore(0.71779, -1.13922, 0.43310),
            Mach1Point3DCore(0.43310, 0.71779, -1.13922),
            Mach1Point3DCore(-0.43310, -0.71779, -1.13922),
            Mach1Point3DCore(-0.34445, 0.88595, -1.04740),
            Mach1Point3DCore(-1.04740, -0.34445, 0.88595),
            Mach1Point3DCore(0.34445, -0.88595, -1.04740),
            Mach1Point3DCore(0.88595, -1.04740, -0.34445),
            Mach1Point3DCore(-1.04740, 0.34445, -0.88595),
            Mach1Point3DCore(-0.88595, -1.04740, 0.34445),
            Mach1Point3DCore(1.04740, 0.34445, 0.88595),
            Mach1Point3DCore(-0.88595, 1.04740, -0.34445),
            Mach1Point3DCore(1.04740, -0.34445, -0.88595),
            Mach1Point3DCore(0.88595, 1.04740, 0.34445),
            Mach1Point3DCore(0.34445, 0.88595, 1.04740),
            Mach1Point3DCore(-0.34445, -0.88595, 1.04740),
            Mach1Point3DCore(1.35378, -0.40488, -0.06298),
            Mach1Point3DCore(-0.06298, 1.35378, -0.40488),
            Mach1Point3DCore(-1.35378, 0.40488, -0.06298),
            Mach1Point3DCore(-0.40488, -0.06298, 1.35378),
            Mach1Point3DCore(-0.06298, -1.35378, 0.40488),
            Mach1Point3DCore(0.40488, -0.06298, -1.35378),
            Mach1Point3DCore(0.06298, -1.35378, -0.40488),
            Mach1Point3DCore(0.40488, 0.06298, 1.35378),
            Mach1Point3DCore(0.06298, 1.35378, 0.40488),
            Mach1Point3DCore(-0.40488, 0.06298, -1.35378),
            Mach1Point3DCore(-1.35378, -0.40488, 0.06298),
            Mach1Point3DCore(1.35378, 0.40488, 0.06298),
        };

    spatialMultichannelAlgo(channelPoints, numChannelPoints, Yaw, Pitch, Roll, result.data());

    return result;
}

void Mach1DecodeCore::spatialAlgoSample_48(float Yaw, float Pitch, float Roll, float *result) {
    const int numChannelPoints = 48;

    Mach1Point3DCore channelPoints[numChannelPoints] =
        {
            Mach1Point3DCore(0.50006, 1.32018, -0.08763),
            Mach1Point3DCore(-0.50006, 1.32018, 0.08763),
            Mach1Point3DCore(-0.08763, 1.32018, -0.50006),
            Mach1Point3DCore(0.08763, 1.32018, 0.50006),
            Mach1Point3DCore(0.50006, -1.32018, 0.08763),
            Mach1Point3DCore(-0.50006, -1.32018, -0.08763),
            Mach1Point3DCore(-0.08763, -1.32018, 0.50006),
            Mach1Point3DCore(0.08763, -1.32018, -0.50006),
            Mach1Point3DCore(1.32018, -0.08763, 0.50006),
            Mach1Point3DCore(1.32018, 0.08763, -0.50006),
            Mach1Point3DCore(1.32018, -0.50006, -0.08763),
            Mach1Point3DCore(1.32018, 0.50006, 0.08763),
            Mach1Point3DCore(-1.32018, 0.08763, 0.50006),
            Mach1Point3DCore(-1.32018, -0.08763, -0.50006),
            Mach1Point3DCore(-1.32018, 0.50006, -0.08763),
            Mach1Point3DCore(-1.32018, -0.50006, 0.08763),
            Mach1Point3DCore(-0.08763, 0.50006, 1.32018),
            Mach1Point3DCore(0.08763, -0.50006, 1.32018),
            Mach1Point3DCore(-0.50006, -0.08763, 1.32018),
            Mach1Point3DCore(0.50006, 0.08763, 1.32018),
            Mach1Point3DCore(0.08763, 0.50006, -1.32018),
            Mach1Point3DCore(-0.08763, -0.50006, -1.32018),
            Mach1Point3DCore(0.50006, -0.08763, -1.32018),
            Mach1Point3DCore(-0.50006, 0.08763, -1.32018),
            Mach1Point3DCore(0.90487, 0.99978, 0.42693),
            Mach1Point3DCore(-0.90487, 0.99978, -0.42693),
            Mach1Point3DCore(0.42693, 0.99978, -0.90487),
            Mach1Point3DCore(-0.42693, 0.99978, 0.90487),
            Mach1Point3DCore(0.90487, -0.99978, -0.42693),
            Mach1Point3DCore(-0.90487, -0.99978, 0.42693),
            Mach1Point3DCore(0.42693, -0.99978, 0.90487),
            Mach1Point3DCore(-0.42693, -0.99978, -0.90487),
            Mach1Point3DCore(0.99978, 0.42693, 0.90487),
            Mach1Point3DCore(0.99978, -0.42693, -0.90487),
            Mach1Point3DCore(0.99978, -0.90487, 0.42693),
            Mach1Point3DCore(0.99978, 0.90487, -0.42693),
            Mach1Point3DCore(-0.99978, -0.42693, 0.90487),
            Mach1Point3DCore(-0.99978, 0.42693, -0.90487),
            Mach1Point3DCore(-0.99978, 0.90487, 0.42693),
            Mach1Point3DCore(-0.99978, -0.90487, -0.42693),
            Mach1Point3DCore(0.42693, 0.90487, 0.99978),
            Mach1Point3DCore(-0.42693, -0.90487, 0.99978),
            Mach1Point3DCore(-0.90487, 0.42693, 0.99978),
            Mach1Point3DCore(0.90487, -0.42693, 0.99978),
            Mach1Point3DCore(-0.42693, 0.90487, -0.99978),
            Mach1Point3DCore(0.42693, -0.90487, -0.99978),
            Mach1Point3DCore(0.90487, 0.42693, -0.99978),
            Mach1Point3DCore(-0.90487, -0.42693, -0.99978),
        };

    spatialMultichannelAlgo(channelPoints, numChannelPoints, Yaw, Pitch, Roll, result);
}

std::vector<float> Mach1DecodeCore::spatialAlgoSample_48(float Yaw, float Pitch, float Roll) {
    const int numChannelPoints = 48;

    std::vector<float> result;
    result.resize(numChannelPoints * 2 + 2);

    Mach1Point3DCore channelPoints[numChannelPoints] =
        {
            Mach1Point3DCore(0.50006, 1.32018, -0.08763),
            Mach1Point3DCore(-0.50006, 1.32018, 0.08763),
            Mach1Point3DCore(-0.08763, 1.32018, -0.50006),
            Mach1Point3DCore(0.08763, 1.32018, 0.50006),
            Mach1Point3DCore(0.50006, -1.32018, 0.08763),
            Mach1Point3DCore(-0.50006, -1.32018, -0.08763),
            Mach1Point3DCore(-0.08763, -1.32018, 0.50006),
            Mach1Point3DCore(0.08763, -1.32018, -0.50006),
            Mach1Point3DCore(1.32018, -0.08763, 0.50006),
            Mach1Point3DCore(1.32018, 0.08763, -0.50006),
            Mach1Point3DCore(1.32018, -0.50006, -0.08763),
            Mach1Point3DCore(1.32018, 0.50006, 0.08763),
            Mach1Point3DCore(-1.32018, 0.08763, 0.50006),
            Mach1Point3DCore(-1.32018, -0.08763, -0.50006),
            Mach1Point3DCore(-1.32018, 0.50006, -0.08763),
            Mach1Point3DCore(-1.32018, -0.50006, 0.08763),
            Mach1Point3DCore(-0.08763, 0.50006, 1.32018),
            Mach1Point3DCore(0.08763, -0.50006, 1.32018),
            Mach1Point3DCore(-0.50006, -0.08763, 1.32018),
            Mach1Point3DCore(0.50006, 0.08763, 1.32018),
            Mach1Point3DCore(0.08763, 0.50006, -1.32018),
            Mach1Point3DCore(-0.08763, -0.50006, -1.32018),
            Mach1Point3DCore(0.50006, -0.08763, -1.32018),
            Mach1Point3DCore(-0.50006, 0.08763, -1.32018),
            Mach1Point3DCore(0.90487, 0.99978, 0.42693),
            Mach1Point3DCore(-0.90487, 0.99978, -0.42693),
            Mach1Point3DCore(0.42693, 0.99978, -0.90487),
            Mach1Point3DCore(-0.42693, 0.99978, 0.90487),
            Mach1Point3DCore(0.90487, -0.99978, -0.42693),
            Mach1Point3DCore(-0.90487, -0.99978, 0.42693),
            Mach1Point3DCore(0.42693, -0.99978, 0.90487),
            Mach1Point3DCore(-0.42693, -0.99978, -0.90487),
            Mach1Point3DCore(0.99978, 0.42693, 0.90487),
            Mach1Point3DCore(0.99978, -0.42693, -0.90487),
            Mach1Point3DCore(0.99978, -0.90487, 0.42693),
            Mach1Point3DCore(0.99978, 0.90487, -0.42693),
            Mach1Point3DCore(-0.99978, -0.42693, 0.90487),
            Mach1Point3DCore(-0.99978, 0.42693, -0.90487),
            Mach1Point3DCore(-0.99978, 0.90487, 0.42693),
            Mach1Point3DCore(-0.99978, -0.90487, -0.42693),
            Mach1Point3DCore(0.42693, 0.90487, 0.99978),
            Mach1Point3DCore(-0.42693, -0.90487, 0.99978),
            Mach1Point3DCore(-0.90487, 0.42693, 0.99978),
            Mach1Point3DCore(0.90487, -0.42693, 0.99978),
            Mach1Point3DCore(-0.42693, 0.90487, -0.99978),
            Mach1Point3DCore(0.42693, -0.90487, -0.99978),
            Mach1Point3DCore(0.90487, 0.42693, -0.99978),
            Mach1Point3DCore(-0.90487, -0.42693, -0.99978),
        };

    spatialMultichannelAlgo(channelPoints, numChannelPoints, Yaw, Pitch, Roll, result.data());

    return result;
}

void Mach1DecodeCore::spatialAlgoSample_60(float Yaw, float Pitch, float Roll, float *result) {
    const int numChannelPoints = 60;

    Mach1Point3DCore channelPoints[numChannelPoints] =
        {
            Mach1Point3DCore(0.77221, -1.06624, -0.51714),
            Mach1Point3DCore(-0.51714, 0.77221, -1.06624),
            Mach1Point3DCore(-0.77221, 1.06624, -0.51714),
            Mach1Point3DCore(-1.06624, -0.51714, 0.77221),
            Mach1Point3DCore(-0.51714, -0.77221, 1.06624),
            Mach1Point3DCore(1.06624, -0.51714, -0.77221),
            Mach1Point3DCore(0.51714, -0.77221, -1.06624),
            Mach1Point3DCore(1.06624, 0.51714, 0.77221),
            Mach1Point3DCore(0.51714, 0.77221, 1.06624),
            Mach1Point3DCore(-1.06624, 0.51714, -0.77221),
            Mach1Point3DCore(-0.77221, -1.06624, 0.51714),
            Mach1Point3DCore(0.77221, 1.06624, 0.51714),
            Mach1Point3DCore(-1.00870, 0.99036, 0.04822),
            Mach1Point3DCore(0.04822, -1.00870, 0.99035),
            Mach1Point3DCore(1.00870, -0.99036, 0.04822),
            Mach1Point3DCore(0.99036, 0.04822, -1.00870),
            Mach1Point3DCore(0.04822, 1.00870, -0.99036),
            Mach1Point3DCore(-0.99036, 0.04822, 1.00870),
            Mach1Point3DCore(-0.04822, 1.00870, 0.99036),
            Mach1Point3DCore(-0.99036, -0.04822, -1.00870),
            Mach1Point3DCore(-0.04822, -1.00870, -0.99035),
            Mach1Point3DCore(0.99036, -0.04822, 1.00870),
            Mach1Point3DCore(1.00870, 0.99036, -0.04822),
            Mach1Point3DCore(-1.00870, -0.99036, -0.04822),
            Mach1Point3DCore(0.10898, 0.39071, -1.35502),
            Mach1Point3DCore(-1.35502, 0.10898, 0.39071),
            Mach1Point3DCore(-0.10898, -0.39071, -1.35502),
            Mach1Point3DCore(0.39071, -1.35502, 0.10898),
            Mach1Point3DCore(-1.35502, -0.10898, -0.39071),
            Mach1Point3DCore(-0.39071, -1.35502, -0.10898),
            Mach1Point3DCore(1.35502, -0.10898, 0.39071),
            Mach1Point3DCore(-0.39071, 1.35502, 0.10898),
            Mach1Point3DCore(1.35502, 0.10898, -0.39071),
            Mach1Point3DCore(0.39071, 1.35502, -0.10898),
            Mach1Point3DCore(-0.10898, 0.39071, 1.35502),
            Mach1Point3DCore(0.10898, -0.39071, 1.35502),
            Mach1Point3DCore(-1.10800, 0.63907, 0.60377),
            Mach1Point3DCore(0.60377, -1.10800, 0.63907),
            Mach1Point3DCore(1.10800, -0.63907, 0.60377),
            Mach1Point3DCore(0.63907, 0.60377, -1.10800),
            Mach1Point3DCore(0.60377, 1.10800, -0.63907),
            Mach1Point3DCore(-0.63907, 0.60377, 1.10800),
            Mach1Point3DCore(-0.60377, 1.10800, 0.63907),
            Mach1Point3DCore(-0.63907, -0.60377, -1.10800),
            Mach1Point3DCore(-0.60377, -1.10800, -0.63907),
            Mach1Point3DCore(0.63907, -0.60377, 1.10800),
            Mach1Point3DCore(1.10800, 0.63907, -0.60377),
            Mach1Point3DCore(-1.10800, -0.63907, -0.60377),
            Mach1Point3DCore(-1.31996, -0.47890, 0.17020),
            Mach1Point3DCore(0.17020, -1.31996, -0.47890),
            Mach1Point3DCore(1.31996, 0.47890, 0.17020),
            Mach1Point3DCore(-0.47890, 0.17020, -1.31996),
            Mach1Point3DCore(0.17020, 1.31996, 0.47890),
            Mach1Point3DCore(0.47890, 0.17020, 1.31996),
            Mach1Point3DCore(-0.17020, 1.31996, -0.47890),
            Mach1Point3DCore(0.47890, -0.17020, -1.31996),
            Mach1Point3DCore(-0.17020, -1.31996, 0.47890),
            Mach1Point3DCore(-0.47890, -0.17020, 1.31996),
            Mach1Point3DCore(1.31996, -0.47890, -0.17020),
            Mach1Point3DCore(-1.31996, 0.47890, -0.17020),
        };

    spatialMultichannelAlgo(channelPoints, numChannelPoints, Yaw, Pitch, Roll, result);
}

std::vector<float> Mach1DecodeCore::spatialAlgoSample_60(float Yaw, float Pitch, float Roll) {
    const int numChannelPoints = 60;

    std::vector<float> result;
    result.resize(numChannelPoints * 2 + 2);

    Mach1Point3DCore channelPoints[numChannelPoints] =
        {
            Mach1Point3DCore(0.77221, -1.06624, -0.51714),
            Mach1Point3DCore(-0.51714, 0.77221, -1.06624),
            Mach1Point3DCore(-0.77221, 1.06624, -0.51714),
            Mach1Point3DCore(-1.06624, -0.51714, 0.77221),
            Mach1Point3DCore(-0.51714, -0.77221, 1.06624),
            Mach1Point3DCore(1.06624, -0.51714, -0.77221),
            Mach1Point3DCore(0.51714, -0.77221, -1.06624),
            Mach1Point3DCore(1.06624, 0.51714, 0.77221),
            Mach1Point3DCore(0.51714, 0.77221, 1.06624),
            Mach1Point3DCore(-1.06624, 0.51714, -0.77221),
            Mach1Point3DCore(-0.77221, -1.06624, 0.51714),
            Mach1Point3DCore(0.77221, 1.06624, 0.51714),
            Mach1Point3DCore(-1.00870, 0.99036, 0.04822),
            Mach1Point3DCore(0.04822, -1.00870, 0.99035),
            Mach1Point3DCore(1.00870, -0.99036, 0.04822),
            Mach1Point3DCore(0.99036, 0.04822, -1.00870),
            Mach1Point3DCore(0.04822, 1.00870, -0.99036),
            Mach1Point3DCore(-0.99036, 0.04822, 1.00870),
            Mach1Point3DCore(-0.04822, 1.00870, 0.99036),
            Mach1Point3DCore(-0.99036, -0.04822, -1.00870),
            Mach1Point3DCore(-0.04822, -1.00870, -0.99035),
            Mach1Point3DCore(0.99036, -0.04822, 1.00870),
            Mach1Point3DCore(1.00870, 0.99036, -0.04822),
            Mach1Point3DCore(-1.00870, -0.99036, -0.04822),
            Mach1Point3DCore(0.10898, 0.39071, -1.35502),
            Mach1Point3DCore(-1.35502, 0.10898, 0.39071),
            Mach1Point3DCore(-0.10898, -0.39071, -1.35502),
            Mach1Point3DCore(0.39071, -1.35502, 0.10898),
            Mach1Point3DCore(-1.35502, -0.10898, -0.39071),
            Mach1Point3DCore(-0.39071, -1.35502, -0.10898),
            Mach1Point3DCore(1.35502, -0.10898, 0.39071),
            Mach1Point3DCore(-0.39071, 1.35502, 0.10898),
            Mach1Point3DCore(1.35502, 0.10898, -0.39071),
            Mach1Point3DCore(0.39071, 1.35502, -0.10898),
            Mach1Point3DCore(-0.10898, 0.39071, 1.35502),
            Mach1Point3DCore(0.10898, -0.39071, 1.35502),
            Mach1Point3DCore(-1.10800, 0.63907, 0.60377),
            Mach1Point3DCore(0.60377, -1.10800, 0.63907),
            Mach1Point3DCore(1.10800, -0.63907, 0.60377),
            Mach1Point3DCore(0.63907, 0.60377, -1.10800),
            Mach1Point3DCore(0.60377, 1.10800, -0.63907),
            Mach1Point3DCore(-0.63907, 0.60377, 1.10800),
            Mach1Point3DCore(-0.60377, 1.10800, 0.63907),
            Mach1Point3DCore(-0.63907, -0.60377, -1.10800),
            Mach1Point3DCore(-0.60377, -1.10800, -0.63907),
            Mach1Point3DCore(0.63907, -0.60377, 1.10800),
            Mach1Point3DCore(1.10800, 0.63907, -0.60377),
            Mach1Point3DCore(-1.10800, -0.63907, -0.60377),
            Mach1Point3DCore(-1.31996, -0.47890, 0.17020),
            Mach1Point3DCore(0.17020, -1.31996, -0.47890),
            Mach1Point3DCore(1.31996, 0.47890, 0.17020),
            Mach1Point3DCore(-0.47890, 0.17020, -1.31996),
            Mach1Point3DCore(0.17020, 1.31996, 0.47890),
            Mach1Point3DCore(0.47890, 0.17020, 1.31996),
            Mach1Point3DCore(-0.17020, 1.31996, -0.47890),
            Mach1Point3DCore(0.47890, -0.17020, -1.31996),
            Mach1Point3DCore(-0.17020, -1.31996, 0.47890),
            Mach1Point3DCore(-0.47890, -0.17020, 1.31996),
            Mach1Point3DCore(1.31996, -0.47890, -0.17020),
            Mach1Point3DCore(-1.31996, 0.47890, -0.17020),
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
        *P = _P;
        *R = _R;
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
        _P = *P;
        _R = *R;
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

Mach1Point3DCore Mach1DecodeCore::getCurrentAngle() {
    Mach1Point3DCore angle(currentYaw, currentPitch, currentRoll);
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
    case Mach1DecodeAlgoSpatial_8:
        return 8;
    case Mach1DecodeAlgoHorizon_4:
        return 4;
    case Mach1DecodeAlgoHorizonPairs:
        return 4;
    case Mach1DecodeAlgoSpatial_12:
        return 12;
    case Mach1DecodeAlgoSpatial_14:
        return 14;
    case Mach1DecodeAlgoSpatial_32:
        return 32;
    case Mach1DecodeAlgoSpatial_36:
        return 36;
    case Mach1DecodeAlgoSpatial_48:
        return 48;
    case Mach1DecodeAlgoSpatial_60:
        return 60;
    }
    return 0;
}

int Mach1DecodeCore::getFormatCoeffCount() {
    switch (algorithmType) {
    case Mach1DecodeAlgoSpatial_8:
        return (8 * 2) + 2;
    case Mach1DecodeAlgoHorizon_4:
        return (4 * 2) + 2;
    case Mach1DecodeAlgoHorizonPairs:
        return (4 * 2) + 2;
    case Mach1DecodeAlgoSpatial_12:
        return (12 * 2) + 2;
    case Mach1DecodeAlgoSpatial_14:
        return (14 * 2) + 2;
    case Mach1DecodeAlgoSpatial_32:
        return (32 * 2) + 2;
    case Mach1DecodeAlgoSpatial_36:
        return (36 * 2) + 2;
    case Mach1DecodeAlgoSpatial_48:
        return (48 * 2) + 2;
    case Mach1DecodeAlgoSpatial_60:
        return (60 * 2) + 2;
    }
    return 0;
}

void Mach1DecodeCore::setRotation(Mach1Point3DCore newRotationFromMinusOnetoOne) {
    rotation = newRotationFromMinusOnetoOne * 360.0;
}

void Mach1DecodeCore::setRotationDegrees(Mach1Point3DCore newRotationDegrees) {
    rotation = newRotationDegrees;
}

void Mach1DecodeCore::setRotationRadians(Mach1Point3DCore newRotationRadians) {
    rotation = newRotationRadians * (180.0 / PI);
}

void Mach1DecodeCore::setRotationQuat(Mach1Point4DCore newRotationQuat) {
    Mach1Point3DCore angles;

    // roll (x-axis rotation)
    float sinr_cosp = 2 * (newRotationQuat.w * newRotationQuat.x + newRotationQuat.y * newRotationQuat.z);
    float cosr_cosp = 1 - 2 * (newRotationQuat.x * newRotationQuat.x + newRotationQuat.y * newRotationQuat.y);
    angles.x = atan2f(sinr_cosp, cosr_cosp);

    // pitch (y-axis rotation)
    float sinp = 2 * (newRotationQuat.w * newRotationQuat.y - newRotationQuat.z * newRotationQuat.x);
    if (std::abs(sinp) >= 1) {
        angles.y = std::copysignf(PI / 2, sinp); // use 90 degrees if out of range
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
        case Mach1DecodeAlgoSpatial_8:
            res = spatialAlgo_8(Yaw, Pitch, Roll, bufferSize, sampleIndex);
            break;

        case Mach1DecodeAlgoHorizon_4:
            res = horizonAlgo_4(Yaw, Pitch, Roll, bufferSize, sampleIndex);
            break;

        case Mach1DecodeAlgoHorizonPairs:
            res = horizonPairsAlgo(Yaw, Pitch, Roll, bufferSize, sampleIndex);
            break;

        case Mach1DecodeAlgoSpatial_12:
            res = spatialAlgo_12(Yaw, Pitch, Roll, bufferSize, sampleIndex);
            break;

        case Mach1DecodeAlgoSpatial_14:
            res = spatialAlgo_14(Yaw, Pitch, Roll, bufferSize, sampleIndex);
            break;

        case Mach1DecodeAlgoSpatial_32:
            res = spatialAlgo_32(Yaw, Pitch, Roll, bufferSize, sampleIndex);
            break;

        case Mach1DecodeAlgoSpatial_36:
            res = spatialAlgo_36(Yaw, Pitch, Roll, bufferSize, sampleIndex);
            break;

        case Mach1DecodeAlgoSpatial_48:
            res = spatialAlgo_48(Yaw, Pitch, Roll, bufferSize, sampleIndex);
            break;

        case Mach1DecodeAlgoSpatial_60:
            res = spatialAlgo_60(Yaw, Pitch, Roll, bufferSize, sampleIndex);
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
        case Mach1DecodeAlgoSpatial_8:
            spatialAlgo_8(Yaw, Pitch, Roll, result, bufferSize, sampleIndex);
            break;

        case Mach1DecodeAlgoHorizon_4:
            horizonAlgo_4(Yaw, Pitch, Roll, result, bufferSize, sampleIndex);
            break;

        case Mach1DecodeAlgoHorizonPairs:
            horizonPairsAlgo(Yaw, Pitch, Roll, result, bufferSize, sampleIndex);
            break;

        case Mach1DecodeAlgoSpatial_12:
            spatialAlgo_12(Yaw, Pitch, Roll, result, bufferSize, sampleIndex);
            break;

        case Mach1DecodeAlgoSpatial_14:
            spatialAlgo_14(Yaw, Pitch, Roll, result, bufferSize, sampleIndex);
            break;

        case Mach1DecodeAlgoSpatial_32:
            spatialAlgo_32(Yaw, Pitch, Roll, result, bufferSize, sampleIndex);
            break;

        case Mach1DecodeAlgoSpatial_36:
            spatialAlgo_36(Yaw, Pitch, Roll, result, bufferSize, sampleIndex);
            break;

        case Mach1DecodeAlgoSpatial_48:
            spatialAlgo_48(Yaw, Pitch, Roll, result, bufferSize, sampleIndex);
            break;

        case Mach1DecodeAlgoSpatial_60:
            spatialAlgo_60(Yaw, Pitch, Roll, result, bufferSize, sampleIndex);
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

// The following functions are deprecated as of now
//--------------------------------------------------
//  Begin function that has to be called
//  before per-sample coefficient calculation.
//

void Mach1DecodeCore::beginBuffer() {
    previousYaw = currentYaw;
    previousPitch = currentPitch;
    previousRoll = currentRoll;

    updateAngles();
}

//  End function that has to be called
//  after per-sample coefficient calculation.

void Mach1DecodeCore::endBuffer() {
    //  ;)
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
            for (int i = 0; i < 18; i++) {
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

std::vector<float> Mach1DecodeCore::horizonAlgo_4(float Yaw, float Pitch, float Roll,
                                                  int bufferSize, int sampleIndex) {

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

    float coefficients[4];
    coefficients[0] = 1.f - std::min(1.f, std::min(360.f - Yaw, Yaw) / 90.f);
    coefficients[1] = 1.f - std::min(1.f, std::abs(90.f - Yaw) / 90.f);
    coefficients[2] = 1.f - std::min(1.f, std::abs(180.f - Yaw) / 90.f);
    coefficients[3] = 1.f - std::min(1.f, std::abs(270.f - Yaw) / 90.f);

    std::vector<float> result;
    result.resize(8);
    result[0] = coefficients[0]; // 1 left
    result[1] = coefficients[3]; //   right
    result[2] = coefficients[1]; // 2 left
    result[3] = coefficients[0]; //   right
    result[4] = coefficients[3]; // 3 left
    result[5] = coefficients[2]; //   right
    result[6] = coefficients[2]; // 4 left
    result[7] = coefficients[1]; //   right

    result.push_back(1.0); // static stereo L
    result.push_back(1.0); // static stereo R
    return result;
}

void Mach1DecodeCore::horizonAlgo_4(float Yaw, float Pitch, float Roll, float *result,
                                    int bufferSize, int sampleIndex) {
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

    float coefficients[4];
    coefficients[0] = 1.f - std::min(1.f, std::min(360.f - Yaw, Yaw) / 90.f);
    coefficients[1] = 1.f - std::min(1.f, std::abs(90.f - Yaw) / 90.f);
    coefficients[2] = 1.f - std::min(1.f, std::abs(180.f - Yaw) / 90.f);
    coefficients[3] = 1.f - std::min(1.f, std::abs(270.f - Yaw) / 90.f);

    result[0] = coefficients[0]; // 1 left
    result[1] = coefficients[3]; //   right
    result[2] = coefficients[1]; // 2 left
    result[3] = coefficients[0]; //   right
    result[4] = coefficients[3]; // 3 left
    result[5] = coefficients[2]; //   right
    result[6] = coefficients[2]; // 4 left
    result[7] = coefficients[1]; //   right

    result[8] = 1.0; // static stereo L
    result[9] = 1.0; // static stereo R
}

// ------------------------------------------------------------------

//
//  Four pairs audio format.
//
//  Order of input angles:
//  Y = Yaw in degrees
//  P = Pitch in degrees
//  R = Roll in degrees
//

std::vector<float> Mach1DecodeCore::horizonPairsAlgo(float Yaw, float Pitch, float Roll,
                                                     int bufferSize, int sampleIndex) {

    return processSample(&Mach1DecodeCore::horizonPairsAlgoSample, Yaw, Pitch, Roll, bufferSize, sampleIndex);
}

void Mach1DecodeCore::horizonPairsAlgo(float Yaw, float Pitch, float Roll, float *result,
                                       int bufferSize, int sampleIndex) {

    return processSample(&Mach1DecodeCore::horizonPairsAlgoSample, Yaw, Pitch, Roll, result, bufferSize, sampleIndex);
}

// ------------------------------------------------------------------

//
//  Eight channel audio format (isotropic version).
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
//  Additional channel audio formats (isotropic version).
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

std::vector<float> Mach1DecodeCore::spatialAlgo_32(float Yaw, float Pitch, float Roll,
                                                   int bufferSize, int sampleIndex) {

    return processSample(&Mach1DecodeCore::spatialAlgoSample_32, Yaw, Pitch, Roll, bufferSize, sampleIndex);
}

void Mach1DecodeCore::spatialAlgo_32(float Yaw, float Pitch, float Roll, float *result,
                                     int bufferSize, int sampleIndex) {

    return processSample(&Mach1DecodeCore::spatialAlgoSample_32, Yaw, Pitch, Roll, result, bufferSize, sampleIndex);
}

// ------------------------------------------------------------------

std::vector<float> Mach1DecodeCore::spatialAlgo_36(float Yaw, float Pitch, float Roll,
                                                   int bufferSize, int sampleIndex) {

    return processSample(&Mach1DecodeCore::spatialAlgoSample_36, Yaw, Pitch, Roll, bufferSize, sampleIndex);
}

void Mach1DecodeCore::spatialAlgo_36(float Yaw, float Pitch, float Roll, float *result,
                                     int bufferSize, int sampleIndex) {

    return processSample(&Mach1DecodeCore::spatialAlgoSample_36, Yaw, Pitch, Roll, result, bufferSize, sampleIndex);
}

// ------------------------------------------------------------------

std::vector<float> Mach1DecodeCore::spatialAlgo_48(float Yaw, float Pitch, float Roll,
                                                   int bufferSize, int sampleIndex) {

    return processSample(&Mach1DecodeCore::spatialAlgoSample_48, Yaw, Pitch, Roll, bufferSize, sampleIndex);
}

void Mach1DecodeCore::spatialAlgo_48(float Yaw, float Pitch, float Roll, float *result,
                                     int bufferSize, int sampleIndex) {

    return processSample(&Mach1DecodeCore::spatialAlgoSample_48, Yaw, Pitch, Roll, result, bufferSize, sampleIndex);
}

// ------------------------------------------------------------------

std::vector<float> Mach1DecodeCore::spatialAlgo_60(float Yaw, float Pitch, float Roll,
                                                   int bufferSize, int sampleIndex) {

    return processSample(&Mach1DecodeCore::spatialAlgoSample_60, Yaw, Pitch, Roll, bufferSize, sampleIndex);
}

void Mach1DecodeCore::spatialAlgo_60(float Yaw, float Pitch, float Roll, float *result,
                                     int bufferSize, int sampleIndex) {

    return processSample(&Mach1DecodeCore::spatialAlgoSample_60, Yaw, Pitch, Roll, result, bufferSize, sampleIndex);
}

// ------------------------------------------------------------------