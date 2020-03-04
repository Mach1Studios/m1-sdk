//  Mach1 SDK
//  Copyright © 2018 Mach1. All rights reserved.
//  

/*
Positional 3D Coords

    X+ = strafe right
    X- = strafe left
    Y+ = up
    Y- = down
    Z+ = forward
    Z- = backward
 */

#include "Mach1EncodeCore.h"
#include <math.h>
#include <algorithm>
#include <iostream>
#include <map>
#include <vector>

float clamp(float n, float lower, float upper) {
	return std::max(lower, std::min(n, upper));
}

M1EncodeCorePointResults::M1EncodeCorePointResults() {
	for (int i = 0; i < MAX_POINTS_COUNT; i++) {
		pointsNames[i] = "";
		ppoints[i].x = 0;
		ppoints[i].y = 0;
		ppoints[i].z = 0;
	}
	pointsCount = 0;
}

M1EncodeCorePointResults::~M1EncodeCorePointResults() {

}

std::vector<M1EncodeCorePoint> M1EncodeCorePointResults::getPoints() {
	return std::vector<M1EncodeCorePoint>(ppoints, std::end(ppoints));
}

std::vector<std::vector<float>> M1EncodeCorePointResults::getGains() {
	return gains;
}

std::vector<std::string> M1EncodeCorePointResults::getPointsNames() {
	return std::vector<std::string>(pointsNames, std::end(pointsNames));
}

std::vector<float> M1EncodeCorePointResults::getGainsForInputChannelNamed(std::string pointName) {
	for (int i = 0; i < pointsCount; i++) {
		if (pointsNames[i] == pointName)
			return gains[i];
	}

	std::vector<float> result;
	if (gains.size() > 0) {
		result.resize(gains[0].size());
	}
	return result;
}

int M1EncodeCorePointResults::getPointsCount()
{
	return pointsCount;
}

void M1EncodeCore::processGains4Channels(float x, float y, float(&result)[4]) {
	result[2] = (1.0f - x) * (1.0f - y);
	result[0] = x * (1.0f - y);
	result[1] = x * y;
	result[3] = (1.0f - x) * y;
}

void M1EncodeCore::processGains8Channels(float x, float y, float z, float(&result)[8]) {
	result[2] = (1.0f - x) * (1.0f - y) * z;
	result[0] = x * (1.0f - y) * z;
	result[1] = x * y * z;
	result[3] = (1.0f - x) * y * z;

	result[6] = (1.0f - x) * (1.0f - y) * (1.0f - z);
	result[4] = x * (1.0f - y) * (1.0f - z);
	result[5] = x * y * (1.0f - z);
	result[7] = (1.0f - x) * y * (1.0f - z);
}

M1EncodeCore::M1EncodeCore() {
	inputMode = InputMode::INPUT_MONO;
	outputMode = OutputMode::OUTPUT_SPATIAL_8CH;

	rotation = 0;
	diverge = 0;
	pitch = 0;

	sRotate = 0;
	sSpread = 0;
	autoOrbit = false;
	isotropicEncode = true; 

	ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	timeLastCalculation = 0;

	// first init
	if (arr_Points == nullptr) {
		arr_Points = new M1EncodeCorePoint[MAX_POINTS_COUNT];
	}

	if (arr_Gains == nullptr) {
		arr_Gains = new float*[MAX_CHANNELS_COUNT];
		for (int i = 0; i < MAX_CHANNELS_COUNT; i++) {
			arr_Gains[i] = new float[MAX_CHANNELS_COUNT];
		}
	}

	if (arr_PointsNames == nullptr) {
		arr_PointsNames = new char*[MAX_CHANNELS_COUNT];
		for (int i = 0; i < MAX_CHANNELS_COUNT; i++) {
			arr_PointsNames[i] = new char[255];
			arr_PointsNames[i][0] = '\0';
		}
	}

	if (arr_GainsForInputChannelNamed == nullptr) {
		arr_GainsForInputChannelNamed = new float[MAX_CHANNELS_COUNT];
	}

    if (arr_ResultingCoeffsDecoded == nullptr) {
        arr_ResultingCoeffsDecoded = new float[MAX_POINTS_COUNT * 2];
    }
}

