//  Mach1 Spatial SDK
//  Copyright © 2017-2020 Mach1. All rights reserved.

#pragma once

#include <vector>
#include <string>
#include "Mach1DecodeMINCAPI.h"

class Mach1Decode
{
	void* M1obj;

public:

	Mach1Decode();
	~Mach1Decode();

	void setDecodeAlgoType(Mach1DecodeAlgoType newAlgorithmType);

#ifndef  __EMSCRIPTEN__ 
	void decode(float Yaw, float Pitch, float Roll, float *result, int bufferSize = 0, int sampleIndex = 0);
	void decodeCoeffs(float *result, int bufferSize = 0, int sampleIndex = 0);
#endif
	std::vector<float> decode(float Yaw, float Pitch, float Roll, int bufferSize = 0, int sampleIndex = 0);
	std::vector<float> decodeCoeffs(int bufferSize = 0, int sampleIndex = 0);
	std::vector<float> decodeCoeffsUsingTranscodeMatrix(std::vector< std::vector<float> > matrix, int channels, int bufferSize = 0, int sampleIndex = 0);

	int getFormatChannelCount();
	void setRotation(Mach1Point3D newRotationFromMinusOnetoOne);
	void setRotationDegrees(Mach1Point3D newRotationDegrees);
	void setRotationRadians(Mach1Point3D newRotationRadians);
	void setRotationQuat(Mach1Point4D newRotationQuat);

	void beginBuffer();
	void endBuffer();

	template<typename T>
	void decodeBuffer(std::vector< std::vector<T> >* inBuffer, std::vector< std::vector<T> >* outBuffer, int inputPoints, int bufferSize);

	template<typename T>
	void decodeBuffer(std::vector<T*>* inBuffer, std::vector<T*>* outBuffer, int inputPoints, int bufferSize);

    Mach1Point3D getCurrentAngle();
};

template<typename T>
void Mach1Decode::decodeBuffer(std::vector< std::vector<T> >* inBuffer, std::vector< std::vector<T> >* outBuffer, int inputPoints, int bufferSize)
{
	beginBuffer();

	T sample = 0;
	int cOffset = 0;
	int inputChannelsCount = inBuffer->size() / inputPoints;
	int outChannelsCount = outBuffer->size();

	std::vector<float> startVolumes = decodeCoeffs(bufferSize, 0);
	std::vector<float> endVolumes = decodeCoeffs(bufferSize, bufferSize);
	std::vector<float> volumes(inputChannelsCount * 2);

	float* startVol = startVolumes.data();
	float* endVol = endVolumes.data();
	float* vol = volumes.data();

	for (size_t i = 0; i < bufferSize; i++) {
		float lerp = float(i) / bufferSize;
		for (size_t c = 0; c < volumes.size(); c++) {
			vol[c] = startVol[c] * (1 - lerp) + endVol[c] * lerp;
		}

		for (size_t c = 0; c < outChannelsCount; c++) {
			sample = 0;
			cOffset = c < inputPoints ? c : 0;
			for (size_t k = 0; k < inputChannelsCount; k++)
			{
				sample += inBuffer->operator[](k * inputPoints + cOffset)[i] * vol[k * 2 + c];
			}
			outBuffer->operator[](c)[i] = sample;
		}
	}

	endBuffer();
}

template<typename T>
void Mach1Decode::decodeBuffer(std::vector<T*>* inBuffer, std::vector<T*>* outBuffer, int inputPoints, int bufferSize)
{
	beginBuffer();

	T sample = 0;
	int offset = 0;

	std::vector<float> startVolumes = decodeCoeffs(bufferSize, 0);
	std::vector<float> endVolumes = decodeCoeffs(bufferSize, bufferSize);
	std::vector<float> volumes(inBuffer->size() * 2);

	float* startVol = startVolumes.data();
	float* endVol = endVolumes.data();
	float* vol = volumes.data();

	for (size_t i = 0; i < bufferSize; i++) {
		float lerp = float(i) / bufferSize;
		for (size_t c = 0; c < volumes.size(); c++) {
			vol[c] = startVol[c] * (1 - lerp) + endVol[c] * lerp;
		}

		for (size_t c = 0; c < outBuffer->size(); c++) {
			sample = 0;
			offset = c < inputPoints ? c : 0;
			for (size_t k = 0; k < inBuffer->size(); k++)
			{
				sample += inBuffer->operator[](k * inputPoints + offset)[i] * vol[k * 2 + c];
			}
			outBuffer->operator[](c)[i] = sample;
		}
	}

	endBuffer();
}
