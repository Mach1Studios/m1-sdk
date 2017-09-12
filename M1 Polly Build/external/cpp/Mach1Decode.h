#pragma once

#include <vector>

class Mach1Decode
{
	void* M1obj;

public:
    enum AngularSettingsType {
        m1Default = 0, m1Unity, m1UE, m1oFEasyCam, m1Android, m1iOS
    };

	Mach1Decode();
	~Mach1Decode();

	std::vector<float> horizonAlgo(float Yaw, float Pitch, float Roll, int bufferSize = 0, int sampleIndex = 0);
	std::vector<float> horizonPairsAlgo(float Yaw, float Pitch, float Roll, int bufferSize = 0, int sampleIndex = 0);
	std::vector<float> spatialAlgo(float Yaw, float Pitch, float Roll, int bufferSize = 0, int sampleIndex = 0);
	std::vector<float> spatialAltAlgo(float Yaw, float Pitch, float Roll, int bufferSize = 0, int sampleIndex = 0);
	std::vector<float> spatialPairsAlgo(float Yaw, float Pitch, float Roll, int bufferSize = 0, int sampleIndex = 0);

    void setAngularSettingsType(AngularSettingsType type);
 
	void beginBuffer();
    void endBuffer();
	
	long getCurrentTime();
};