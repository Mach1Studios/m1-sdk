#include "M1DSPAlgorithms.h"
#include "M1DSPAlgorithmsCAPI.h"

M1DSPAlgorithms::M1DSPAlgorithms()
{
	M1obj = M1DSPAlgorithmsCAPI_create();
}

M1DSPAlgorithms::~M1DSPAlgorithms()
{
	M1DSPAlgorithmsCAPI_delete(M1obj);
} 

std::vector<float> M1DSPAlgorithms::fourChannelAlgorithm(float Yaw, float Pitch, float Roll, bool smoothAngles)
{
	float* data = M1DSPAlgorithmsCAPI_fourChannelAlgorithm(M1obj, Yaw, Pitch, Roll, smoothAngles);
	return std::vector<float>(data, data + 10);
}

std::vector<float> M1DSPAlgorithms::fourPairsAlgorithm(float Yaw, float Pitch, float Roll, bool smoothAngles)
{
	float* data = M1DSPAlgorithmsCAPI_fourPairsAlgorithm(M1obj, Yaw, Pitch, Roll, smoothAngles);
	return std::vector<float>(data, data + 6);
}

std::vector<float> M1DSPAlgorithms::eightChannelsIsotropicAlgorithm(float Yaw, float Pitch, float Roll, bool smoothAngles)
{
	float* data = M1DSPAlgorithmsCAPI_eightChannelsIsotropicAlgorithm(M1obj, Yaw, Pitch, Roll, smoothAngles);
	return std::vector<float>(data, data + 18);
}

std::vector<float> M1DSPAlgorithms::eightChannelsAlgorithm(float Yaw, float Pitch, float Roll, bool smoothAngles)
{
	float* data = M1DSPAlgorithmsCAPI_eightChannelsAlgorithm(M1obj, Yaw, Pitch, Roll, smoothAngles);
	return std::vector<float>(data, data + 18);
}

std::vector<float> M1DSPAlgorithms::eightPairsAlgorithm(float Yaw, float Pitch, float Roll, bool smoothAngles)
{
	float* data = M1DSPAlgorithmsCAPI_eightPairsAlgorithm(M1obj, Yaw, Pitch, Roll, smoothAngles);
	return std::vector<float>(data, data + 10);
}

long M1DSPAlgorithms::getCurrentTime()
{
	return M1DSPAlgorithmsCAPI_getCurrentTime(M1obj);
}
