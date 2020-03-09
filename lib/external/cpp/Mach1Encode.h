//  Mach1 SDK
//  Copyright © 2018 Mach1. All rights reserved.
//
//  Header file

#pragma once

#include <vector>
#include <string>
#include "Mach1EncodeCAPI.h"
#include "Mach1DecodeCAPI.h"

class Mach1Encode {
private:
	void* M1obj;

public:
	Mach1Encode();
	~Mach1Encode();

	std::vector<Mach1Point3D> getPoints();
	std::vector<std::vector<float>> getGains();
	std::vector<std::string> getPointsNames();
	std::vector<float> getGainsForInputChannelNamed(std::string pointName);

	void generatePointResults();
	int getPointsCount();

	std::vector<float> getResultingVolumesDecoded(Mach1DecodeAlgoType decodeType, std::vector<float>& decodeResult);

	Mach1EncodeInputModeType getInputMode();
	Mach1EncodeOutputModeType getOutputMode();
	int getOutputChannelsCount();

	template<typename T>
	void encodeBuffer(std::vector<std::vector<T>>* inBuffer, std::vector<std::vector<T>>* outBuffer, int bufferSize);

	template<typename T>
	void encodeBuffer(std::vector<T*>* inBuffer, std::vector<T*>* outBuffer, int bufferSize);

	void setInputMode(Mach1EncodeInputModeType inputMode);
	void setOutputMode(Mach1EncodeOutputModeType outputMode);

#if __cplusplus > 201103L
	[[deprecated("setRotation is deprecated, please use setRotationDegrees instead")]]
#endif
	void setRotation(float rotation);
	
	void setRotationDegrees(float rotation);
	void setRotation0to1(float rotation);

	void setDiverge(float diverge);
	void setPitch(float pitch);
	void setStereoRotate(float sRotate);
	void setStereoSpread(float sSpread);
	void setAutoOrbit(bool autoOrbit);
	void setIsotropicEncode(bool isotropicEncode);
};

