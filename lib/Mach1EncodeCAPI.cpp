//  Mach1 SDK
//  Copyright © 2018 Mach1. All rights reserved.
//
//

#include "Mach1EncodeCAPI.h"
#include "Mach1EncodeCore.h"

#include <string.h>

using namespace std;

void* Mach1EncodeCAPI_create() {
	return new M1EncodeCore();
}

void Mach1EncodeCAPI_delete(void* M1obj) {
	if (M1obj != nullptr) {
		delete (M1EncodeCore*)M1obj;
		M1obj = nullptr;
	}
}

void* Mach1EncodeCAPI_getPoints(void* M1obj) {
	std::vector<M1EncodeCorePoint> vec = ((M1EncodeCore*)M1obj)->resultingPoints.getPoints();

	static M1EncodeCorePoint* arr = nullptr;

	// first init
	if (arr == nullptr) {
		arr = new M1EncodeCorePoint[7];
	}

	// clear
	for (int i = 0; i < 7; i++) {
		arr[i] = M1EncodeCorePoint();
	}

	for (int i = 0; i < vec.size(); i++) memcpy(&arr[i], &vec[i], sizeof(M1EncodeCorePoint));

	return arr;
}

void* Mach1EncodeCAPI_getGains(void* M1obj) {
	std::vector<std::vector<float>> vec = ((M1EncodeCore*)M1obj)->resultingPoints.getGains();

	static float** arr = nullptr;

	// first init
	if (arr == nullptr) {
		arr = new float*[7];
		for (int i = 0; i < 7; i++) {
			arr[i] = new float[8];
		}
	}

	// clear
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 8; j++) {
			arr[i][j] = 0;
		}
	}

	for (int i = 0; i < vec.size(); i++) memcpy(arr[i], vec[i].data(), vec[i].size() * sizeof(float));

	return arr;
}

void* Mach1EncodeCAPI_getPointsNames(void* M1obj) {
	std::vector<std::string> vec = ((M1EncodeCore*)M1obj)->resultingPoints.getPointsNames();

	static char** arr = nullptr;

	// first init
	if (arr == nullptr) {
		arr = new char*[7];
		for (int i = 0; i < 7; i++) {
			arr[i] = new char[255];
			arr[i][0] = '\0';
		}
	}

	// clear
	for (int i = 0; i < 7; i++) {
		arr[i][0] = '\0';
	}

	for (int i = 0; i < vec.size(); i++) memcpy(arr[i], vec[i].c_str(), (vec[i].length() + 1) * sizeof(char));

	return arr;
}

void* Mach1EncodeCAPI_getGainsForInputChannelNamed(void* M1obj, char * pointName) {
	std::vector<float> vec = ((M1EncodeCore*)M1obj)->resultingPoints.getGainsForInputChannelNamed(pointName);

	static float* arr = nullptr;

	// first init
	if (arr == nullptr) {
		arr = new float[7];
	}

	// clear
	for (int i = 0; i < 7; i++) {
		arr[i] = 0;
	}

	for (int i = 0; i < vec.size(); i++) memcpy(&arr[i], &vec[i], sizeof(float));

	return arr;
}

void Mach1EncodeCAPI_generatePointResults(void* M1obj) {
	((M1EncodeCore*)M1obj)->generatePointResults();
}

int Mach1EncodeCAPI_getPointsCount(void * M1obj)
{
	return ((M1EncodeCore*)M1obj)->resultingPoints.getPointsCount();
}

void* Mach1EncodeCAPI_getResultingVolumesDecoded(void * M1obj, enum Mach1DecodeAlgoType decodeType, float * decodeResult)
{
	static float* arr = nullptr;

	// first init
	if (arr == nullptr) {
		arr = new float[14];
	}

	// clear
	for (int i = 0; i < 14; i++) {
		arr[i] = 0;
	}
	
	((M1EncodeCore*)M1obj)->getResultingVolumesDecoded(decodeType, decodeResult, arr);

	return arr;
}

void Mach1EncodeCAPI_setInputMode(void* M1obj, enum Mach1EncodeInputModeType inputMode) {
	((M1EncodeCore*)M1obj)->setInputMode(static_cast<M1EncodeCore::InputMode>(inputMode));
}

void Mach1EncodeCAPI_setOutputMode(void* M1obj, enum Mach1EncodeOutputModeType outputMode) {
	((M1EncodeCore*)M1obj)->setOutputMode(static_cast<M1EncodeCore::OutputMode>(outputMode));
}

void Mach1EncodeCAPI_setRotation(void* M1obj, float rotation) {
	((M1EncodeCore*)M1obj)->setRotation(rotation);
}

void Mach1EncodeCAPI_setDiverge(void* M1obj, float diverge) {
	((M1EncodeCore*)M1obj)->setDiverge(diverge);
}

void Mach1EncodeCAPI_setPitch(void* M1obj, float pitch) {
	((M1EncodeCore*)M1obj)->setPitch(pitch);
}

void Mach1EncodeCAPI_setStereoRotate(void* M1obj, float sRotate) {
	((M1EncodeCore*)M1obj)->setStereoRotate(sRotate);
}

void Mach1EncodeCAPI_setStereoSpread(void* M1obj, float sSpread) {
	((M1EncodeCore*)M1obj)->setStereoSpread(sSpread);
}

void Mach1EncodeCAPI_setAutoOrbit(void* M1obj, bool autoOrbit) {
	((M1EncodeCore*)M1obj)->setAutoOrbit(autoOrbit);
}

void Mach1EncodeCAPI_setIsotropicEncode(void* M1obj, bool isotropicEncode) {
	((M1EncodeCore*)M1obj)->setIsotropicEncode(isotropicEncode);
}
