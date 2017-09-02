#pragma once

#include <vector>

class M1DSPAlgorithms
{
	void* M1obj;

public:

	M1DSPAlgorithms();
	~M1DSPAlgorithms();

	std::vector<float> fourChannelAlgorithm(float Yaw, float Pitch, float Roll, bool smoothAngles = false);
	std::vector<float> fourPairsAlgorithm(float Yaw, float Pitch, float Roll, bool smoothAngles = false);
	std::vector<float> eightChannelsIsotropicAlgorithm(float Yaw, float Pitch, float Roll, bool smoothAngles = false);
	std::vector<float> eightChannelsAlgorithm(float Yaw, float Pitch, float Roll, bool smoothAngles = false);
	std::vector<float> eightPairsAlgorithm(float Yaw, float Pitch, float Roll, bool smoothAngles = false);

	long getCurrentTime();
};