M1EncodeCore::~M1EncodeCore() {
	if (arr_Points != nullptr) {
		delete[] arr_Points;
	}

	if (arr_Gains != nullptr) {
		for (int i = 0; i < MAX_CHANNELS_COUNT; i++) {
			delete[] arr_Gains[i];
		}
		delete[] arr_Gains;
	}

	if (arr_PointsNames != nullptr) {
		for (int i = 0; i < MAX_CHANNELS_COUNT; i++) {
			delete[] arr_PointsNames[i];
		}
		delete[] arr_PointsNames;
	}

	if (arr_GainsForInputChannelNamed != nullptr) {
		delete[] arr_GainsForInputChannelNamed;
	}

	if (arr_ResultingCoeffsDecoded != nullptr) {
		delete[] arr_ResultingCoeffsDecoded;
	}
}

void M1EncodeCore::generatePointResults() {
	long tStart = getCurrentTime();

	float normalisedOutputDiverge = diverge * (1 / cos(PI * 0.25f));
	M1EncodeCorePoint centerpoint = { cos((rotation)* PI * 2) * normalisedOutputDiverge, 0, sin((rotation)* PI * 2) * normalisedOutputDiverge };

	if (inputMode == INPUT_MONO) {

		resultingPoints.pointsCount = 1;
		resultingPoints.pointsNames[0] = "M";

		if (outputMode == OUTPUT_HORIZON_4CH) {
			resultingPoints.ppoints[0] = { centerpoint.x, 0, centerpoint.z };
		}
		else
		{
			if (isotropicEncode) {
				resultingPoints.ppoints[0] = { centerpoint.x * sin((-pitch + 1) * PI /2), cos((-pitch + 1) * PI / 2) * normalisedOutputDiverge, centerpoint.z * sin((-pitch + 1) * PI / 2) };
			}
			else {
				resultingPoints.ppoints[0] = { centerpoint.x, pitch, centerpoint.z };
			}
		}
	}
	else if (inputMode == INPUT_STEREO) {

		float sRotationInRadians;
		if (autoOrbit) {
			sRotationInRadians = rotation * PI * 2 - PI / 2;
		}
		else {
			sRotationInRadians = sRotate * DEG_TO_RAD - PI / 2;
		}

		resultingPoints.pointsCount = 2;

		std::vector<std::string> names = { "L", "R" };
		std::vector<M1EncodeCorePoint> pnts = {
			{ centerpoint.x + cos((sRotationInRadians)) * sSpread, pitch, centerpoint.z + sin((sRotationInRadians)) * sSpread },
			{ centerpoint.x - cos((sRotationInRadians)) * sSpread, pitch, centerpoint.z - sin((sRotationInRadians)) * sSpread }
		};

		for (int i = 0; i < resultingPoints.pointsCount; i++)
		{
			resultingPoints.pointsNames[i] = names[i];
			resultingPoints.ppoints[i] = pnts[i];
			if (outputMode == OUTPUT_HORIZON_4CH) {
				resultingPoints.ppoints[i].y = 0;
			}
		}
	}
	else if (inputMode == INPUT_QUAD) {

		resultingPoints.pointsCount = 4;

		std::vector<std::string> names = { "L", "R", "Rs", "Ls" };
		std::vector<M1EncodeCorePoint> pnts = {
			{ cos((rotation + 0.125f - 0.25f) * PI * 2) * normalisedOutputDiverge, pitch, sin((rotation + 0.125f - 0.25f) * PI * 2) * normalisedOutputDiverge },
			{ cos((rotation + 0.125f) * PI * 2) * normalisedOutputDiverge, pitch, sin((rotation + 0.125f) * PI * 2) * normalisedOutputDiverge },
			{ cos((rotation + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge, pitch, sin((rotation + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge },
			{ cos((rotation + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge, pitch, sin((rotation + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge },
		};

		for (int i = 0; i < resultingPoints.pointsCount; i++)
		{
			resultingPoints.pointsNames[i] = names[i];
			resultingPoints.ppoints[i] = pnts[i];
			if (outputMode == OUTPUT_HORIZON_4CH) {
				resultingPoints.ppoints[i].y = 0;
			}
		}
	}
	else if (inputMode == INPUT_LCRS) {

		resultingPoints.pointsCount = 4;

		std::vector<std::string> names = { "L", "C", "R", "S" };
		std::vector<M1EncodeCorePoint> pnts = {
			{ cos((rotation - 0.125f) * PI * 2) * normalisedOutputDiverge, pitch, sin((rotation - 0.125f) * PI * 2) * normalisedOutputDiverge },
			{ 0, 0, 0 },
			{ cos((rotation + 0.125f) * PI * 2) * normalisedOutputDiverge, pitch, sin((rotation + 0.125f) * PI * 2) * normalisedOutputDiverge },
			{ 0, 0, 0 },
		};
		pnts[1] = (pnts[0] + pnts[2]) / 2;
		pnts[3] = -pnts[1];

		for (int i = 0; i < resultingPoints.pointsCount; i++)
		{
			resultingPoints.pointsNames[i] = names[i];
			resultingPoints.ppoints[i] = pnts[i];
			if (outputMode == OUTPUT_HORIZON_4CH) {
				resultingPoints.ppoints[i].y = 0;
			}
		}

	}
	else if (inputMode == INPUT_AFORMAT) {

		resultingPoints.pointsCount = 4;

		std::vector<std::string> names = { "FLU", "FRD", "BLD", "BRU" };
		std::vector<M1EncodeCorePoint> pnts = {
			{ cos((rotation + 0.125f - 0.25f) * PI * 2) * diverge, (1 * diverge), sin((rotation + 0.125f - 0.25f) * PI * 2) * diverge },
			{ cos((rotation + 0.125f) * PI * 2) * diverge, (-1 * diverge), sin((rotation + 0.125f) * PI * 2) * diverge },
			{ cos((rotation + 0.125f + 0.5f) * PI * 2) * diverge, (-1 * diverge), sin((rotation + 0.125f + 0.5f) * PI * 2) * diverge },
			{ cos((rotation + 0.125f + 0.25f) * PI * 2) * diverge, (1 * diverge), sin((rotation + 0.125f + 0.25f) * PI * 2) * diverge },
		};

		for (int i = 0; i < resultingPoints.pointsCount; i++)
		{
			resultingPoints.pointsNames[i] = names[i];
			resultingPoints.ppoints[i] = pnts[i];
			if (outputMode == OUTPUT_HORIZON_4CH) {
				resultingPoints.ppoints[i].y = 0;
			}
		}

	}
	else if (inputMode == INPUT_BFORMAT || inputMode == INPUT_1OAACN) { // duplicate?

		resultingPoints.pointsCount = 7;

		std::vector<std::string> names = { "W","1","2","3","-1","-2","-3" };
		std::vector<M1EncodeCorePoint> pnts = {
			{ 0, 0, 0 },  // W
			{ cos((rotation + 0.250f) * PI * 2) * diverge, 0, sin((rotation + 0.250f) * PI * 2) * diverge }, // Left/Right / L
			{ 0, 1.0f * diverge, 0 }, // Top/Bottom / T
			{ cos((rotation + 0.0f) * PI * 2) * diverge, 0, sin((rotation + 0.0f) * PI * 2) * diverge }, // Front/Back / F
			{ cos((rotation + 0.75f) * PI * 2) * diverge, 0, sin((rotation + 0.75f) * PI * 2) * diverge }, // -Left/Right / R
			{ 0, -1.0f * diverge, 0 }, // -Top/Bottom / B
			{ cos((rotation + 0.5f) * PI * 2) * diverge, 0, sin((rotation + 0.5f) * PI * 2) * diverge }, // -Front/Back / B
		};

		/*
		X = left/right angle
		Y = up/down angle
		Z = front/back angle
		 */

		for (int i = 0; i < resultingPoints.pointsCount; i++)
		{
			resultingPoints.pointsNames[i] = names[i];
			resultingPoints.ppoints[i] = pnts[i];
			if (outputMode == OUTPUT_HORIZON_4CH) { 
				resultingPoints.ppoints[i].y = 0;
			}
		}

	}
	else if (inputMode == INPUT_1OAFUMA) {

		resultingPoints.pointsCount = 7;

		std::vector<std::string> names = { "W","X","Y","Z","-X","-Y","-Z" };
		std::vector<M1EncodeCorePoint> pnts = {
			{ 0, 0, 0 }, // W
			{ cos((rotation + 0.0f) * PI * 2) * diverge, 0, sin((rotation + 0.0f) * PI * 2) * diverge }, // Front/Back / F
			{ cos((rotation + 0.25f) * PI * 2) * diverge, 0, sin((rotation + 0.25f) * PI * 2) * diverge }, // Left/Right / L
			{ 0, 1.0f * diverge, 0 },  // Top/Bottom / T
			{ cos((rotation + 0.5f) * PI * 2) * diverge, 0, sin((rotation + 0.5f) * PI * 2) * diverge }, // -Front/Back / B
			{ cos((rotation + 0.75f) * PI * 2) * diverge, 0, sin((rotation + 0.75f) * PI * 2) * diverge }, // -Left/Right / R
			{ 0, -1.0f * diverge, 0 }, // -Top/Bottom / B
		};

		for (int i = 0; i < resultingPoints.pointsCount; i++)
		{
			resultingPoints.pointsNames[i] = names[i];
			resultingPoints.ppoints[i] = pnts[i];
			if (outputMode == OUTPUT_HORIZON_4CH) {
				resultingPoints.ppoints[i].y = 0;
			}
		}
	}
	else if (inputMode == INPUT_2OAACN || inputMode == INPUT_2OAFUMA || inputMode == INPUT_3OAACN || inputMode == INPUT_3OAFUMA) { // duplicate?

		/*
		TODO: Rework this into something smarter
		Currently expects 2OA and 3OA conversion to Mach1Spatial8 externally from this API
		This API will just supply the rotations for Mach1Spatial8 Cuboid
		*/

					 
		resultingPoints.pointsCount = 8;

		std::vector<std::string> names = { "1","2","3","4","5","6","7","8" };
		std::vector<M1EncodeCorePoint> pnts = {
			{ cos((rotation + 0.125f - 0.25f) * PI * 2) * normalisedOutputDiverge, 1.0f * diverge, sin((rotation + 0.125f - 0.25f) * PI * 2) * normalisedOutputDiverge },
			{ cos((rotation + 0.125f) * PI * 2) * normalisedOutputDiverge, 1.0f * diverge, sin((rotation + 0.125f) * PI * 2) * normalisedOutputDiverge },
			{ cos((rotation + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge, 1.0f * diverge, sin((rotation + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge },
			{ cos((rotation + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge, 1.0f * diverge, sin((rotation + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge },
			{ cos((rotation + 0.125f - 0.25f) * PI * 2) * normalisedOutputDiverge, -1.0f * diverge, sin((rotation + 0.125f - 0.25f) * PI * 2) * normalisedOutputDiverge },
			{ cos((rotation + 0.125f) * PI * 2) * normalisedOutputDiverge, -1.0f * diverge, sin((rotation + 0.125f) * PI * 2) * normalisedOutputDiverge },
			{ cos((rotation + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge, -1.0f * diverge, sin((rotation + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge },
			{ cos((rotation + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge, -1.0f * diverge, sin((rotation + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge },
		};

		for (int i = 0; i < resultingPoints.pointsCount; i++)
		{
			resultingPoints.pointsNames[i] = names[i];
			resultingPoints.ppoints[i] = pnts[i];
			if (outputMode == OUTPUT_HORIZON_4CH) {
				resultingPoints.ppoints[i].y = 0;
			}
		}
	}
	else if (inputMode == INPUT_LCR) {

		resultingPoints.pointsCount = 3;

		std::vector<std::string> names = { "L","C","R" };
		std::vector<M1EncodeCorePoint> pnts = {
			{ cos((rotation - 0.125f) * PI * 2) * normalisedOutputDiverge, pitch, sin((rotation - 0.125f) * PI * 2) * normalisedOutputDiverge },
			{ 0, 0, 0 },
			{ cos((rotation + 0.125f) * PI * 2) * normalisedOutputDiverge, pitch, sin((rotation + 0.125f) * PI * 2) * normalisedOutputDiverge },
		};
		pnts[1] = (pnts[0] + pnts[2]) / 2;

		for (int i = 0; i < resultingPoints.pointsCount; i++)
		{
			resultingPoints.pointsNames[i] = names[i];
			resultingPoints.ppoints[i] = pnts[i];
			if (outputMode == OUTPUT_HORIZON_4CH) {
				resultingPoints.ppoints[i].y = 0;
			}
		}
	}

	for (int i = 0; i < resultingPoints.pointsCount; i++)
	{
		// Fixing it if we got outside the bounds
		resultingPoints.ppoints[i].x = clamp(resultingPoints.ppoints[i].x, -1, 1);
		resultingPoints.ppoints[i].y = clamp(resultingPoints.ppoints[i].y, -1, 1);
		resultingPoints.ppoints[i].z = clamp(resultingPoints.ppoints[i].z, -1, 1);
	}


	// Generating channel gains

	resultingPoints.gains.resize(resultingPoints.pointsCount);
	for (int i = 0; i < resultingPoints.pointsCount; i++) {

		resultingPoints.ppoints[i].x = resultingPoints.ppoints[i].x / 2 + 0.5f;
		resultingPoints.ppoints[i].y = resultingPoints.ppoints[i].y / 2 + 0.5f;
		resultingPoints.ppoints[i].z = resultingPoints.ppoints[i].z / 2 + 0.5f;

		resultingPoints.gains[i].resize(getOutputChannelsCount());

		// Generating gains for 4 channel output
		if (outputMode == OUTPUT_HORIZON_4CH) {
			float gains[4];
			processGains4Channels(resultingPoints.ppoints[i].x,
				resultingPoints.ppoints[i].z, gains);
			for (int j = 0; j < getOutputChannelsCount(); j++) {
				resultingPoints.gains[i][j] = gains[j];
			}
		}

		// Generating gains for 8 channel output
		if (outputMode == OUTPUT_SPATIAL_8CH) {
			float gains[8];
			processGains8Channels(resultingPoints.ppoints[i].x,
				resultingPoints.ppoints[i].z,
				resultingPoints.ppoints[i].y, gains);
			for (int j = 0; j < getOutputChannelsCount(); j++) {
				resultingPoints.gains[i][j] = gains[j];
			}
		}
	}

	timeLastCalculation = getCurrentTime() - tStart;

}


void M1EncodeCore::getResultingCoeffsDecoded(Mach1DecodeAlgoType decodeType, float* decodeResult, float* result)
{
    // clear
    for (int i = 0; i < resultingPoints.pointsCount * 2; i++) result[i] = 0;


    int decodeResultSize = 0;
    switch (decodeType)
    {
    case Mach1DecodeAlgoSpatial:
        decodeResultSize = 16;
        break;
    case Mach1DecodeAlgoAltSpatial:
        decodeResultSize = 16;
        break;
    case Mach1DecodeAlgoHorizon:
        decodeResultSize = 8;
        break;
    case Mach1DecodeAlgoHorizonPairs:
        decodeResultSize = 8;
        break;
    case Mach1DecodeAlgoSpatialPairs:
        decodeResultSize = 8;
        break;
    case Mach1DecodeAlgoSpatialPlus:
        decodeResultSize = 12;
        break;
    case Mach1DecodeAlgoSpatialPlusPlus:
        decodeResultSize = 14;
        break;
    case Mach1DecodeAlgoSpatialExt:
        decodeResultSize = 16;
        break;
    case Mach1DecodeAlgoSpatialExtPlus:
        decodeResultSize = 18;
        break;
    default:
        break;
    }

    // decode - 8, 16
    if (getOutputChannelsCount() * 2 != decodeResultSize) {
        std::cout << "This encode type is not suitable for decode type!" << std::endl;
    }

    for (int j = 0; j < resultingPoints.pointsCount; j++) {
        for (int i = 0; i < getOutputChannelsCount(); i++) {
            result[j * 2 + 0] += decodeResult[i * 2 + 0] * resultingPoints.gains[j][i]; // left
            result[j * 2 + 1] += decodeResult[i * 2 + 1] * resultingPoints.gains[j][i]; // right
        }
    }
}

[[deprecated]]
void M1EncodeCore::getResultingVolumesDecoded(Mach1DecodeAlgoType decodeType, float* decodeResult, float* result)
{
	int decodeResultSize = 0;
	switch (decodeType)
	{
	case Mach1DecodeAlgoSpatial:
		decodeResultSize = 16;
		break;
	case Mach1DecodeAlgoAltSpatial:
		decodeResultSize = 16;
		break;
	case Mach1DecodeAlgoHorizon:
		decodeResultSize = 8;
		break;
	case Mach1DecodeAlgoHorizonPairs:
		decodeResultSize = 8;
		break;
	case Mach1DecodeAlgoSpatialPairs:
		decodeResultSize = 8;
		break;
	case Mach1DecodeAlgoSpatialPlus:
		decodeResultSize = 12;
		break;
	case Mach1DecodeAlgoSpatialPlusPlus:
		decodeResultSize = 14;
		break;
    case Mach1DecodeAlgoSpatialExt:
        decodeResultSize = 16;
        break;
    case Mach1DecodeAlgoSpatialExtPlus:
        decodeResultSize = 18;
        break;
	default:
		break;
	}

	// clear
	for (int i = 0; i < decodeResultSize; i++) result[i] = 0;

	// decode - 8, 16
	if (getOutputChannelsCount() * 2 != decodeResultSize) {
		std::cout << "This encode type is not suitable for decode type!" << std::endl;
	}

	for (int j = 0; j < resultingPoints.pointsCount; j++) {
		for (int i = 0; i < getOutputChannelsCount(); i++) {
			result[j * 2 + 0] += decodeResult[i * 2 + 0] * resultingPoints.gains[j][i]; // left
			result[j * 2 + 1] += decodeResult[i * 2 + 1] * resultingPoints.gains[j][i]; // right
		}
	}
}

M1EncodeCore::InputMode M1EncodeCore::getInputMode() {
	return inputMode;
}

M1EncodeCore::OutputMode M1EncodeCore::getOutputMode() {
	return outputMode;
}

int M1EncodeCore::getOutputChannelsCount() {
	switch (outputMode) {
		case OUTPUT_HORIZON_4CH: return 4;
		case OUTPUT_SPATIAL_8CH: return 8;
		case OUTPUT_SPATIALPLUS_12CH: return 12;
		case OUTPUT_SPATIALPLUSPLUS_14CH: return 14;
		case OUTPUT_SPATIALEXT_16CH: return 16;
		case OUTPUT_SPATIALEXTPLUS_18CH: return 18;
	}
	return 0;
}

void M1EncodeCore::setInputMode(InputMode inputMode) {
	this->inputMode = inputMode;
}

void M1EncodeCore::setOutputMode(OutputMode outputMode) {
	this->outputMode = outputMode;
}

void M1EncodeCore::setRotation(float rotation) {
	this->rotation = rotation;
}

void M1EncodeCore::setDiverge(float diverge) {
	this->diverge = diverge;
}

void M1EncodeCore::setPitch(float pitch) {
	this->pitch = pitch;
}

void M1EncodeCore::setStereoRotate(float sRotate) {
	this->sRotate = sRotate;
}

void M1EncodeCore::setStereoSpread(float sSpread) {
	this->sSpread = sSpread;
}

void M1EncodeCore::setAutoOrbit(bool autoOrbit) {
	this->autoOrbit = autoOrbit;
}

void M1EncodeCore::setIsotropicEncode(bool isotropicEncode){
	this->isotropicEncode = isotropicEncode;
}

long M1EncodeCore::getCurrentTime()
{
	return (long)(duration_cast<milliseconds>(system_clock::now().time_since_epoch()) - ms).count();
}

long M1EncodeCore::getLastCalculationTime()
{
	return timeLastCalculation;
}