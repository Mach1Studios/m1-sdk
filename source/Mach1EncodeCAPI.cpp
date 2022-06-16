//  Mach1 Spatial SDK
//  Copyright © 2017-2020 Mach1. All rights reserved.

#include "Mach1EncodeCore.h"
#include "Mach1EncodeCAPI.h"
#include <cstring>

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
	std::vector<Mach1Point3DCore> vec = ((M1EncodeCore*)M1obj)->resultingPoints.getPoints();

	// clear
	for (int i = 0; i < MAX_POINTS_COUNT; i++) {
		((M1EncodeCore*)M1obj)->arr_Points[i] = Mach1Point3DCore();
	}

	for (int i = 0; i < vec.size(); i++) std::memcpy(&(((M1EncodeCore*)M1obj)->arr_Points[i]), &vec[i], sizeof(Mach1Point3DCore));

	return ((M1EncodeCore*)M1obj)->arr_Points;
}

void* Mach1EncodeCAPI_getGains(void* M1obj) {
	std::vector<std::vector<float>> vec = ((M1EncodeCore*)M1obj)->resultingPoints.getGains();

	// clear
	for (int i = 0; i < MAX_POINTS_COUNT; i++) {
		for (int j = 0; j < MAX_CHANNELS_COUNT; j++) {
			((M1EncodeCore*)M1obj)->arr_Gains[i][j] = 0;
		}
	}

	for (int i = 0; i < vec.size(); i++) std::memcpy(((M1EncodeCore*)M1obj)->arr_Gains[i], vec[i].data(), vec[i].size() * sizeof(float));

	return ((M1EncodeCore*)M1obj)->arr_Gains;
}

void* Mach1EncodeCAPI_getPointsNames(void* M1obj) {
	std::vector<std::string> vec = ((M1EncodeCore*)M1obj)->resultingPoints.getPointsNames();

	// clear
	for (int i = 0; i < MAX_POINTS_COUNT; i++) {
		((M1EncodeCore*)M1obj)->arr_PointsNames[i][0] = '\0';
	}

	for (int i = 0; i < vec.size(); i++) std::memcpy(((M1EncodeCore*)M1obj)->arr_PointsNames[i], vec[i].c_str(), (vec[i].length() + 1) * sizeof(char));

	return ((M1EncodeCore*)M1obj)->arr_PointsNames;
}

void* Mach1EncodeCAPI_getGainsForInputChannelNamed(void* M1obj, char * pointName) {
	std::vector<float> vec = ((M1EncodeCore*)M1obj)->resultingPoints.getGainsForInputChannelNamed(pointName);

	// clear
	for (int i = 0; i < MAX_CHANNELS_COUNT; i++) {
		((M1EncodeCore*)M1obj)->arr_GainsForInputChannelNamed[i] = 0;
	}

	for (int i = 0; i < vec.size(); i++) std::memcpy(&((M1EncodeCore*)M1obj)->arr_GainsForInputChannelNamed[i], &vec[i], sizeof(float));

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
	for (int i = 0; i < MAX_CHANNELS_COUNT; i++) {
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

int Mach1EncodeCAPI_getInputChannelsCount(void* M1obj) {
	return ((M1EncodeCore*)M1obj)->getInputChannelsCount();
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

void Mach1EncodeCAPI_setAzimuth(void* M1obj, float azimuthFromMinus1To1) {
	((M1EncodeCore*)M1obj)->setAzimuth(azimuthFromMinus1To1);
}

void Mach1EncodeCAPI_setAzimuthDegrees(void* M1obj, float azimuthDegrees) {
	((M1EncodeCore*)M1obj)->setAzimuthDegrees(azimuthDegrees);
}

void Mach1EncodeCAPI_setAzimuthRadians(void* M1obj, float azimuthRadians) {
	((M1EncodeCore*)M1obj)->setAzimuthRadians(azimuthRadians);
}

void Mach1EncodeCAPI_setDiverge(void* M1obj, float divergeFromMinus1To1) {
	((M1EncodeCore*)M1obj)->setDiverge(divergeFromMinus1To1);
}

void Mach1EncodeCAPI_setElevation(void* M1obj, float elevationFromMinus1to1) {
	((M1EncodeCore*)M1obj)->setElevation(elevationFromMinus1to1);
}

void Mach1EncodeCAPI_setElevationDegrees(void* M1obj, float elevationFromMinus90to90) {
	((M1EncodeCore*)M1obj)->setElevationDegrees(elevationFromMinus90to90);
}

void Mach1EncodeCAPI_setElevationRadians(void* M1obj, float elevationFromMinusHalfPItoHalfPI) {
	((M1EncodeCore*)M1obj)->setElevationRadians(elevationFromMinusHalfPItoHalfPI);
}

void Mach1EncodeCAPI_setOrbitRotation(void* M1obj, float orbitRotationFromMinusOnetoOne) {
	((M1EncodeCore*)M1obj)->setOrbitRotation(orbitRotationFromMinusOnetoOne);
}

void Mach1EncodeCAPI_setOrbitRotationDegrees(void* M1obj, float orbitRotationDegrees) {
	((M1EncodeCore*)M1obj)->setOrbitRotationDegrees(orbitRotationDegrees);
}

void Mach1EncodeCAPI_setOrbitRotationRadians(void* M1obj, float orbitRotationRadians) {
	((M1EncodeCore*)M1obj)->setOrbitRotationRadians(orbitRotationRadians);
}

void Mach1EncodeCAPI_setStereoSpread(void* M1obj, float sSpreadFrom0to1) {
	((M1EncodeCore*)M1obj)->setStereoSpread(sSpreadFrom0to1);
}

void Mach1EncodeCAPI_setAutoOrbit(void* M1obj, bool autoOrbit) {
	((M1EncodeCore*)M1obj)->setAutoOrbit(autoOrbit);
}

void Mach1EncodeCAPI_setPannerMode(void* M1obj, enum Mach1EncodePannerMode pannerMode) {
	((M1EncodeCore*)M1obj)->setPannerMode(static_cast<M1EncodeCore::PannerMode>(pannerMode));
}

void Mach1EncodeCAPI_setFrontSurroundPerspective(void* M1obj, bool frontSurroundPerspective) {
	((M1EncodeCore*)M1obj)->setFrontSurroundPerspective(frontSurroundPerspective);
}

void Mach1EncodeCAPI_setOutputGain(void* M1obj, float outputGainMultipler, bool isDecibel) {
	((M1EncodeCore*)M1obj)->setOutputGain(outputGainMultipler, isDecibel);
}

long Mach1EncodeCAPI_getLastCalculationTime(void* M1obj) {
	return ((M1EncodeCore*)M1obj)->getLastCalculationTime();
}
