//  Mach1 SDK
//  Copyright © 2018 Mach1. All rights reserved.

#include "Mach1Encode.h"

Mach1Encode::Mach1Encode()
{
	M1obj = Mach1EncodeCAPI_create();
}

Mach1Encode::~Mach1Encode()
{
	Mach1EncodeCAPI_delete(M1obj);
}

std::vector<Mach1Point3D> Mach1Encode::getPoints()
{
	std::vector<Mach1Point3D> vec(Mach1EncodeCAPI_getPointsCount(M1obj));

	Mach1Point3D* arr = (Mach1Point3D*)Mach1EncodeCAPI_getPoints(M1obj);
	for (int i = 0; i < vec.size(); i++) vec[i] = arr[i];

	return vec;
}

std::vector<std::vector<float>> Mach1Encode::getGains()
{
	std::vector<std::vector<float>> vec(Mach1EncodeCAPI_getPointsCount(M1obj));

	float** arr = (float**)Mach1EncodeCAPI_getGains(M1obj);

	for (int i = 0; i < vec.size(); i++) {
		vec[i].resize(8);
		for (int j = 0; j < vec[i].size(); j++) {
			vec[i][j] = arr[i][j];
		}
	}

	return vec;
}

std::vector<std::string> Mach1Encode::getPointsNames()
{
	std::vector<std::string> vec(Mach1EncodeCAPI_getPointsCount(M1obj));

	char** arr = (char**)Mach1EncodeCAPI_getPointsNames(M1obj);

	for (int i = 0; i < vec.size(); i++) {
		vec[i] = arr[i];
	}

	return vec;
}

std::vector<float> Mach1Encode::getGainsForInputChannelNamed(std::string pointName)
{
	std::vector<float> vec(8);

	float* arr = (float*)Mach1EncodeCAPI_getGainsForInputChannelNamed(M1obj, (char*)pointName.c_str());

	for (int i = 0; i < vec.size(); i++) {
		vec[i] = arr[i];
	}

	return vec;
}

void Mach1Encode::generatePointResults()
{
	Mach1EncodeCAPI_generatePointResults(M1obj);
}

int Mach1Encode::getPointsCount()
{
	return Mach1EncodeCAPI_getPointsCount(M1obj);
}

void Mach1Encode::setInputMode(Mach1EncodeInputModeType inputMode)
{
	Mach1EncodeCAPI_setInputMode(M1obj, inputMode);
}

void Mach1Encode::setOutputMode(Mach1EncodeOutputModeType outputMode)
{
	Mach1EncodeCAPI_setOutputMode(M1obj, outputMode);
}

void Mach1Encode::setRotation(float rotation)
{
	Mach1EncodeCAPI_setRotation(M1obj, rotation);
}

void Mach1Encode::setDiverge(float diverge)
{
	Mach1EncodeCAPI_setDiverge(M1obj, diverge);
}

void Mach1Encode::setPitch(float pitch)
{
	Mach1EncodeCAPI_setPitch(M1obj, pitch);
}

void Mach1Encode::setStereoRotate(float sRotate)
{
	Mach1EncodeCAPI_setStereoRotate(M1obj, sRotate);
}

void Mach1Encode::setStereoSpread(float sSpread)
{
	Mach1EncodeCAPI_setStereoSpread(M1obj, sSpread);
}

void Mach1Encode::setAutoOrbit(bool autoOrbit)
{
	Mach1EncodeCAPI_setAutoOrbit(M1obj, autoOrbit);
}

void Mach1Encode::setIsotropicEncode(bool isotropicEncode)
{
	Mach1EncodeCAPI_setIsotropicEncode(M1obj, isotropicEncode);
}
