//  Mach1 SDK
//  Copyright © 2018 Mach1. All rights reserved.
//
//

#include "Mach1EncodeCore.h"
#include "Mach1EncodeCAPI.h"

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

	// clear
	for (int i = 0; i < 8; i++) {
		((M1EncodeCore*)M1obj)->arr_Points[i] = M1EncodeCorePoint();
	}

	for (int i = 0; i < vec.size(); i++) memcpy(&(((M1EncodeCore*)M1obj)->arr_Points)[i], &vec[i], sizeof(M1EncodeCorePoint));

	return ((M1EncodeCore*)M1obj)->arr_Points;
}

void* Mach1EncodeCAPI_getGains(void* M1obj) {
	std::vector<std::vector<float>> vec = ((M1EncodeCore*)M1obj)->resultingPoints.getGains();

	// clear
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			((M1EncodeCore*)M1obj)->arr_Gains[i][j] = 0;
		}
	}

	for (int i = 0; i < vec.size(); i++) memcpy(((M1EncodeCore*)M1obj)->arr_Gains[i], vec[i].data(), vec[i].size() * sizeof(float));

	return ((M1EncodeCore*)M1obj)->arr_Gains;
}

void* Mach1EncodeCAPI_getPointsNames(void* M1obj) {
	std::vector<std::string> vec = ((M1EncodeCore*)M1obj)->resultingPoints.getPointsNames();

	// clear
	for (int i = 0; i < 8; i++) {
		((M1EncodeCore*)M1obj)->arr_PointsNames[i][0] = '\0';
	}

	for (int i = 0; i < vec.size(); i++) memcpy(((M1EncodeCore*)M1obj)->arr_PointsNames[i], vec[i].c_str(), (vec[i].length() + 1) * sizeof(char));

	return ((M1EncodeCore*)M1obj)->arr_PointsNames;
}

void* Mach1EncodeCAPI_getGainsForInputChannelNamed(void* M1obj, char * pointName) {
	std::vector<float> vec = ((M1EncodeCore*)M1obj)->resultingPoints.getGainsForInputChannelNamed(pointName);

	// clear
	for (int i = 0; i < 8; i++) {
		((M1EncodeCore*)M1obj)->arr_GainsForInputChannelNamed[i] = 0;
	}

	for (int i = 0; i < vec.size(); i++) memcpy(&((M1EncodeCore*)M1obj)->arr_GainsForInputChannelNamed[i], &vec[i], sizeof(float));

	return ((M1EncodeCore*)M1obj)->arr_GainsForInputChannelNamed;
}

void Mach1EncodeCAPI_generatePointResults(void* M1obj) {
	((M1EncodeCore*)M1obj)->generatePointResults();
}

int Mach1EncodeCAPI_getPointsCount(void * M1obj)
{
	return ((M1EncodeCore*)M1obj)->resultingPoints.getPointsCount();
}

void* Mach1EncodeCAPI_getResultingCoeffsDecoded(void * M1obj, enum Mach1DecodeAlgoType decodeType, float * decodeResult)
{
	// clear
	for (int i = 0; i < MAX_POINTS_COUNT * 2; i++) {
		((M1EncodeCore*)M1obj)->arr_ResultingCoeffsDecoded[i] = 0;
	}
	
	((M1EncodeCore*)M1obj)->getResultingCoeffsDecoded(decodeType, decodeResult, ((M1EncodeCore*)M1obj)->arr_ResultingCoeffsDecoded);

	return ((M1EncodeCore*)M1obj)->arr_ResultingCoeffsDecoded;
}

enum Mach1EncodeInputModeType Mach1EncodeCAPI_getInputMode(void* M1obj) {
	return (Mach1EncodeInputModeType)(int)((M1EncodeCore*)M1obj)->getInputMode();
}

enum Mach1EncodeOutputModeType Mach1EncodeCAPI_getOutputMode(void* M1obj) {
	return (Mach1EncodeOutputModeType)(int)((M1EncodeCore*)M1obj)->getOutputMode();
}

int Mach1EncodeCAPI_getOutputChannelsCount(void* M1obj) {
	return ((M1EncodeCore*)M1obj)->getOutputChannelsCount();
}

void Mach1EncodeCAPI_setInputMode(void* M1obj, enum Mach1EncodeInputModeType inputMode) {
	((M1EncodeCore*)M1obj)->setInputMode(static_cast<M1EncodeCore::InputMode>(inputMode));
}

void Mach1EncodeCAPI_setOutputMode(void* M1obj, enum Mach1EncodeOutputModeType outputMode) {
	((M1EncodeCore*)M1obj)->setOutputMode(static_cast<M1EncodeCore::OutputMode>(outputMode));
}

void Mach1EncodeCAPI_setRotationDegrees(void* M1obj, float rotation) {
	((M1EncodeCore*)M1obj)->setRotationDegrees(rotation);
}

void Mach1EncodeCAPI_setRotation0to1(void* M1obj, float rotation) {
	((M1EncodeCore*)M1obj)->setRotation0to1(rotation);
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

long Mach1EncodeCAPI_getLastCalculationTime(void* M1obj) {
	return ((M1EncodeCore*)M1obj)->getLastCalculationTime();
}

/* DEPRECATED START*/
void* Mach1EncodeCAPI_getResultingVolumesDecoded(void * M1obj, enum Mach1DecodeAlgoType decodeType, float * decodeResult)
{
	// clear
	for (int i = 0; i < MAX_POINTS_COUNT * 2; i++) {
		((M1EncodeCore*)M1obj)->arr_ResultingCoeffsDecoded[i] = 0;
	}
	
	((M1EncodeCore*)M1obj)->getResultingVolumesDecoded(decodeType, decodeResult, ((M1EncodeCore*)M1obj)->arr_ResultingCoeffsDecoded);

	return ((M1EncodeCore*)M1obj)->arr_ResultingCoeffsDecoded;
}
/* DEPRECATED END */
