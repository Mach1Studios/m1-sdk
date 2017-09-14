//
//  Mach1 SDK
//
//  Multichannel audio format family
//
//  Mixing algorithms v 0.9.96a
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



//////////////

class Mach1Decode {
    
public:
    enum AngularSettingsType {
        m1Default = 0, m1Unity, m1UE, m1oFEasyCam, m1Android, m1iOS
    };

private:
    
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
 
 	float filterSpeed;
   
    // Angular settings functions
    void fillPlatformAngles(AngularSettingsType type, float* Y, float* P, float* R) {
        switch(type) {
            case m1Default:
                *Y = *Y;
                *P = *P;
                *R = *R;
                break;
                
            case m1Unity:
                *Y = -*Y;                   // Y in Unity
                *P = -*P - 180;             // X in Unity
                *R = *R;                    // Z in Unity
                break;
                
            case m1UE:
                *Y = *Y;                    // Y in UE
                (*P < 0 ? 360 + *P : *P);   // Z in UE
                *R = *R;                    // X in UE
                
            case m1oFEasyCam:
                *Y = *Y;
                *P = *P;
                *R = *R;
                
            case m1Android:
                *Y = -*Y + 90;
                *P = -*P;
                *R = *R;

            case m1iOS:
                *Y = *Y;
                *P = *P;
                *R = *R;
                
            default:
                break;
        }
    }
    
    AngularSettingsType angularSetting;

public:
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