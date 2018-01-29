//  Mach1 SDK
//  Copyright © 2017 Mach1. All rights reserved.
//
//  Header file

#pragma once

#ifndef Mach1Decode_h
#define Mach1Decode_h

#include <vector>
#include <cmath>
#include <chrono>
#include <string>
#include <algorithm>

using namespace std::chrono;

#ifndef PI
#define PI       3.14159265358979323846
#endif 



//////////////

class Mach1Decode {
    
public:
    enum AngularSettingsType {
        m1Default = 0, m1Unity, m1UE, m1oFEasyCam, m1Android, m1iOS
    };

private:
    
	typedef std::vector<float> (Mach1Decode::*functionAlgoSample)(float Yaw, float Pitch, float Roll);
	std::vector<float> processSample(functionAlgoSample funcAlgoSample, float Yaw, float Pitch, float Roll, int bufferSize = 0, int sampleIndex = 0);
	milliseconds ms;

    // Math utilities
    
    static inline float mDegToRad(float degrees);
    
    static inline float mmap(float value, float inputMin, float inputMax, float outputMin, float outputMax, bool clamp = false);
    
    static inline float clamp(float a, float min, float max );
    
    static inline float alignAngle(float a, float min = -180, float max = 180);
    
    static inline float lerp(float x1, float x2, float t);

    
    float radialDistance(float angle1, float angle2);
    
    float targetDirectionMultiplier(float angleCurrent, float angleTarget);
    
    // Filter features
    // Envelope follower feature is defined here, in updateAngles()
    
    void updateAngles();
    
	float currentYaw, currentPitch, currentRoll;
	float targetYaw, targetPitch, targetRoll;
    float previousYaw, previousPitch, previousRoll;
    
	long timeLastUpdate;
   
    bool smoothAngles;
 
    // Angular settings functions
    void fillPlatformAngles(AngularSettingsType type, float* Y, float* P, float* R);
    
    AngularSettingsType angularSetting;
    
