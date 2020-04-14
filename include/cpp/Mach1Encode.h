//  Mach1 Spatial SDK
//  Copyright © 2017-2020 Mach1. All rights reserved.

#pragma once

#include <vector>
#include <string>
#include "Mach1EncodeCAPI.h"
#include "Mach1DecodeCAPI.h"

class Mach1Encode {
private:
	void* M1obj;
	std::vector<std::vector<float>> gains;
	
public:
	Mach1Encode();
	~Mach1Encode();

	std::vector<Mach1Point3D> getPoints();
	std::vector<std::vector<float>> getGains();
	std::vector<std::string> getPointsNames();
	std::vector<float> getGainsForInputChannelNamed(std::string pointName);

	void generatePointResults();
	int getPointsCount();

#if __cplusplus > 201103L
	[[deprecated("getResultingVolumesDecoded is deprecated, please use getResultingCoeffsDecoded instead")]]
#endif
	std::vector<float> getResultingVolumesDecoded(Mach1DecodeAlgoType decodeType, std::vector<float>& decodeResult);
	std::vector<float> getResultingCoeffsDecoded(Mach1DecodeAlgoType decodeType, std::vector<float>& decodeResult);

	Mach1EncodeInputModeType getInputMode();
	Mach1EncodeOutputModeType getOutputMode();
	int getInputChannelsCount();
	int getOutputChannelsCount();

	template<typename T>
	void encodeBuffer(std::vector<std::vector<T>>* inBuffer, std::vector<std::vector<T>>* outBuffer, int bufferSize);

	template<typename T>
	void encodeBuffer(std::vector<T*>* inBuffer, std::vector<T*>* outBuffer, int bufferSize);

	void setInputMode(Mach1EncodeInputModeType inputMode);
	void setOutputMode(Mach1EncodeOutputModeType outputMode);

#if __cplusplus > 201103L
	[[deprecated("setRotation is deprecated due to ambiguity of use, please use setAzimuth0to1, setAzimuthDegrees or setAzimuthRadians instead")]]
#endif
	void setRotation(float rotationDegrees);
	void setAzimuth(float azimuthFromMinus1To1);
	void setAzimuthDegrees(float azimuthDegrees);
	void setAzimuthRadians(float azimuthRadians);

#if __cplusplus > 201103L
	[[deprecated("setPitch is deprecated due to ambiguity of use, please use setElevation0to1, setStereoRotationDegrees or setStereoRotationRadians instead")]]
#endif
	void setPitch(float pitchFromMinus90to90);
	void setElevation(float elevationFromMinus1to1);
	void setElevationDegrees(float elevationFromMinus90to90);
	void setElevationRadians(float elevationFromMinusHalfPItoHalfPI);

	void setIsotropicEncode(bool isotropicEncode);

#if __cplusplus > 201103L
	[[deprecated("setStereoRotate is deprecated due to ambiguity of use, please use setOrbitRotation0to1, setOrbitRotationDegrees or setOrbitRotationRadians instead")]]
#endif
	void setStereoRotate(float sRotateDegrees);
	void setOrbitRotation(float orbitRotationFromMinusOnetoOne);
	void setOrbitRotationDegrees(float orbitRotationDegrees);
	void setOrbitRotationRadians(float orbitRotationRadians);

	void setDiverge(float divergeFromMinus1To1);
	void setStereoSpread(float sSpreadFrom0to1);
	void setAutoOrbit(bool autoOrbit);
};

template<typename T>
inline void Mach1Encode::encodeBuffer(std::vector<std::vector<T>>* inBuffer, std::vector<std::vector<T>>* outBuffer, int bufferSize)
{
	std::vector<std::vector<float>> gains = getGains();
	if (this->gains.size() != gains.size()) this->gains = gains;

	T value;
	float prc = 0;
	float gain = 0;
	for (size_t c = 0; c < gains.size(); c++) {
		for (size_t k = 0; k < gains[c].size(); k++) {
			for (size_t i = 0; i < bufferSize; i++) {
				prc = 1.0 * i / bufferSize;
				gain = this->gains[c][k] * (1 - prc) + gains[c][k] * prc;
				outBuffer->operator[](k * getInputChannelsCount() + c)[i] = inBuffer->operator[](c)[i] * gain;
			}
		}
	}

	this->gains = gains;
}

template<typename T>
void Mach1Encode::encodeBuffer(std::vector<T*>* inBuffer, std::vector<T*>* outBuffer, int bufferSize)
{
	std::vector<std::vector<float>> gains = getGains();
	if (this->gains.size() != gains.size()) this->gains = gains;

	T value;
	float prc = 0;
	float gain = 0;
	for (size_t c = 0; c < gains.size(); c++) {
		for (size_t k = 0; k < gains[c].size(); k++) {
			for (size_t i = 0; i < bufferSize; i++) {
				prc = 1.0 * i / bufferSize;
				gain = this->gains[c][k] * (1 - prc) + gains[c][k] * prc;
				outBuffer->operator[](k * getInputChannelsCount() + c)[i] = inBuffer->operator[](c)[i] * gain;
			}
		}
	}

	this->gains = gains;
}
