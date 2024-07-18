//  Mach1 Spatial SDK
//  Copyright © 2017-2020 Mach1. All rights reserved.

/*
DISCLAIMER:
This header file is not an example of use but an decoder that will require periodic
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

#pragma once

#include <vector>

#include "Mach1DecodeCAPI.h"
#include "Mach1Point3DCore.h"
#include "Mach1Point4DCore.h"

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
    typedef std::vector<float> (Mach1DecodeCore::*functionAlgoSample)(float Yaw, float Pitch, float Roll);
    typedef void (Mach1DecodeCore::*functionAlgoSampleHP)(float Yaw, float Pitch, float Roll, float *result);

    std::vector<float> processSample(functionAlgoSample funcAlgoSample, float Yaw, float Pitch, float Roll, int bufferSize = 0, int sampleIndex = 0);
    void processSample(functionAlgoSampleHP funcAlgoSampleHP, float Yaw, float Pitch, float Roll, float *result, int bufferSize = 0, int sampleIndex = 0);

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

    bool smoothAngles;

    Mach1PlatformType platformType;

    // Selected algo type

    Mach1DecodeAlgoType algorithmType;

    inline float _dot(const Mach1Point3DCore &p1, const Mach1Point3DCore &p2) const;

    bool linePlaneIntersection(Mach1Point3DCore &contact, Mach1Point3DCore ray, Mach1Point3DCore rayOrigin, Mach1Point3DCore normal, Mach1Point3DCore coord);

    void horizonPairsAlgoSample(float Yaw, float Pitch, float Roll, float *result);
    std::vector<float> horizonPairsAlgoSample(float Yaw, float Pitch, float Roll);

    void spatialMultichannelAlgo(Mach1Point3DCore *channelPoints, int numChannelPoints, float Yaw, float Pitch, float Roll, float *result);

    void spatialAlgoSample_8(float Yaw, float Pitch, float Roll, float *result);
    std::vector<float> spatialAlgoSample_8(float Yaw, float Pitch, float Roll);

    void spatialAlgoSample_12(float Yaw, float Pitch, float Roll, float *result);
    std::vector<float> spatialAlgoSample_12(float Yaw, float Pitch, float Roll);

    void spatialAlgoSample_14(float Yaw, float Pitch, float Roll, float *result);
    std::vector<float> spatialAlgoSample_14(float Yaw, float Pitch, float Roll);

    // log
    std::vector<std::string> strLog;
    void addToLog(std::string str, int maxCount = 100);

    Mach1Point3DCore rotation;

  public:
    char *getLog();

    float filterSpeed;

    // Angular settings functions
    static void convertAnglesToMach1(Mach1PlatformType platformType, float *Y, float *P, float *R);
    static void convertAnglesToPlatform(Mach1PlatformType platformType, float *Y, float *P, float *R);

    // static void convertQuaternionToMach1(Mach1PlatformType platformType, float* X, float* Y, float* Z, float* W);
    // static void convertQuaternionToPlatform(Mach1PlatformType platformType, float* Y, float* P, float* R);

    Mach1Point3DCore getCurrentAngle();

    Mach1DecodeCore();

    void setPlatformType(Mach1PlatformType type);
    Mach1PlatformType getPlatformType();

    int getFormatChannelCount();
    int getFormatCoeffCount();

    void setRotation(Mach1Point3DCore newRotationFromMinusOnetoOne);
    void setRotationDegrees(Mach1Point3DCore newRotationDegrees);
    void setRotationRadians(Mach1Point3DCore newRotationRadians);
    void setRotationQuat(Mach1Point4DCore newRotationQuat);

    void setFilterSpeed(float filterSpeed);

    long getCurrentTime();
    long getLastCalculationTime();

    // Set the algorithm type to use when decoding

    void setDecodeAlgoType(Mach1DecodeAlgoType newAlgorithmType);
    Mach1DecodeAlgoType getDecodeAlgoType();

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

    // The following functions are deprecated as of now

    //--------------------------------------------------

    //
    //  Four channel audio format
    //
    //  Order of input angles:
    //  Y = Yaw in degrees
    //  P = Pitch in degrees
    //  R = Roll in degrees
    //

    std::vector<float> horizonAlgo_4(float Yaw, float Pitch, float Roll, int bufferSize = 0, int sampleIndex = 0);

    void horizonAlgo_4(float Yaw, float Pitch, float Roll, float *result, int bufferSize = 0, int sampleIndex = 0);

    // ------------------------------------------------------------------

    //
    //  Four pairs audio format.
    //
    //  Order of input angles:
    //  Y = Yaw in degrees
    //  P = Pitch in degrees
    //  R = Roll in degrees
    //

    std::vector<float> horizonPairsAlgo(float Yaw, float Pitch, float Roll, int bufferSize = 0, int sampleIndex = 0);

    void horizonPairsAlgo(float Yaw, float Pitch, float Roll, float *result, int bufferSize = 0, int sampleIndex = 0);

    // ------------------------------------------------------------------

    //
    //  Eight channel audio format (isotropic version).
    //
    //  Order of input angles:
    //  Y = Yaw in degrees
    //  P = Pitch in degrees
    //  R = Roll in degrees
    //

    std::vector<float> spatialAlgo_8(float Yaw, float Pitch, float Roll, int bufferSize = 0, int sampleIndex = 0);

    void spatialAlgo_8(float Yaw, float Pitch, float Roll, float *result, int bufferSize, int sampleIndex);

    // ------------------------------------------------------------------

    //
    //  Additional channel audio formats (isotropic version).
    //
    //  Order of input angles:
    //  Y = Yaw in degrees
    //  P = Pitch in degrees
    //  R = Roll in degrees
    //

    std::vector<float> spatialAlgo_12(float Yaw, float Pitch, float Roll, int bufferSize = 0, int sampleIndex = 0);
    void spatialAlgo_12(float Yaw, float Pitch, float Roll, float *result, int bufferSize, int sampleIndex);

    // ------------------------------------------------------------------

    std::vector<float> spatialAlgo_14(float Yaw, float Pitch, float Roll, int bufferSize = 0, int sampleIndex = 0);
    void spatialAlgo_14(float Yaw, float Pitch, float Roll, float *result, int bufferSize, int sampleIndex);

    // ------------------------------------------------------------------
};