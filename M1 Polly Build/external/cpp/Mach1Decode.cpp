#include "Mach1Decode.h"
#include "Mach1DecodeCAPI.h"

Mach1Decode::Mach1Decode()
{
	M1obj = Mach1DecodeCAPI_create();
}

Mach1Decode::~Mach1Decode()
{
	Mach1DecodeCAPI_delete(M1obj);
} 

std::vector<float> Mach1Decode::horizonAlgo(float Yaw, float Pitch, float Roll, bool smoothAngles)
{
	float* data = Mach1DecodeCAPI_fourChannelAlgorithm(M1obj, Yaw, Pitch, Roll, smoothAngles);
	return std::vector<float>(data, data + 10);
}

std::vector<float> Mach1Decode::horizonPairsAlgo(float Yaw, float Pitch, float Roll, bool smoothAngles)
{
	float* data = Mach1DecodeCAPI_fourPairsAlgorithm(M1obj, Yaw, Pitch, Roll, smoothAngles);
	return std::vector<float>(data, data + 6);
}

std::vector<float> Mach1Decode::spatialAlgo(float Yaw, float Pitch, float Roll, bool smoothAngles)
{
	float* data = Mach1DecodeCAPI_eightChannelsIsotropicAlgorithm(M1obj, Yaw, Pitch, Roll, smoothAngles);
	return std::vector<float>(data, data + 18);
}

std::vector<float> Mach1Decode::spatialAltAlgo(float Yaw, float Pitch, float Roll, bool smoothAngles)
{
	float* data = Mach1DecodeCAPI_eightChannelsAlgorithm(M1obj, Yaw, Pitch, Roll, smoothAngles);
	return std::vector<float>(data, data + 18);
}

std::vector<float> Mach1Decode::spatialPairsAlgo(float Yaw, float Pitch, float Roll, bool smoothAngles)
{
	float* data = Mach1DecodeCAPI_eightPairsAlgorithm(M1obj, Yaw, Pitch, Roll, smoothAngles);
	return std::vector<float>(data, data + 10);
}

long Mach1Decode::getCurrentTime()
{
	return Mach1DecodeCAPI_getCurrentTime(M1obj);
}
