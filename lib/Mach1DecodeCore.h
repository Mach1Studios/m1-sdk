//  Mach1 SDK
//  Copyright © 2018 Mach1. All rights reserved.
//
//  Header file

/*
DISCLAIMER:
This header file is not an example of use but an decoder that will require periodic
updates and should not be integrated in sections but remain as an update-able factored file.
*/

#pragma once

#ifndef Mach1DecodeCore_h
#define Mach1DecodeCore_h

#include <vector>
#include <cmath>
#include <chrono>
#include <string>
#include <algorithm>

#include "Mach1Point3DCore.h"
#include "Mach1Point4DCore.h"
#include "Mach1DecodeCAPI.h"

#ifndef SWIG
using namespace std::chrono;
#endif

#ifndef PI
#define PI       3.14159265358979323846f
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
    

	inline float _dot(const Mach1Point3DCore& p1, const Mach1Point3DCore& p2) const;

	bool linePlaneIntersection(Mach1Point3DCore& contact, Mach1Point3DCore ray, Mach1Point3DCore rayOrigin, Mach1Point3DCore normal, Mach1Point3DCore coord);

	void horizonPairsAlgoSample(float Yaw, float Pitch, float Roll, float *result);
	std::vector<float> horizonPairsAlgoSample(float Yaw, float Pitch, float Roll);

	void spatialAltAlgoSample(float Yaw, float Pitch, float Roll, float *result);
	std::vector<float> spatialAltAlgoSample(float Yaw, float Pitch, float Roll);

	void spatialMultichannelAlgo(Mach1Point3DCore* points, int countPoints, float Yaw, float Pitch, float Roll, float *result);

	void spatialAlgoSample(float Yaw, float Pitch, float Roll, float *result);
	std::vector<float> spatialAlgoSample(float Yaw, float Pitch, float Roll);

	void spatialPlusAlgoSample(float Yaw, float Pitch, float Roll, float *result);
	std::vector<float> spatialPlusAlgoSample(float Yaw, float Pitch, float Roll);

	void spatialPlusPlusAlgoSample(float Yaw, float Pitch, float Roll, float *result);
	std::vector<float> spatialPlusPlusAlgoSample(float Yaw, float Pitch, float Roll);

	void spatialExtAlgoSample(float Yaw, float Pitch, float Roll, float *result);
	std::vector<float> spatialExtAlgoSample(float Yaw, float Pitch, float Roll);

	void spatialExtPlusAlgoSample(float Yaw, float Pitch, float Roll, float *result);
	std::vector<float> spatialExtPlusAlgoSample(float Yaw, float Pitch, float Roll);

	// log
	std::vector<std::string> strLog;
	void addToLog(std::string str, int maxCount = 100);

public:
    
 	char* getLog();

    float filterSpeed;

	// Angular settings functions
	static void convertAnglesToMach1(Mach1PlatformType platformType, float* Y, float* P, float* R);
	static void convertAnglesToPlatform(Mach1PlatformType platformType, float* Y, float* P, float* R);

	Mach1Point3DCore getCurrentAngle();

    Mach1DecodeCore();
    
	void setPlatformType(Mach1PlatformType type);
	Mach1PlatformType getPlatformType();

	int getOutputChannelsCount();

    void setFilterSpeed(float filterSpeed);

    void beginBuffer();
    void endBuffer();

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

    // Decode using the current algorithm type in a more efficient way

    void decode(float Yaw, float Pitch, float Roll, float *result, int bufferSize = 0, int sampleIndex = 0);

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
    
    std::vector<float> horizonAlgo(float Yaw, float Pitch, float Roll, int bufferSize = 0, int sampleIndex = 0);

    void horizonAlgo(float Yaw, float Pitch, float Roll, float *result, int bufferSize = 0, int sampleIndex = 0);

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
    
    std::vector<float> spatialAlgo(float Yaw, float Pitch, float Roll, int bufferSize = 0, int sampleIndex = 0);
    
    void spatialAlgo(float Yaw, float Pitch, float Roll, float *result, int bufferSize, int sampleIndex);
    
// ------------------------------------------------------------------

//
//  Additional channel audio formats (isotropic version).
//
//  Order of input angles:
//  Y = Yaw in degrees
//  P = Pitch in degrees
//  R = Roll in degrees
//

	std::vector<float> spatialPlusAlgo(float Yaw, float Pitch, float Roll, int bufferSize = 0, int sampleIndex = 0);

	void spatialPlusAlgo(float Yaw, float Pitch, float Roll, float *result, int bufferSize, int sampleIndex);

	// ------------------------------------------------------------------

	std::vector<float> spatialPlusPlusAlgo(float Yaw, float Pitch, float Roll, int bufferSize = 0, int sampleIndex = 0);

	void spatialPlusPlusAlgo(float Yaw, float Pitch, float Roll, float *result, int bufferSize, int sampleIndex);

	// ------------------------------------------------------------------

    std::vector<float> spatialExtAlgo(float Yaw, float Pitch, float Roll, int bufferSize = 0, int sampleIndex = 0);

    void spatialExtAlgo(float Yaw, float Pitch, float Roll, float *result, int bufferSize, int sampleIndex);

    // ------------------------------------------------------------------

    std::vector<float> spatialExtPlusAlgo(float Yaw, float Pitch, float Roll, int bufferSize = 0, int sampleIndex = 0);

    void spatialExtPlusAlgo(float Yaw, float Pitch, float Roll, float *result, int bufferSize, int sampleIndex);

    // ------------------------------------------------------------------

	//
    //  Eight channel audio format.
    //
    //  Order of input angles:
    //  Y = Yaw in degrees
    //  P = Pitch in degrees
    //  R = Roll in degrees
    //
    
    std::vector<float> spatialAltAlgo(float Yaw, float Pitch, float Roll, int bufferSize = 0, int sampleIndex = 0);

    void spatialAltAlgo(float Yaw, float Pitch, float Roll, float *result, int bufferSize = 0, int sampleIndex = 0);

    // ------------------------------------------------------------------
    
    //
    //  Eight pairs audio format.
    //
    //  Order of input angles:
    //  Y = Yaw in degrees
    //  P = Pitch in degrees
    //  R = Roll in degrees
    //
    
    std::vector<float> spatialPairsAlgo(float Yaw, float Pitch, float Roll, int bufferSize = 0, int sampleIndex = 0);

    void spatialPairsAlgo(float Yaw, float Pitch, float Roll, float *result, int bufferSize = 0, int sampleIndex = 0);

    // ------------------------------------------------------------------
    
};

#endif /* Mach1DecodeCore_h */
