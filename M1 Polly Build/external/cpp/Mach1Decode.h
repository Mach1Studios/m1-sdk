#pragma once

#include <vector>

class Mach1Decode
{
	void* M1obj;

public:

	Mach1Decode();
	~Mach1Decode();

	std::vector<float> horizonAlgo(float Yaw, float Pitch, float Roll, bool smoothAngles = false);
	std::vector<float> horizonPairsAlgo(float Yaw, float Pitch, float Roll, bool smoothAngles = false);
	std::vector<float> spatialAlgo(float Yaw, float Pitch, float Roll, bool smoothAngles = false);
	std::vector<float> spatialAltAlgo(float Yaw, float Pitch, float Roll, bool smoothAngles = false);
	std::vector<float> spatialPairsAlgo(float Yaw, float Pitch, float Roll, bool smoothAngles = false);

	long getCurrentTime();
};