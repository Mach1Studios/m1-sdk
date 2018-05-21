//  Mach1 SDK
//  Copyright © 2017 Mach1. All rights reserved.

#pragma once

#include <vector>

class Mach1Decode
{

public:
    enum AngularSettingsType {
        m1Default = 0, m1Unity, m1UE, m1oFEasyCam, m1Android, m1iOSPortrait, m1iOSLandscape
    };

	Mach1Decode();
	~Mach1Decode();

	void* M1obj;


	std::vector<float> horizonAlgo(float Yaw, float Pitch, float Roll, int bufferSize = 0, int sampleIndex = 0);
    void horizonAlgo(float Yaw, float Pitch, float Roll, float *result, int bufferSize = 0, int sampleIndex = 0);
    
	std::vector<float> horizonPairsAlgo(float Yaw, float Pitch, float Roll, int bufferSize = 0, int sampleIndex = 0);
    void horizonPairsAlgo(float Yaw, float Pitch, float Roll, float *result, int bufferSize = 0, int sampleIndex = 0);

    
	std::vector<float> spatialAlgo(float Yaw, float Pitch, float Roll, int bufferSize = 0, int sampleIndex = 0);
    void spatialAlgo(float Yaw, float Pitch, float Roll, float *result, int bufferSize, int sampleIndex);

    
	std::vector<float> spatialAltAlgo(float Yaw, float Pitch, float Roll, int bufferSize = 0, int sampleIndex = 0);
    void spatialAltAlgo(float Yaw, float Pitch, float Roll, float *result, int bufferSize = 0, int sampleIndex = 0);

    
	std::vector<float> spatialPairsAlgo(float Yaw, float Pitch, float Roll, int bufferSize = 0, int sampleIndex = 0);
    void spatialPairsAlgo(float Yaw, float Pitch, float Roll, float *result, int bufferSize = 0, int sampleIndex = 0);


    void setAngularSettingsType(AngularSettingsType type);
    void setFilterSpeed(float filterSpeed);

	void beginBuffer();
    void endBuffer();
	
	long getCurrentTime();
	char* getLog();
    Mach1Point3D getCurrentAngle();
};
