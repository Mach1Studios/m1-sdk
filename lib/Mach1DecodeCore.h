//  Mach1 SDK
//  Copyright © 2018 Mach1. All rights reserved.
//
//  Header file

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
    

	inline float _dot(const Mach1Point3DCore& p1, const Mach1Point3DCore& p2) const {
		return p1.x * p2.x + p1.y * p2.y + p1.z * p2.z;
	}

	bool linePlaneIntersection(Mach1Point3DCore& contact, Mach1Point3DCore ray, Mach1Point3DCore rayOrigin, Mach1Point3DCore normal, Mach1Point3DCore coord)
	{
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

    void horizonPairsAlgoSample(float Yaw, float Pitch, float Roll, float *result) {
        //Orientation input safety clamps/alignment
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
    

    
    std::vector<float> horizonPairsAlgoSample(float Yaw, float Pitch, float Roll) {
        //Orientation input safety clamps/alignment
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
    
    void spatialAltAlgoSample(float Yaw, float Pitch, float Roll, float *result) {
        //Orientation input safety clamps/alignment
        Pitch = alignAngle(Pitch, -180, 180);
        Pitch = clamp(Pitch, -90, 90); // -90, 90
        
        Yaw = alignAngle(Yaw, 0, 360);
        
        Roll = alignAngle(Roll, -180, 180);
        Roll = clamp(Roll, -90, 90); // -90, 90
        
        float coefficients[8];
        coefficients[0] = 1.f - std::min(1.f, std::min((float)360.f - Yaw, Yaw) / 90.f);
        coefficients[1] = 1.f - std::min(1.f, std::abs((float)90.f - Yaw) / 90.f);
        coefficients[2] = 1.f - std::min(1.f, std::abs((float)180.f - Yaw) / 90.f);
        coefficients[3] = 1.f - std::min(1.f, std::abs((float)270.f - Yaw) / 90.f);
        
        float tiltAngle = mmap(Roll, -90.f, 90.f, 0.f, 1.f, true);
        //Use Equal Power if engine requires
        /*
         float tiltHigh = cos(tiltAngle * (0.5 * PI));
         float tiltLow = cos((1.f0 - tiltAngle) * (0.5 * PI));
         */
        float tiltHigh = tiltAngle;
        float tiltLow = 1.f - tiltHigh;
        
        //ISSUE//
        //Able to kill stereo by making both pitch and tilt at max or min values together without proper clamps
        
        result[0] = coefficients[0] * tiltHigh * 2.0f; // 1 left
        result[1] = coefficients[3] * tiltHigh * 2.0f; //   right
        result[2] = coefficients[1] * tiltLow * 2.0f; // 2 left
        result[3] = coefficients[0] * tiltLow * 2.0f; //   right
        result[4] = coefficients[3] * tiltLow * 2.0f; // 3 left
        result[5] = coefficients[2] * tiltLow * 2.0f; //   right
        result[6] = coefficients[2] * tiltHigh * 2.0f; // 4 left
        result[7] = coefficients[1] * tiltHigh * 2.0f; //   right
        
        result[0 + 8] = coefficients[0] * tiltLow * 2.0f; // 1 left
        result[1 + 8] = coefficients[3] * tiltLow * 2.0f; //   right
        result[2 + 8] = coefficients[1] * tiltHigh * 2.0f; // 2 left
        result[3 + 8] = coefficients[0] * tiltHigh * 2.0f; //   right
        result[4 + 8] = coefficients[3] * tiltHigh * 2.0f; // 3 left
        result[5 + 8] = coefficients[2] * tiltHigh * 2.0f; //   right
        result[6 + 8] = coefficients[2] * tiltLow * 2.0f; // 4 left
        result[7 + 8] = coefficients[1] * tiltLow * 2.0f; //   right
        
        float pitchAngle = mmap(Pitch, 90.f, -90.f, 0., 1.f, true);
        //Use Equal Power if engine requires
        /*
         float pitchHigherHalf = cos(pitchAngle * (0.5*PI));
         float pitchLowerHalf = cos((1.f0 - pitchAngle) * (0.5*PI));
         */
        float pitchHigherHalf = pitchAngle;
        float pitchLowerHalf = 1.f - pitchHigherHalf;
        
        for (int i = 0; i < 8; i++) {
            result[i] *= pitchLowerHalf;
            result[i + 8] *= pitchHigherHalf;
        }
        
        result[8 + 8] = 1.0f; // static stereo L
        result[9 + 8] = 1.0f; // static stereo R
        
    }
    
    std::vector<float> spatialAltAlgoSample(float Yaw, float Pitch, float Roll) {
        //Orientation input safety clamps/alignment
        Pitch = alignAngle(Pitch, -180, 180);
        Pitch = clamp(Pitch, -90, 90); // -90, 90
        
        Yaw = alignAngle(Yaw, 0, 360);
        
        Roll = alignAngle(Roll, -180, 180);
        Roll = clamp(Roll, -90, 90); // -90, 90
        
        float coefficients[8];
        coefficients[0] = 1.f - std::min(1.f, std::min((float)360.f - Yaw, Yaw) / 90.f);
        coefficients[1] = 1.f - std::min(1.f, std::abs((float)90.f - Yaw) / 90.f);
        coefficients[2] = 1.f - std::min(1.f, std::abs((float)180.f - Yaw) / 90.f);
        coefficients[3] = 1.f - std::min(1.f, std::abs((float)270.f - Yaw) / 90.f);
        
        float tiltAngle = mmap(Roll, -90.f, 90.f, 0.f, 1.f, true);
        //Use Equal Power if engine requires
        /*
         float tiltHigh = cos(tiltAngle * (0.5 * PI));
         float tiltLow = cos((1.f0 - tiltAngle) * (0.5 * PI));
         */
        float tiltHigh = tiltAngle;
        float tiltLow = 1.f - tiltHigh;
        
        //ISSUE//
        //Able to kill stereo by making both pitch and tilt at max or min values together without proper clamps
        
        std::vector<float> result;
        result.resize(16);
        result[0] = coefficients[0] * tiltHigh * 2.0f; // 1 left
        result[1] = coefficients[3] * tiltHigh * 2.0f; //   right
        result[2] = coefficients[1] * tiltLow * 2.0f; // 2 left
        result[3] = coefficients[0] * tiltLow * 2.0f; //   right
        result[4] = coefficients[3] * tiltLow * 2.0f; // 3 left
        result[5] = coefficients[2] * tiltLow * 2.0f; //   right
        result[6] = coefficients[2] * tiltHigh * 2.0f; // 4 left
        result[7] = coefficients[1] * tiltHigh * 2.0f; //   right
        
        result[0 + 8] = coefficients[0] * tiltLow * 2.0f; // 1 left
        result[1 + 8] = coefficients[3] * tiltLow * 2.0f; //   right
        result[2 + 8] = coefficients[1] * tiltHigh * 2.0f; // 2 left
        result[3 + 8] = coefficients[0] * tiltHigh * 2.0f; //   right
        result[4 + 8] = coefficients[3] * tiltHigh * 2.0f; // 3 left
        result[5 + 8] = coefficients[2] * tiltHigh * 2.0f; //   right
        result[6 + 8] = coefficients[2] * tiltLow * 2.0f; // 4 left
        result[7 + 8] = coefficients[1] * tiltLow * 2.0f; //   right
        
        float pitchAngle = mmap(Pitch, 90.f, -90.f, 0., 1.f, true);
        //Use Equal Power if engine requires
        /*
         float pitchHigherHalf = cos(pitchAngle * (0.5*PI));
         float pitchLowerHalf = cos((1.f0 - pitchAngle) * (0.5*PI));
         */
        float pitchHigherHalf = pitchAngle;
        float pitchLowerHalf = 1.f - pitchHigherHalf;
        
        for (int i = 0; i < 8; i++) {
            result[i] *= pitchLowerHalf;
            result[i + 8] *= pitchHigherHalf;
        }
        
        result.push_back(1.f); // static stereo L
        result.push_back(1.f); // static stereo R
        
        
        return result;
    }
    
    void spatialAlgoSample(float Yaw, float Pitch, float Roll, float *result) {
        
        Mach1Point3DCore simulationAngles = Mach1Point3DCore(Yaw, Pitch, Roll);
        
        Mach1Point3DCore faceVector1 = Mach1Point3DCore(cos(mDegToRad(simulationAngles[0])),
                                    sin(mDegToRad(simulationAngles[0]))).normalize();
        
        
        Mach1Point3DCore faceVector2 = faceVector1.getRotated(simulationAngles[1],
                                                    Mach1Point3DCore(cos(mDegToRad(simulationAngles[0] - 90)),
                                                           sin(mDegToRad(simulationAngles[0] - 90))).normalize());
        
        
        Mach1Point3DCore faceVector21 = faceVector1.getRotated(simulationAngles[1] + 90,
                                                     Mach1Point3DCore(cos(mDegToRad(simulationAngles[0] - 90)),
                                                            sin(mDegToRad(simulationAngles[0] - 90))).normalize());
        
        Mach1Point3DCore faceVectorLeft = faceVector21.getRotated(-simulationAngles[2] + 90, faceVector2);
        Mach1Point3DCore faceVectorRight = faceVector21.getRotated(-simulationAngles[2] - 90, faceVector2);
        
        
        Mach1Point3DCore faceVectorOffsetted = Mach1Point3DCore(cos(mDegToRad(simulationAngles[0])),
                                            sin(mDegToRad(simulationAngles[0]))).normalize().rotate(
                                                                                                    simulationAngles[1] + 10,
                                                                                                    Mach1Point3DCore(cos(mDegToRad(simulationAngles[0] - 90)),
                                                                                                           sin(mDegToRad(simulationAngles[0] - 90))).normalize()) - faceVector2;
        
        Mach1Point3DCore tiltSphereRotated = faceVectorOffsetted.getRotated(-simulationAngles[2], faceVector2);
        
        // Drawing another 8 dots
        
        Mach1Point3DCore points[8] =
        { Mach1Point3DCore(100, -100, -100),
            Mach1Point3DCore(100, 100, -100),
            Mach1Point3DCore(-100, -100, -100),
            Mach1Point3DCore(-100, 100, -100),
            
            Mach1Point3DCore(100, -100, 100),
            Mach1Point3DCore(100, 100, 100),
            Mach1Point3DCore(-100, -100, 100),
            Mach1Point3DCore(-100, 100, 100)
            
        };

		Mach1Point3DCore planes[8][2] =
		{
			{ Mach1Point3DCore(0, 1, 0), Mach1Point3DCore(0, 100, 0) },
			{ Mach1Point3DCore(0, -1, 0), Mach1Point3DCore(0, -100, 0) },
			{ Mach1Point3DCore(1, 0, 0), Mach1Point3DCore(100, 0, 0) },
			{ Mach1Point3DCore(-1, 0, 0), Mach1Point3DCore(-100, 0, 0) },
			{ Mach1Point3DCore(0, 0, 1), Mach1Point3DCore(0, 0, 100) },
			{ Mach1Point3DCore(0, 0, -1), Mach1Point3DCore(0, 0, -100) }
		};
		
		// intersection
		Mach1Point3DCore contactL = faceVectorLeft * 100 + faceVector2 * 100;
		Mach1Point3DCore contactR = faceVectorRight * 100 + faceVector2 * 100;

		
		for (int j = 0; j < 8; j++) {
			linePlaneIntersection(contactL, Mach1Point3DCore(0, 0, 0), faceVectorLeft * 100 + faceVector2 * 100, planes[j][0], planes[j][1]);
			linePlaneIntersection(contactR, Mach1Point3DCore(0, 0, 0), faceVectorRight * 100 + faceVector2 * 100, planes[j][0], planes[j][1]);
		}

        
        float qL[8];
        for (int i = 0; i < 8; i++) {
            qL[i] = (contactL - points[i]).length();
        }
        
        float qR[8];
        for (int i = 0; i < 8; i++) {
            qR[i] = (contactR - points[i]).length();
        }
        
		float d = sqrt(100 * 100 + 200 * 200);
		for (int i = 0; i < 8; i++) {
			float vL = clamp(mmap(qL[i], 0, d, 1.f, 0.), 0, 1);
			float vR = clamp(mmap(qR[i], 0, d, 1.f, 0.), 0, 1);

			result[i * 2 + 0] = vL;
			result[i * 2 + 1] = vR;
		}
        
        // Volume Balancer v2.0
		float sumL = 0, sumR = 0;
        for (int i = 0; i < 8; i++) {
            sumL += result[i * 2];
            sumR += result[i * 2 + 1];
        }
        
		for (int i = 0; i < 8; i++) {
			result[i * 2 + 0] /= sumL;
			result[i * 2 + 1] /= sumR;
		}

		//if(sumL > 1.0 || sumR > 1.0) printf("%f - %f\r\n", sumL, sumR);

		result[8 + 8] = 1.0f; // static stereo L
        result[9 + 8] = 1.0f; // static stereo R
        
    };
    
    std::vector<float> spatialAlgoSample(float Yaw, float Pitch, float Roll) {
        
        Mach1Point3DCore simulationAngles = Mach1Point3DCore(Yaw, Pitch, Roll);
        
        Mach1Point3DCore faceVector1 = Mach1Point3DCore(cos(mDegToRad(simulationAngles[0])),
                                    sin(mDegToRad(simulationAngles[0]))).normalize();
        
        
        Mach1Point3DCore faceVector2 = faceVector1.getRotated(simulationAngles[1],
                                                    Mach1Point3DCore(cos(mDegToRad(simulationAngles[0] - 90)),
                                                           sin(mDegToRad(simulationAngles[0] - 90))).normalize());
        
        
        Mach1Point3DCore faceVector21 = faceVector1.getRotated(simulationAngles[1] + 90,
                                                     Mach1Point3DCore(cos(mDegToRad(simulationAngles[0] - 90)),
                                                            sin(mDegToRad(simulationAngles[0] - 90))).normalize());
        
        Mach1Point3DCore faceVectorLeft = faceVector21.getRotated(-simulationAngles[2] + 90, faceVector2);
        Mach1Point3DCore faceVectorRight = faceVector21.getRotated(-simulationAngles[2] - 90, faceVector2);
        
        
        Mach1Point3DCore faceVectorOffsetted = Mach1Point3DCore(cos(mDegToRad(simulationAngles[0])),
                                            sin(mDegToRad(simulationAngles[0]))).normalize().rotate(
                                                                                                    simulationAngles[1] + 10,
                                                                                                    Mach1Point3DCore(cos(mDegToRad(simulationAngles[0] - 90)),
                                                                                                           sin(mDegToRad(simulationAngles[0] - 90))).normalize()) - faceVector2;
        
        Mach1Point3DCore tiltSphereRotated = faceVectorOffsetted.rotate(-simulationAngles[2], faceVector2);
        
        // Drawing another 8 dots
        
		Mach1Point3DCore points[8] =
		{ Mach1Point3DCore(100, -100, -100),
			Mach1Point3DCore(100, 100, -100),
			Mach1Point3DCore(-100, -100, -100),
			Mach1Point3DCore(-100, 100, -100),

			Mach1Point3DCore(100, -100, 100),
			Mach1Point3DCore(100, 100, 100),
			Mach1Point3DCore(-100, -100, 100),
			Mach1Point3DCore(-100, 100, 100)

		};

		Mach1Point3DCore planes[8][2] =
		{
			{ Mach1Point3DCore(0, 1, 0), Mach1Point3DCore(0, 100, 0) },
			{ Mach1Point3DCore(0, -1, 0), Mach1Point3DCore(0, -100, 0) },
			{ Mach1Point3DCore(1, 0, 0), Mach1Point3DCore(100, 0, 0) },
			{ Mach1Point3DCore(-1, 0, 0), Mach1Point3DCore(-100, 0, 0) },
			{ Mach1Point3DCore(0, 0, 1), Mach1Point3DCore(0, 0, 100) },
			{ Mach1Point3DCore(0, 0, -1), Mach1Point3DCore(0, 0, -100) }
		};

		// intersection
		Mach1Point3DCore contactL = faceVectorLeft * 100 + faceVector2 * 100;
		Mach1Point3DCore contactR = faceVectorRight * 100 + faceVector2 * 100;
		for (int j = 0; j < 8; j++) {
			linePlaneIntersection(contactL, Mach1Point3DCore(0, 0, 0), faceVectorLeft * 100 + faceVector2 * 100, planes[j][0], planes[j][1]);
			linePlaneIntersection(contactR, Mach1Point3DCore(0, 0, 0), faceVectorRight * 100 + faceVector2 * 100, planes[j][0], planes[j][1]);
		}


		float qL[8];
		for (int i = 0; i < 8; i++) {
			qL[i] = (contactL - points[i]).length();
		}

		float qR[8];
		for (int i = 0; i < 8; i++) {
			qR[i] = (contactR - points[i]).length();
		}
        
        std::vector<float> result;
        result.resize(16);
        
		float d = sqrt(100 * 100 + 200 * 200);
		for (int i = 0; i < 8; i++) {
			float vL = clamp(mmap(qL[i], 0, d, 1.f, 0.), 0, 1);
			float vR = clamp(mmap(qR[i], 0, d, 1.f, 0.), 0, 1);

			result[i * 2 + 0] = vL;
			result[i * 2 + 1] = vR;
		}

		// Volume Balancer v2.0
		float sumL = 0, sumR = 0;
		for (int i = 0; i < 8; i++) {
			sumL += result[i * 2];
			sumR += result[i * 2 + 1];
		}

		for (int i = 0; i < 8; i++) {
			result[i * 2 + 0] /= sumL;
			result[i * 2 + 1] /= sumR;
		}

		//if(sumL > 1.0 || sumR > 1.0) printf("%f - %f\r\n", sumL, sumR);
        
        
        result.push_back(1.f); // static stereo L
        result.push_back(1.f); // static stereo R
        
        return result;
        
    };
    
	// log
	std::vector<std::string> strLog;
	void addToLog(std::string str, int maxCount = 100);

public:
    
 	char* getLog();

    float filterSpeed;

	// Angular settings functions
	static void convertAnglesToMach1(Mach1PlatformType platformType, float* Y, float* P, float* R);
	static void convertAnglesToPlatform(Mach1PlatformType platformType, float* Y, float* P, float* R);

	Mach1Point3DCore getCurrentAngle() {
		Mach1Point3DCore angle(currentYaw, currentPitch, currentRoll);
		return angle;
	}

    Mach1DecodeCore();
    
	void setPlatformType(Mach1PlatformType type);
	Mach1PlatformType getPlatformType();


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