    std::vector<float> horizonPairsAlgoSample(float Yaw, float Pitch, float Roll) {
        //Orientation input safety clamps/alignment
        Yaw = alignAngle(Yaw, 0, 360);
        
        float volumes[4];
        volumes[0] = 1.f - std::min(1.f, std::min((float)360.f - Yaw, Yaw) / 90.f);
        volumes[1] = 1.f - std::min(1.f, std::abs((float)90.f - Yaw) / 90.f);
        volumes[2] = 1.f - std::min(1.f, std::abs((float)180.f - Yaw) / 90.f);
        volumes[3] = 1.f - std::min(1.f, std::abs((float)270.f - Yaw) / 90.f);
        
        std::vector<float> result;
        result.push_back(volumes[0]);
        result.push_back(volumes[1]);
        result.push_back(volumes[2]);
        result.push_back(volumes[3]);
        
        return result;
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
        result[0] = coefficients[0] * tiltHigh * 2.0; // 1 left
        result[1] = coefficients[3] * tiltHigh * 2.0; //   right
        result[2] = coefficients[1] * tiltLow * 2.0; // 2 left
        result[3] = coefficients[0] * tiltLow * 2.0; //   right
        result[4] = coefficients[3] * tiltLow * 2.0; // 3 left
        result[5] = coefficients[2] * tiltLow * 2.0; //   right
        result[6] = coefficients[2] * tiltHigh * 2.0; // 4 left
        result[7] = coefficients[1] * tiltHigh * 2.0; //   right
        
        result[0 + 8] = coefficients[0] * tiltLow * 2.0; // 1 left
        result[1 + 8] = coefficients[3] * tiltLow * 2.0; //   right
        result[2 + 8] = coefficients[1] * tiltHigh * 2.0; // 2 left
        result[3 + 8] = coefficients[0] * tiltHigh * 2.0; //   right
        result[4 + 8] = coefficients[3] * tiltHigh * 2.0; // 3 left
        result[5 + 8] = coefficients[2] * tiltHigh * 2.0; //   right
        result[6 + 8] = coefficients[2] * tiltLow * 2.0; // 4 left
        result[7 + 8] = coefficients[1] * tiltLow * 2.0; //   right
        
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
    
    std::vector<float> spatialAlgoSample(float Yaw, float Pitch, float Roll) {
        
        mPoint simulationAngles = mPoint(Yaw, Pitch, Roll);
        
        mPoint faceVector1 = mPoint(cos(mDegToRad(simulationAngles[0])),
                                    sin(mDegToRad(simulationAngles[0]))).normalize();
        
        
        mPoint faceVector2 = faceVector1.getRotated(simulationAngles[1],
                                                    mPoint(cos(mDegToRad(simulationAngles[0] - 90)),
                                                           sin(mDegToRad(simulationAngles[0] - 90))).normalize());
        
        
        mPoint faceVector21 = faceVector1.getRotated(simulationAngles[1] + 90,
                                                     mPoint(cos(mDegToRad(simulationAngles[0] - 90)),
                                                            sin(mDegToRad(simulationAngles[0] - 90))).normalize());
        
        mPoint faceVectorLeft = faceVector21.getRotated(-simulationAngles[2] + 90, faceVector2);
        mPoint faceVectorRight = faceVector21.getRotated(-simulationAngles[2] - 90, faceVector2);
        
        
        mPoint faceVectorOffsetted = mPoint(cos(mDegToRad(simulationAngles[0])),
                                            sin(mDegToRad(simulationAngles[0]))).normalize().rotate(
                                                                                                    simulationAngles[1] + 10,
                                                                                                    mPoint(cos(mDegToRad(simulationAngles[0] - 90)),
                                                                                                           sin(mDegToRad(simulationAngles[0] - 90))).normalize()) - faceVector2;
        
        mPoint tiltSphereRotated = faceVectorOffsetted.rotate(-simulationAngles[2], faceVector2);
        
        // Drawing another 8 dots
        
        mPoint points[8] =
        { mPoint(100, -100, -100),
            mPoint(100, 100, -100),
            mPoint(-100, -100, -100),
            mPoint(-100, 100, -100),
            
            mPoint(100, -100, 100),
            mPoint(100, 100, 100),
            mPoint(-100, -100, 100),
            mPoint(-100, 100, 100)
            
        };
        
        float qL[8];
        for (int i = 0; i < 8; i++) {
            qL[i] = (faceVectorLeft * 100 + faceVector2 * 100 - points[i]).length();
        }
        
        float qR[8];
        for (int i = 0; i < 8; i++) {
            qR[i] = (faceVectorRight * 100 + faceVector2 * 100 - points[i]).length();
        }
        
        std::vector<float> result;
        result.resize(16);
        
        for (int i = 0; i < 8; i++) {
            float vL = clamp(mmap(qL[i], 0, 223, 1.f, 0.), 0, 1);
            float vR = clamp(mmap(qR[i], 0, 223, 1.f, 0.), 0, 1);
            
            result[i * 2] = vL;
            result[i * 2 + 1] = vR;
            
        }
        
        // Volume Balancer v2.0
        
        float sumL = 0, sumR = 0;
        for (int i = 0; i < 8; i++) {
            sumL += result[i * 2];
            sumR += result[i * 2 + 1];
        }
        
        float multipliersL[8], multipliersR[8];
        for (int i = 0; i < 8; i++) {
            multipliersL[i] = result[i * 2] / sumL;
            multipliersR[i] = result[i * 2 + 1] / sumR;
        }
        
        float sumDiffL = sumL - 1.f;
        float sumDiffR = sumR - 1.f;
        
        float correctedVolumesL[8], correctedVolumesR[8];
        for (int i = 0; i < 8; i++) {
            correctedVolumesL[i] = result[i * 2] - sumDiffL * multipliersL[i];
            correctedVolumesR[i] = result[i * 2 + 1] - sumDiffR * multipliersR[i];
        }
        
        for (int i = 0; i < 8; i++) {
            result[i * 2] = correctedVolumesL[i];
            result[i * 2 + 1] = correctedVolumesR[i];
        }
        
        
        result.push_back(1.f); // static stereo L
        result.push_back(1.f); // static stereo R
        
        return result;
        
    };
    
	// log
	std::vector<std::string> strLog;
	void addToLog(std::string str, int maxCount = 100);

public:
    
 	char* getLog();

   // for test
    float filterSpeed;
    
    struct mPoint {
        float x, y, z;
        
        mPoint();
        
        mPoint(float X, float Y, float Z);
        
        mPoint(float X, float Y);
        
        inline mPoint operator+( const mPoint& pnt ) const;
        inline mPoint operator*( const float f ) const;
        inline mPoint operator*( const mPoint& vec ) const;
        inline mPoint operator-( const mPoint& vec ) const;
        inline float length() const;
        float operator[] (int index);
        inline mPoint& rotate( float angle, const mPoint& axis );
        inline mPoint& normalize();
        inline mPoint getNormalized() const;
        inline mPoint getRotated( float angle, const mPoint& axis ) const;
    };
    
	mPoint getCurrentAngle() {
		return mPoint(currentYaw, currentPitch, currentRoll);
	}
    
    Mach1Decode();
    
    void setAngularSettingsType(AngularSettingsType type);

    void beginBuffer();
    void endBuffer();

    long getCurrentTime();

    //--------------------------------------------------
    
    //
    //  Four channel audio format
    //
    //  Order of input angles:
    //  Y = Yaw in angles
    //  P = Pitch in angles
    //  R = Roll in angles
    //
    
    std::vector<float> horizonAlgo(float Yaw, float Pitch, float Roll, int bufferSize = 0, int sampleIndex = 0);
    
    // ------------------------------------------------------------------
    
    //
    //  Four pairs audio format.
    //
    //  Order of input angles:
    //  Y = Yaw in angles
    //  P = Pitch in angles
    //  R = Roll in angles
    //
    
    std::vector<float> horizonPairsAlgo(float Yaw, float Pitch, float Roll, int bufferSize = 0, int sampleIndex = 0);
    
    // ------------------------------------------------------------------
    
    //
    //  Eight channel audio format (isotropic version).
    //
    //  Order of input angles:
    //  Y = Yaw in angles
    //  P = Pitch in angles
    //  R = Roll in angles
    //
    
    std::vector<float> spatialAlgo(float Yaw, float Pitch, float Roll, int bufferSize = 0, int sampleIndex = 0);
    
    // ------------------------------------------------------------------
    
    //
    //  Eight channel audio format.
    //
    //  Order of input angles:
    //  Y = Yaw in angles
    //  P = Pitch in angles
    //  R = Roll in angles
    //
    
    std::vector<float> spatialAltAlgo(float Yaw, float Pitch, float Roll, int bufferSize = 0, int sampleIndex = 0);
    
    // ------------------------------------------------------------------
    
    //
    //  Eight pairs audio format.
    //
    //  Order of input angles:
    //  Y = Yaw in angles
    //  P = Pitch in angles
    //  R = Roll in angles
    //
    
    std::vector<float> spatialPairsAlgo(float Yaw, float Pitch, float Roll, int bufferSize = 0, int sampleIndex = 0);
    
    // ------------------------------------------------------------------
    
};

#endif /* Mach1Decode_h */