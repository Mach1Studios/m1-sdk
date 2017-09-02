#include "M1DSPAlgorithmsCAPI.h"
#include "M1DSPAlgorithms.h"

void* M1DSPAlgorithmsCAPI_create()
{
	return new M1DSPAlgorithms();
}

void M1DSPAlgorithmsCAPI_delete(void * M1obj)
{
	if (M1obj != nullptr)
		delete M1obj;
}

float* M1DSPAlgorithmsCAPI_fourChannelAlgorithm(void* M1obj, float Yaw, float Pitch, float Roll, bool smoothAngles)
{
	static float data[10];
	
	if (M1obj != nullptr) {
		std::vector<float> v = ((M1DSPAlgorithms*)M1obj)->fourChannelAlgorithm(Yaw, Pitch, Roll, smoothAngles);
		for (int i = 0; i < 10; i++) data[i] = v[i];
	}
	else {
		for (int i = 0; i < 10; i++) data[i] = 0;
	}

	return data;
}

float* M1DSPAlgorithmsCAPI_fourPairsAlgorithm(void* M1obj, float Yaw, float Pitch, float Roll, bool smoothAngles)
{
	static float data[6];

	if (M1obj != nullptr) {
		std::vector<float> v = ((M1DSPAlgorithms*)M1obj)->fourPairsAlgorithm(Yaw, Pitch, Roll, smoothAngles);
		for (int i = 0; i < 6; i++) data[i] = v[i];
	}
	else {
		for (int i = 0; i < 6; i++) data[i] = 0;
	}

	return data;
}

float* M1DSPAlgorithmsCAPI_eightChannelsIsotropicAlgorithm(void* M1obj, float Yaw, float Pitch, float Roll, bool smoothAngles)
{
	static float data[18];

	if (M1obj != nullptr) {
		std::vector<float> v = ((M1DSPAlgorithms*)M1obj)->eightChannelsIsotropicAlgorithm(Yaw, Pitch, Roll, smoothAngles);
		for (int i = 0; i < 18; i++) data[i] = v[i];
	}
	else {
		for (int i = 0; i < 18; i++) data[i] = 0;
	}

	return data;
}

float* M1DSPAlgorithmsCAPI_eightChannelsAlgorithm(void* M1obj, float Yaw, float Pitch, float Roll, bool smoothAngles)
{
	static float data[18];

	if (M1obj != nullptr) {
		std::vector<float> v = ((M1DSPAlgorithms*)M1obj)->eightChannelsAlgorithm(Yaw, Pitch, Roll, smoothAngles);
		for (int i = 0; i < 18; i++) data[i] = v[i];
	}
	else {
		for (int i = 0; i < 18; i++) data[i] = 0;
	}

	return data;
}

float* M1DSPAlgorithmsCAPI_eightPairsAlgorithm(void* M1obj, float Yaw, float Pitch, float Roll, bool smoothAngles)
{
	static float data[10];

	if (M1obj != nullptr) {
		std::vector<float> v = ((M1DSPAlgorithms*)M1obj)->eightPairsAlgorithm(Yaw, Pitch, Roll, smoothAngles);
		for (int i = 0; i < 10; i++) data[i] = v[i];
	}
	else {
		for (int i = 0; i < 10; i++) data[i] = 0;
	}

	return data;
}

long M1DSPAlgorithmsCAPI_getCurrentTime(void * M1obj)
{
	return ((M1DSPAlgorithms*)M1obj)->getCurrentTime();
}
