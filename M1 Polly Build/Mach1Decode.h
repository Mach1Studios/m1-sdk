//
//  Mach1 SDK
//
//  Multichannel audio format family
//
//  Mixing algorithms v 0.9.94b
//
//  Header file

#pragma once

#ifndef Mach1Decode_h
#define Mach1Decode_h

#include <vector>
#include <cmath>
#include <chrono>

using namespace std::chrono;

#ifndef PI
#define PI       3.14159265358979323846
#endif 

//
// Point utility class
//

//////////////

class Mach1Decode {
    
	milliseconds ms;

    //
    static float mDegToRad(float degrees);
    
    
    //
    // Map utility
    //
    
    static float mmap(float value, float inputMin, float inputMax, float outputMin, float outputMax, bool clamp = false);
    
    static float clamp(float a, float min, float max );
    
    static float alignAngle(float a, float min = -180, float max = 180);
    
    float radialDistance(float angle1, float angle2);
    
    float targetDirectionMultiplier(float angleCurrent, float angleTarget);
    
    // Envelope follower feature is defined here, in updateAngles()
    
    void updateAngles();
    
	float currentYaw, currentPitch, currentRoll;
	float targetYaw, targetPitch, targetRoll;

	long timeLastUpdate;

public:

	float speed;

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
    
    std::vector<float> horizonAlgo(float Yaw, float Pitch, float Roll, bool smoothAngles = false);
    
    // ------------------------------------------------------------------
    
    //
    //  Four pairs audio format.
    //
    //  Order of input angles:
    //  Y = Yaw in angles
    //  P = Pitch in angles
    //  R = Roll in angles
    //
    
    std::vector<float> horizonPairsAlgo(float Yaw, float Pitch, float Roll, bool smoothAngles = false);
    
    // ------------------------------------------------------------------
    
    //
    //  Eight channel audio format (isotropic version).
    //
    //  Order of input angles:
    //  Y = Yaw in angles
    //  P = Pitch in angles
    //  R = Roll in angles
    //
    
    std::vector<float> spatialAlgo(float Yaw, float Pitch, float Roll, bool smoothAngles = false);
    
    // ------------------------------------------------------------------
    
    //
    //  Eight channel audio format.
    //
    //  Order of input angles:
    //  Y = Yaw in angles
    //  P = Pitch in angles
    //  R = Roll in angles
    //
    
    std::vector<float> spatialAltAlgo(float Yaw, float Pitch, float Roll, bool smoothAngles = false);
    
    // ------------------------------------------------------------------
    
    //
    //  Eight pairs audio format.
    //
    //  Order of input angles:
    //  Y = Yaw in angles
    //  P = Pitch in angles
    //  R = Roll in angles
    //
    
    std::vector<float> spatialPairsAlgo(float Yaw, float Pitch, float Roll, bool smoothAngles = false);
    
    // ------------------------------------------------------------------
    
};

#endif /* Mach1Decode_h */