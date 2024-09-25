//  Mach1 Spatial SDK
//  Copyright © 2017 Mach1. All rights reserved.

/*
DISCLAIMER:
This header file is not an example of use but an decoder that will require periodic
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

Mach1DecodeCore normalizes all input ranges to an unsigned "0 to 1" range for Yaw, Pitch and Roll.
 */

#pragma once

#include <chrono>
#include <string>
#include <vector>

#include "Mach1DecodeCAPI.h"
#include "Mach1Point3D.h"
#include "Mach1Point4D.h"

#ifndef SWIG
using namespace std::chrono;
#endif

#ifndef PI
#    define PI 3.14159265358979323846f
#endif

//////////////

class Mach1DecodeCore {

  public:
    static float mDegToRad(float degrees);
    static float mmap(float value, float inputMin, float inputMax, float outputMin, float outputMax, bool clamp = false);
    static float clamp(float a, float min, float max);

  private:
    typedef std::vector<float> (Mach1DecodeCore::*processSampleForMultichannel)(float Yaw, float Pitch, float Roll);
    typedef void (Mach1DecodeCore::*processSampleForStereoPairs)(float Yaw, float Pitch, float Roll, float *result);

    std::vector<float> processSample(processSampleForMultichannel _processSampleForMultichannel, float Yaw, float Pitch, float Roll, int bufferSize = 0, int sampleIndex = 0);
    void processSample(processSampleForStereoPairs _processSampleForStereoPairs, float Yaw, float Pitch, float Roll, float *result, int bufferSize = 0, int sampleIndex = 0);

    milliseconds ms;

    // Math utilities
    static float alignAngle(float a, float min = -180, float max = 180);
    static float lerp(float x1, float x2, float t);
    float radialDistance(float angle1, float angle2);
    float targetDirectionMultiplier(float angleCurrent, float angleTarget);

    // Filter features
    // Envelope follower feature is defined here, in updateAngles()

    void updateAngles();

    float currentYaw, currentPitch, currentRoll;
    float targetYaw, targetPitch, targetRoll;
    float previousYaw, previousPitch, previousRoll;

    long timeLastUpdate;
    long timeLastCalculation;

    Mach1PlatformType platformType;

    // Selected algo type

    Mach1DecodeMode decodeMode;

    inline float _dot(const Mach1Point3D &p1, const Mach1Point3D &p2) const;

    bool linePlaneIntersection(Mach1Point3D &contact, Mach1Point3D ray, Mach1Point3D rayOrigin, Mach1Point3D normal, Mach1Point3D coord);

    void spatialMultichannelAlgo(Mach1Point3D *channelPoints, int numChannelPoints, float Yaw, float Pitch, float Roll, float *result);
    
    void spatialAlgo_4(float Yaw, float Pitch, float Roll, float *result);
    std::vector<float> spatialAlgo_4(float Yaw, float Pitch, float Roll);

    void spatialAlgo_8(float Yaw, float Pitch, float Roll, float *result);
    std::vector<float> spatialAlgo_8(float Yaw, float Pitch, float Roll);

    void spatialAlgo_12(float Yaw, float Pitch, float Roll, float *result);
    std::vector<float> spatialAlgo_12(float Yaw, float Pitch, float Roll);

    void spatialAlgo_14(float Yaw, float Pitch, float Roll, float *result);
    std::vector<float> spatialAlgo_14(float Yaw, float Pitch, float Roll);

    // log
    std::vector<std::string> strLog;
    void addToLog(std::string str, int maxCount = 100);

    Mach1Point3D rotation;

  public:
    char *getLog();
    float filterSpeed;

    // Angular settings functions
    static void convertAnglesToMach1(Mach1PlatformType platformType, float *Y, float *P, float *R);
    static void convertAnglesToPlatform(Mach1PlatformType platformType, float *Y, float *P, float *R);
    // static void convertQuaternionToMach1(Mach1PlatformType platformType, float* X, float* Y, float* Z, float* W);
    // static void convertQuaternionToPlatform(Mach1PlatformType platformType, float* Y, float* P, float* R);

    Mach1Point3D getCurrentAngle();

    Mach1DecodeCore();

    void setPlatformType(Mach1PlatformType type);
    Mach1PlatformType getPlatformType();

    int getFormatChannelCount();
    int getFormatCoeffCount();

    void setRotation(Mach1Point3D newRotationFromMinusOnetoOne);
    void setRotationDegrees(Mach1Point3D newRotationDegrees);
    void setRotationRadians(Mach1Point3D newRotationRadians);
    void setRotationQuat(Mach1Point4D newRotationQuat);

    void setFilterSpeed(float filterSpeed);

    long getCurrentTime();
    long getLastCalculationTime();

    // Set the algorithm type to use when decoding

    void setDecodeMode(Mach1DecodeMode mode);
    Mach1DecodeMode getDecodeMode();

    // Decode using the current algorithm type

    //  Order of input angles:
    //  Y = Yaw in degrees
    //  P = Pitch in degrees
    //  R = Roll in degrees

    std::vector<float> decode(float Yaw, float Pitch, float Roll, int bufferSize = 0, int sampleIndex = 0);
    std::vector<float> decodeCoeffs(int bufferSize = 0, int sampleIndex = 0);
    std::vector<float> decodePannedCoeffs(int bufferSize = 0, int sampleIndex = 0, bool applyPanLaw = true);

    // Decode using the current algorithm type in a more efficient way

    void decode(float Yaw, float Pitch, float Roll, float *result, int bufferSize = 0, int sampleIndex = 0);
    void decodeCoeffs(float *result, int bufferSize = 0, int sampleIndex = 0);
    void decodePannedCoeffs(float *result, int bufferSize = 0, int sampleIndex = 0, bool applyPanLaw = true);
    void decodeCoeffsUsingTranscodeMatrix(void *M1obj, float *matrix, int channels, float *result, int bufferSize = 0, int sampleIndex = 0);

};
