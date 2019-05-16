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

std::vector<float> Mach1Encode::getResultingVolumesDecoded(Mach1DecodeAlgoType decodeType, std::vector<float>& decodeResult)
{
	std::vector<float> vec(14);

	float* arr = (float*)Mach1EncodeCAPI_getResultingVolumesDecoded(M1obj, decodeType, decodeResult.data());

	for (int i = 0; i < vec.size(); i++) {
		vec[i] = arr[i];
	}

	return vec;
}

void Mach1Encode::setInputMode(Mach1EncodeInputModeType inputMode)
{
	Mach1EncodeCAPI_setInputMode(M1obj, inputMode);
	// Sets the number of input streams to be positioned as points
	//
	// - Parameters:
	//     - INPUT_MONO
	//     - INPUT_STEREO
	//     - INPUT_QUAD
	//     - INPUT_AFORMAT
	//     - INPUT_BFORMAT
}

void Mach1Encode::setOutputMode(Mach1EncodeOutputModeType outputMode)
{
	Mach1EncodeCAPI_setOutputMode(M1obj, outputMode);
	// Sets the output spatial format, Mach1Spatial or Mach1Horizon
	//
	// - Parameters:
	//     - OUTPUT_4CH (Mach1Horizon) [Yaw only]
	//     - OUTPUT_8CH (Mach1Spatial) [Yaw, Pitch, Roll]
}

void Mach1Encode::setRotation(float rotation)
{
	Mach1EncodeCAPI_setRotation(M1obj, rotation);
	// Sets the point(s) around the center origin of the vector space
	//
	// - Parameters:
	//     - value range: 0.0 -> 1.0 (0->360)
}

void Mach1Encode::setDiverge(float diverge)
{
	Mach1EncodeCAPI_setDiverge(M1obj, diverge);
	// Sets the point(s) to/from center origin of the vector space
	//
	// - Parameters:
	//     - value range: -1.0 -> 1.0
}

void Mach1Encode::setPitch(float pitch)
{
	Mach1EncodeCAPI_setPitch(M1obj, pitch);
	// Sets the point(s) up/down the vector space
	//
	// - Parameters:
	//     - value range: -1.0 -> 1.0 (-90->90)
}

void Mach1Encode::setStereoRotate(float sRotate)
{
	Mach1EncodeCAPI_setStereoRotate(M1obj, sRotate);
	// Sets the two stereo points around the axis of the center point between them
	//
	// - Parameters:
	//     - value range: -180.0->180.0
}

void Mach1Encode::setStereoSpread(float sSpread)
{
	Mach1EncodeCAPI_setStereoSpread(M1obj, sSpread);
	// Sets the space between the two stereo points
	//
	// - Parameters:
	//     - value range: 0.0 -> 1.0
}

void Mach1Encode::setAutoOrbit(bool autoOrbit)
{
	Mach1EncodeCAPI_setAutoOrbit(M1obj, autoOrbit);
	// Sets encoding behavior acts evenly with distribution
	// across all azimuth/rotation angles and all altitude/pitch angles
	//
	// Remark: Default is true
}

void Mach1Encode::setIsotropicEncode(bool isotropicEncode)
{
	Mach1EncodeCAPI_setIsotropicEncode(M1obj, isotropicEncode);
	// Sets both stereo points rotate in relation to the
	// center point between them so that they always triangulate
	// toward center of the cuboid
	//
	// Remark: Default is true
}
