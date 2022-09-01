//  Mach1 Spatial SDK
//  Copyright © 2017-2020 Mach1. All rights reserved.

/*
Internal Orientation Implementation:
	-  Azimuth[+] = rotate right 0-1 [Range: 0->360 | -180->180]
	-  Azimuth[-] = rotate left 0-1 [Range: 0->360 | -180->180]
	-  Elevation[+] = rotate up 0-1 [Range: -90->90]
	-  Elevation[-] = rotate down 0-1 [Range: -90->90]
	-  OrbitRotation[+] = rotate right 0-1 [Range: 0->360 | -180->180]
	-  OrbitRotation[-] = rotate left 0-1 [Range: 0->360 | -180->180]

Mach1EncodeCore normalizes all input ranges to an unsigned "0 to 1" range for Azimuth, Elevation and OrbitRotation.
 */

#include "Mach1EncodeCore.h"
#include <math.h>
#include <algorithm>
#include <iostream>
#include <map>
#include <vector>

float M1EncodeCore::clamp(float n, float lower, float upper) {
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

std::vector<Mach1Point3DCore> M1EncodeCorePointResults::getPoints() {
	return std::vector<Mach1Point3DCore>(ppoints, std::end(ppoints));
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

float M1EncodeCore::getCoeffForStandardPoint(float x, float y, float z, Mach1Point3DCore point, bool ignoreZ)
{
	// map from [-1,1] to [0,1]
	point.x = (point.x / (1 / 0.707) + 1) / 2;
	point.y = (point.y / (1 / 0.707) + 1) / 2;
	point.z = (point.z / (1 / 0.707) + 1) / 2;

	float dist = pow(pow(point.x - x, 2.0) + pow(point.y - y, 2.0) + pow(point.z - z, 2.0), 0.3);
	dist = clamp(1 - dist, 0, 1);
	dist = dist * dist * dist * dist; // apply easeInQuart

	// "pan law" experiment
	if (pannerMode == MODE_ISOTROPICEQUALPOWER){
		dist = sqrt(1 - pow(dist - 1, 2));
	}

	return dist;
}

std::vector<float> M1EncodeCore::getCoeffSetForStandardPointSet(float x, float y, float z, std::vector<Mach1Point3DCore>& pointSet, bool ignoreZ)
{
	std::vector<float> result;

	std::vector<Mach1Point3DCore> points = pointSet;
	float len = 0;
	
	// normalize cube
	for (auto& i : points) {
		len = (std::max)(len, i.length());
	}
	for (auto& i : points) {
		i = i / len;
	}

	for (auto &i : points) {
		result.push_back(getCoeffForStandardPoint(x, y, z, i, ignoreZ));
	}

	// normalize coeffs
	float c = 0;
	for (auto& i : result) {
		c += i;
	}

	for (auto& i : result) {
		i /= c;
	}

	return result;
}

void M1EncodeCore::processGainsChannels(float x, float y, float z, std::vector<float>& result) {

   /*
	* X = Left(-) to Right(+) (from a top down perspective)
	* Y = Front(+) to Back(-) (from a top down perspective)
	* Z = Top(+) to Bottom(-) (from a top down perspective) 
	*/

	// M1 horizon plane points
	static std::vector<Mach1Point3DCore> m1Horizon_4_Def = { 
												{-1, 1, 0},
												{1, 1, 0},
												{-1, -1, 0},
												{1, -1, 0} };

	// M1 spatial cube points
	static std::vector<Mach1Point3DCore> m1Spatial_8_Def = { 
												{-1, 1, 1},
												{1, 1, 1},
												{-1, -1, 1},
												{1, -1, 1},

												{-1, 1, -1},
												{1, 1, -1},
												{-1, -1, -1},
												{1, -1, -1} };

	// M1 spatial+ cube points
	static std::vector<Mach1Point3DCore> m1Spatial_12_Def = { 
												{-1, 1, 1},
												{1, 1, 1},
												{-1, -1, 1},
												{1, -1, 1},

												{-1, 1, -1},
												{1, 1, -1},
												{-1, -1, -1},
												{1, -1, -1},

												{0, 1 / 0.707, 0},
												{1 / 0.707, 0, 0},
												{0, -1 / 0.707, 0},
												{-1 / 0.707, 0, 0} };

	// M1 spatial++ cube points
	static std::vector<Mach1Point3DCore> m1Spatial_14_Def = { 
												{-1, 1, 1},
												{1, 1, 1},
												{-1, -1, 1},
												{1, -1, 1},

												{-1, 1, -1},
												{1, 1, -1},
												{-1, -1, -1},
												{1, -1, -1},

												{0, 1 / 0.707, 0},
												{1 / 0.707, 0, 0},
												{0, -1 / 0.707, 0},
												{-1 / 0.707, 0, 0},

												{ 1 / 0.707, 0, 0},
												{-1 / 0.707, 0, 0} };

	// M1 spatial extended cube points
	static std::vector<Mach1Point3DCore> m1Spatial_16_Def = { 
												{-1, 1, 1},
												{1, 1, 1},
												{-1, -1, 1},
												{1, -1, 1},

												{-1, 1, -1},
												{1, 1, -1},
												{-1, -1, -1},
												{1, -1, -1},

												{0, 1 / 0.707, 1},
												{1 / 0.707, 0, 1},
												{0, -1 / 0.707, 1},
												{-1 / 0.707, 0, 1},

												{0, 1 / 0.707, -1},
												{1 / 0.707, 0, -1},
												{0, -1 / 0.707, -1},
												{-1 / 0.707, 0, -1} };

	// M1 spatial extended+ cube points
	static std::vector<Mach1Point3DCore> m1Spatial_18_Def = { 
												{-1, 1, 1},
												{1, 1, 1},
												{-1, -1, 1},
												{1, -1, 1},

												{-1, 1, -1},
												{1, 1, -1},
												{-1, -1, -1},
												{1, -1, -1},

												{0, 1 / 0.707, 1},
												{1 / 0.707, 0, 1},
												{0, -1 / 0.707, 1},
												{-1 / 0.707, 0, 1},

												{0, 1 / 0.707, -1},
												{1 / 0.707, 0, -1},
												{0, -1 / 0.707, -1},
												{-1 / 0.707, 0, -1},

												{1 / 0.707, 0, 0},
												{-1 / 0.707, 0, 0} };

	static std::vector<Mach1Point3DCore> m1Spatial_20_Def = {
												{-1, 1, 1},
												{1, 1, 1},
												{-1, -1, 1},
												{1, -1, 1},

												{-1, 1, -1},
												{1, 1, -1},
												{-1, -1, -1},
												{1, -1, -1},

												{0, 1 / 0.707, 1},
												{1 / 0.707, 0, 1},
												{0, -1 / 0.707, 1},
												{-1 / 0.707, 0, 1},

												{0, 1 / 0.707, -1},
												{1 / 0.707, 0, -1},
												{0, -1 / 0.707, -1},
												{-1 / 0.707, 0, -1},

												{0, 1 / 0.707, 0},
												{1 / 0.707, 0, 0},
												{0, -1 / 0.707, 0},
												{-1 / 0.707, 0, 0} };

	static std::vector<Mach1Point3DCore> m1Spatial_32_Def = {
												{0.00000, 1.32048, 0.50689},
												{-0.74953, 1.19950, 0.00000},
												{0.00000, 1.32048, -0.50689},
												{0.74953, 1.19950, 0.00000},
												{0.00000, 0.74953, 1.19950},
												{-0.81928, 0.81928, 0.81128},
												{-1.32048, 0.50689, 0.00000},
												{-0.81928, 0.81928, -0.81128},
												{0.00000, 0.74953, -1.19950},
												{0.81928, 0.81928, -0.81128},
												{1.32048, 0.50689, 0.00000},
												{0.81928, 0.81928, 0.81128},
												{-0.50681, -0.00885, 1.32048},
												{-1.19950, -0.00000, 0.74953},
												{-1.21240, -0.00000, -0.72848},
												{-0.50681, 0.00885, -1.32048},
												{0.00000, -1.32048, 0.50689},
												{0.74953, -1.19950, 0.00000},
												{0.00000, -1.32048, -0.50689},
												{-0.74953, -1.19950, 0.00000},
												{0.00000, -0.74953, 1.19950},
												{0.81928, -0.81928, 0.81128},
												{1.32048, -0.50689, 0.00000},
												{0.81928, -0.81928, -0.81128},
												{0.00000, -0.74953, -1.19950},
												{-0.81928, -0.81928, -0.81128},
												{-1.32048, -0.50689, 0.00000},
												{-0.81928, -0.81928, 0.81128},
												{0.50681, -0.00885, 1.32048},
												{1.19950, 0.00000, 0.74953},
												{1.19950, 0.00000, -0.74953},
												{0.50681, 0.00885, -1.32048} };

	static std::vector<Mach1Point3DCore> m1Spatial_36_Def = {
												{ -0.43310, 0.71779, 1.13922},
												{ 1.13922, -0.43310, 0.71779},
												{ 0.43310, -0.71779, 1.13922},
												{ 0.71779, 1.13922, -0.43310},
												{ 1.13922, 0.43310, -0.71779},
												{ -0.71779, 1.13922, 0.43310},
												{ -1.13922, 0.43310, 0.71779},
												{ -0.71779, -1.13922, -0.43310},
												{ -1.13922, -0.43310, -0.71779},
												{ 0.71779, -1.13922, 0.43310},
												{ 0.43310, 0.71779, -1.13922},
												{ -0.43310, -0.71779, -1.13922},
												{ -0.34445, 0.88595, -1.04740},
												{ -1.04740, -0.34445, 0.88595},
												{ 0.34445, -0.88595, -1.04740},
												{ 0.88595, -1.04740, -0.34445},
												{ -1.04740, 0.34445, -0.88595},
												{ -0.88595, -1.04740, 0.34445},
												{ 1.04740, 0.34445, 0.88595},
												{ -0.88595, 1.04740, -0.34445},
												{ 1.04740, -0.34445, -0.88595},
												{ 0.88595, 1.04740, 0.34445},
												{ 0.34445, 0.88595, 1.04740},
												{ -0.34445, -0.88595, 1.04740},
												{ 1.35378, -0.40488, -0.06298},
												{ -0.06298, 1.35378, -0.40488},
												{ -1.35378, 0.40488, -0.06298},
												{ -0.40488, -0.06298, 1.35378},
												{ -0.06298, -1.35378, 0.40488},
												{ 0.40488, -0.06298, -1.35378},
												{ 0.06298, -1.35378, -0.40488},
												{ 0.40488, 0.06298, 1.35378},
												{ 0.06298, 1.35378, 0.40488},
												{ -0.40488, 0.06298, -1.35378},
												{ -1.35378, -0.40488, 0.06298},
												{ 1.35378, 0.40488, 0.06298} };

	static std::vector<Mach1Point3DCore> m1Spatial_48_Def = {
												{ 0.50006, 1.32018, -0.08763},
												{ -0.50006, 1.32018, 0.08763},
												{ -0.08763, 1.32018, -0.50006},
												{ 0.08763, 1.32018, 0.50006},
												{ 0.50006, -1.32018, 0.08763},
												{ -0.50006, -1.32018, -0.08763},
												{ -0.08763, -1.32018, 0.50006},
												{ 0.08763, -1.32018, -0.50006},
												{ 1.32018, -0.08763, 0.50006},
												{ 1.32018, 0.08763, -0.50006},
												{ 1.32018, -0.50006, -0.08763},
												{ 1.32018, 0.50006, 0.08763},
												{ -1.32018, 0.08763, 0.50006},
												{ -1.32018, -0.08763, -0.50006},
												{ -1.32018, 0.50006, -0.08763},
												{ -1.32018, -0.50006, 0.08763},
												{ -0.08763, 0.50006, 1.32018},
												{ 0.08763, -0.50006, 1.32018},
												{ -0.50006, -0.08763, 1.32018},
												{ 0.50006, 0.08763, 1.32018},
												{ 0.08763, 0.50006, -1.32018},
												{ -0.08763, -0.50006, -1.32018},
												{ 0.50006, -0.08763, -1.32018},
												{ -0.50006, 0.08763, -1.32018},
												{ 0.90487, 0.99978, 0.42693},
												{ -0.90487, 0.99978, -0.42693},
												{ 0.42693, 0.99978, -0.90487},
												{ -0.42693, 0.99978, 0.90487},
												{ 0.90487, -0.99978, -0.42693},
												{ -0.90487, -0.99978, 0.42693},
												{ 0.42693, -0.99978, 0.90487},
												{ -0.42693, -0.99978, -0.90487},
												{ 0.99978, 0.42693, 0.90487},
												{ 0.99978, -0.42693, -0.90487},
												{ 0.99978, -0.90487, 0.42693},
												{ 0.99978, 0.90487, -0.42693},
												{ -0.99978, -0.42693, 0.90487},
												{ -0.99978, 0.42693, -0.90487},
												{ -0.99978, 0.90487, 0.42693},
												{ -0.99978, -0.90487, -0.42693},
												{ 0.42693, 0.90487, 0.99978},
												{ -0.42693, -0.90487, 0.99978},
												{ -0.90487, 0.42693, 0.99978},
												{ 0.90487, -0.42693, 0.99978},
												{ -0.42693, 0.90487, -0.99978},
												{ 0.42693, -0.90487, -0.99978},
												{ 0.90487, 0.42693, -0.99978},
												{ -0.90487, -0.42693, -0.99978} };

	static std::vector<Mach1Point3DCore> m1Spatial_60_Def = {
												{ 0.77221, -1.06624, -0.51714},
												{ -0.51714, 0.77221, -1.06624},
												{ -0.77221, 1.06624, -0.51714},
												{ -1.06624, -0.51714, 0.77221},
												{ -0.51714, -0.77221, 1.06624},
												{ 1.06624, -0.51714, -0.77221},
												{ 0.51714, -0.77221, -1.06624},
												{ 1.06624, 0.51714, 0.77221},
												{ 0.51714, 0.77221, 1.06624},
												{ -1.06624, 0.51714, -0.77221},
												{ -0.77221, -1.06624, 0.51714},
												{ 0.77221, 1.06624, 0.51714},
												{ -1.00870, 0.99036, 0.04822},
												{ 0.04822, -1.00870, 0.99035},
												{ 1.00870, -0.99036, 0.04822},
												{ 0.99036, 0.04822, -1.00870},
												{ 0.04822, 1.00870, -0.99036},
												{ -0.99036, 0.04822, 1.00870},
												{ -0.04822, 1.00870, 0.99036},
												{ -0.99036, -0.04822, -1.00870},
												{ -0.04822, -1.00870, -0.99035},
												{ 0.99036, -0.04822, 1.00870},
												{ 1.00870, 0.99036, -0.04822},
												{ -1.00870, -0.99036, -0.04822},
												{ 0.10898, 0.39071, -1.35502},
												{ -1.35502, 0.10898, 0.39071},
												{ -0.10898, -0.39071, -1.35502},
												{ 0.39071, -1.35502, 0.10898},
												{ -1.35502, -0.10898, -0.39071},
												{ -0.39071, -1.35502, -0.10898},
												{ 1.35502, -0.10898, 0.39071},
												{ -0.39071, 1.35502, 0.10898},
												{ 1.35502, 0.10898, -0.39071},
												{ 0.39071, 1.35502, -0.10898},
												{ -0.10898, 0.39071, 1.35502},
												{ 0.10898, -0.39071, 1.35502},
												{ -1.10800, 0.63907, 0.60377},
												{ 0.60377, -1.10800, 0.63907},
												{ 1.10800, -0.63907, 0.60377},
												{ 0.63907, 0.60377, -1.10800},
												{ 0.60377, 1.10800, -0.63907},
												{ -0.63907, 0.60377, 1.10800},
												{ -0.60377, 1.10800, 0.63907},
												{ -0.63907, -0.60377, -1.10800},
												{ -0.60377, -1.10800, -0.63907},
												{ 0.63907, -0.60377, 1.10800},
												{ 1.10800, 0.63907, -0.60377},
												{ -1.10800, -0.63907, -0.60377},
												{ -1.31996, -0.47890, 0.17020},
												{ 0.17020, -1.31996, -0.47890},
												{ 1.31996, 0.47890, 0.17020},
												{ -0.47890, 0.17020, -1.31996},
												{ 0.17020, 1.31996, 0.47890},
												{ 0.47890, 0.17020, 1.31996},
												{ -0.17020, 1.31996, -0.47890},
												{ 0.47890, -0.17020, -1.31996},
												{ -0.17020, -1.31996, 0.47890},
												{ -0.47890, -0.17020, 1.31996},
												{ 1.31996, -0.47890, -0.17020},
												{ -1.31996, 0.47890, -0.17020} };

	static std::map<OutputMode, std::vector<Mach1Point3DCore>> standards = {
		{OUTPUT_SPATIAL_8CH, m1Spatial_8_Def},
		{OUTPUT_HORIZON_4CH, m1Horizon_4_Def},
		{OUTPUT_SPATIAL_12CH, m1Spatial_12_Def},
		{OUTPUT_SPATIAL_14CH, m1Spatial_14_Def},
		{OUTPUT_SPATIAL_16CH, m1Spatial_16_Def},
		{OUTPUT_SPATIAL_18CH, m1Spatial_18_Def},
		{OUTPUT_SPATIAL_20CH, m1Spatial_20_Def},
		{OUTPUT_SPATIAL_32CH, m1Spatial_32_Def},
		{OUTPUT_SPATIAL_36CH, m1Spatial_36_Def},
		{OUTPUT_SPATIAL_48CH, m1Spatial_48_Def},
		{OUTPUT_SPATIAL_60CH, m1Spatial_60_Def},
	};

	std::vector<Mach1Point3DCore> pointsSet;
	if (standards.find(outputMode) != standards.end()) {
		pointsSet = standards[outputMode];
	}

	result = getCoeffSetForStandardPointSet(x, y, z, pointsSet, outputMode == OUTPUT_HORIZON_4CH ? true : false);
}

int M1EncodeCore::getInputModeFromString(std::string name) {
	for (int i = 0; i < inputModeNames.size(); i++) {
		if (inputModeNames[i].name == name) {
			return inputModeNames[i].mode;
		}
	}
	return -1;
}

int M1EncodeCore::getOutputModeFromString(std::string name) {
	for (int i = 0; i < outputModeNames.size(); i++) {
		if (outputModeNames[i].name == name) {
			return outputModeNames[i].mode;
		}
	}
	return -1;
}

M1EncodeCore::M1EncodeCore() {
	inputMode = InputMode::INPUT_MONO;
	outputMode = OutputMode::OUTPUT_SPATIAL_8CH;
	pannerMode = PannerMode::MODE_ISOTROPICLINEAR;

	azimuth = 0;
	diverge = 0;
	elevation = 0;
	frontSurroundPerspective = true; // default set surround formats to be front first person perspective

	orbitRotation = 0;
	sSpread = 0;
	autoOrbit = false;

	ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	timeLastCalculation = 0;

	outputGainLinearMultipler = 1.0;

	// init additional arrays
	arr_Points = new Mach1Point3DCore[MAX_POINTS_COUNT];

	arr_Gains = new float*[MAX_POINTS_COUNT];
	for (int i = 0; i < MAX_POINTS_COUNT; i++) {
		arr_Gains[i] = new float[MAX_CHANNELS_COUNT];
	}

	arr_PointsNames = new char*[MAX_POINTS_COUNT];
	for (int i = 0; i < MAX_POINTS_COUNT; i++) {
		arr_PointsNames[i] = new char[255];
		arr_PointsNames[i][0] = '\0';
	}

	arr_GainsForInputChannelNamed = new float[MAX_CHANNELS_COUNT];

	arr_ResultingCoeffsDecoded = new float[MAX_CHANNELS_COUNT];
}

M1EncodeCore::~M1EncodeCore() {
	delete[] arr_Points;

	for (int i = 0; i < MAX_POINTS_COUNT; i++) {
		delete[] arr_Gains[i];
	}
	delete[] arr_Gains;

	for (int i = 0; i < MAX_POINTS_COUNT; i++) {
		delete[] arr_PointsNames[i];
	}
	delete[] arr_PointsNames;

	delete[] arr_GainsForInputChannelNamed;

	delete[] arr_ResultingCoeffsDecoded;
}

void M1EncodeCore::generatePointResults() {
	long tStart = getCurrentTime();

	if (pannerMode != MODE_ISOTROPICLINEAR && pannerMode != MODE_ISOTROPICEQUALPOWER){
		pannerMode = MODE_PERIPHONICLINEAR;
	}

	Mach1Point3DCore CenterChannel;

	float normalisedOutputDiverge = diverge * (1 / cos(PI * 0.25f));
	Mach1Point3DCore centerpoint = { (float)cos((azimuth)* PI * 2) * normalisedOutputDiverge, 0, (float)sin((azimuth)* PI * 2) * normalisedOutputDiverge };

	if (inputMode == INPUT_MONO) {

		resultingPoints.pointsCount = 1;
		resultingPoints.pointsNames[0] = "M";

		if (outputMode == OUTPUT_HORIZON_4CH) {
			resultingPoints.ppoints[0] = centerpoint;
		} else {
			if (pannerMode == MODE_ISOTROPICLINEAR || pannerMode == MODE_ISOTROPICEQUALPOWER) {
				resultingPoints.ppoints[0] = { centerpoint.x * (float)sin((-elevation + 1) * PI / 2), (float)cos((-elevation + 1) * PI / 2) * normalisedOutputDiverge, centerpoint.z * (float)sin((-elevation + 1) * PI / 2) };
			} else if (pannerMode == MODE_PERIPHONICLINEAR) {
				resultingPoints.ppoints[0] = { centerpoint.x, elevation, centerpoint.z };
			}
		}
	} else if (inputMode == INPUT_STEREO) {

		float sRotationInRadians;
		if (autoOrbit) {
			sRotationInRadians = azimuth * PI * 2 - PI / 2;
		} else { 
			sRotationInRadians = orbitRotation * PI * 2 - PI / 2;
		}

		resultingPoints.pointsCount = 2;

		std::vector<std::string> names = { "L", "R" };
		std::vector<Mach1Point3DCore> pnts = {
			{ (float)cos((sRotationInRadians)) * sSpread, 0, (float)sin((sRotationInRadians)) * sSpread },
			{ (float)-cos((sRotationInRadians)) * sSpread, 0, (float)-sin((sRotationInRadians)) * sSpread }
		};

		for (int i = 0; i < resultingPoints.pointsCount; i++)
		{
			resultingPoints.pointsNames[i] = names[i];
			if (outputMode == OUTPUT_HORIZON_4CH) {
				resultingPoints.ppoints[i] = pnts[i] + centerpoint;
			} else if (pannerMode == MODE_ISOTROPICLINEAR || pannerMode == MODE_ISOTROPICEQUALPOWER) {
				resultingPoints.ppoints[i] = pnts[i] + Mach1Point3DCore { centerpoint.x * (float)sin((elevation + 1) * PI / 2), elevation, centerpoint.z * (float)sin((elevation + 1) * PI / 2) };
			} else if (pannerMode == MODE_PERIPHONICLINEAR) {
				resultingPoints.ppoints[i] = pnts[i] + Mach1Point3DCore { centerpoint.x, elevation, centerpoint.z };
			}
		}
	} else if (inputMode == INPUT_QUAD) {

		resultingPoints.pointsCount = 4;

		std::vector<std::string> names = { "L", "R", "Ls", "Rs" };
		std::vector<Mach1Point3DCore> pnts = {
			{ (float)cos((azimuth + 0.125f - 0.25f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth + 0.125f - 0.25f) * PI * 2) * normalisedOutputDiverge },
			{ (float)cos((azimuth + 0.125f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth + 0.125f) * PI * 2) * normalisedOutputDiverge },
			{ (float)cos((azimuth + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge },
			{ (float)cos((azimuth + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge },
		};

		for (int i = 0; i < resultingPoints.pointsCount; i++)
		{
			resultingPoints.pointsNames[i] = names[i];
			resultingPoints.ppoints[i] = pnts[i];
			if (outputMode == OUTPUT_HORIZON_4CH) {
				resultingPoints.ppoints[i].y = 0;
			}
		}
	} else if (inputMode == INPUT_LCRS) {

		resultingPoints.pointsCount = 4;

		std::vector<std::string> names = { "L", "C", "R", "S" };
		if (frontSurroundPerspective) {
			CenterChannel = { (float)cos((azimuth + 0.0f) * PI * 2) * diverge, 0, (float)sin((azimuth + 0.0f) * PI * 2) * diverge };
		} else {
			CenterChannel = { 0, 0, 0 };
		}
		std::vector<Mach1Point3DCore> pnts = {
			{ (float)cos((azimuth - 0.125f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth - 0.125f) * PI * 2) * normalisedOutputDiverge },
			CenterChannel,
			{ (float)cos((azimuth + 0.125f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth + 0.125f) * PI * 2) * normalisedOutputDiverge },
			{ (float)cos((azimuth + 0.5f) * PI * 2) * diverge, 0, (float)sin((azimuth + 0.5f) * PI * 2) * diverge },
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

	} else if (inputMode == INPUT_AFORMAT) {

		resultingPoints.pointsCount = 4;

		std::vector<std::string> names = { "FLU", "FRD", "BLD", "BRU" };
		std::vector<Mach1Point3DCore> pnts = {
			{ (float)cos((azimuth + 0.125f - 0.25f) * PI * 2) * diverge, (1 * diverge), (float)sin((azimuth + 0.125f - 0.25f) * PI * 2) * diverge },
			{ (float)cos((azimuth + 0.125f) * PI * 2) * diverge, (-1 * diverge), (float)sin((azimuth + 0.125f) * PI * 2) * diverge },
			{ (float)cos((azimuth + 0.125f + 0.5f) * PI * 2) * diverge, (-1 * diverge), (float)sin((azimuth + 0.125f + 0.5f) * PI * 2) * diverge },
			{ (float)cos((azimuth + 0.125f + 0.25f) * PI * 2) * diverge, (1 * diverge), (float)sin((azimuth + 0.125f + 0.25f) * PI * 2) * diverge },
		};

		for (int i = 0; i < resultingPoints.pointsCount; i++)
		{
			resultingPoints.pointsNames[i] = names[i];
			resultingPoints.ppoints[i] = pnts[i];
			if (outputMode == OUTPUT_HORIZON_4CH) {
				resultingPoints.ppoints[i].y = 0;
			}
		}

	} else if (inputMode == INPUT_1OAACN) { // duplicate?

		resultingPoints.pointsCount = 7;

		std::vector<std::string> names = { "W","1","2","3","-1","-2","-3" };
		std::vector<Mach1Point3DCore> pnts = {
			{ 0, 0, 0 },  // W
			{ (float)cos((azimuth + 0.250f) * PI * 2) * diverge, 0, (float)sin((azimuth + 0.250f) * PI * 2) * diverge }, // Left/Right / L
			{ 0, 1.0f * diverge, 0 }, // Top/Bottom / T
			{ (float)cos((azimuth + 0.0f) * PI * 2) * diverge, 0, (float)sin((azimuth + 0.0f) * PI * 2) * diverge }, // Front/Back / F
			{ (float)cos((azimuth + 0.75f) * PI * 2) * diverge, 0, (float)sin((azimuth + 0.75f) * PI * 2) * diverge }, // -Left/Right / R
			{ 0, -1.0f * diverge, 0 }, // -Top/Bottom / B
			{ (float)cos((azimuth + 0.5f) * PI * 2) * diverge, 0, (float)sin((azimuth + 0.5f) * PI * 2) * diverge }, // -Front/Back / B
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

	} else if (inputMode == INPUT_1OAFUMA) {

		resultingPoints.pointsCount = 7;

		std::vector<std::string> names = { "W","X","Y","Z","-X","-Y","-Z" };
		std::vector<Mach1Point3DCore> pnts = {
			{ 0, 0, 0 }, // W
			{ (float)cos((azimuth + 0.0f) * PI * 2) * diverge, 0, (float)sin((azimuth + 0.0f) * PI * 2) * diverge }, // Front/Back / F
			{ (float)cos((azimuth + 0.25f) * PI * 2) * diverge, 0, (float)sin((azimuth + 0.25f) * PI * 2) * diverge }, // Left/Right / L
			{ 0, 1.0f * diverge, 0 },  // Top/Bottom / T
			{ (float)cos((azimuth + 0.5f) * PI * 2) * diverge, 0, (float)sin((azimuth + 0.5f) * PI * 2) * diverge }, // -Front/Back / B
			{ (float)cos((azimuth + 0.75f) * PI * 2) * diverge, 0, (float)sin((azimuth + 0.75f) * PI * 2) * diverge }, // -Left/Right / R
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
	} else if (inputMode == INPUT_2OAACN || inputMode == INPUT_2OAFUMA || inputMode == INPUT_3OAACN || inputMode == INPUT_3OAFUMA) { // duplicate?

		/*
		TODO: Rework this into something smarter
		Currently expects 2OA and 3OA conversion to Mach1Spatial8 externally from this API
		This API will just supply the azimuths for Mach1Spatial8 Cuboid
		*/
					 
		resultingPoints.pointsCount = 8;

		std::vector<std::string> names = { "1","2","3","4","5","6","7","8" };
		std::vector<Mach1Point3DCore> pnts = {
			{ (float)cos((azimuth + 0.125f - 0.25f) * PI * 2) * normalisedOutputDiverge, 1.0f * diverge, (float)sin((azimuth + 0.125f - 0.25f) * PI * 2) * normalisedOutputDiverge },
			{ (float)cos((azimuth + 0.125f) * PI * 2) * normalisedOutputDiverge, 1.0f * diverge, (float)sin((azimuth + 0.125f) * PI * 2) * normalisedOutputDiverge },
			{ (float)cos((azimuth + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge, 1.0f * diverge, (float)sin((azimuth + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge },
			{ (float)cos((azimuth + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge, 1.0f * diverge, (float)sin((azimuth + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge },
			{ (float)cos((azimuth + 0.125f - 0.25f) * PI * 2) * normalisedOutputDiverge, -1.0f * diverge, (float)sin((azimuth + 0.125f - 0.25f) * PI * 2) * normalisedOutputDiverge },
			{ (float)cos((azimuth + 0.125f) * PI * 2) * normalisedOutputDiverge, -1.0f * diverge, (float)sin((azimuth + 0.125f) * PI * 2) * normalisedOutputDiverge },
			{ (float)cos((azimuth + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge, -1.0f * diverge, (float)sin((azimuth + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge },
			{ (float)cos((azimuth + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge, -1.0f * diverge, (float)(float)sin((azimuth + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge },
		};

		for (int i = 0; i < resultingPoints.pointsCount; i++)
		{
			resultingPoints.pointsNames[i] = names[i];
			resultingPoints.ppoints[i] = pnts[i];
			if (outputMode == OUTPUT_HORIZON_4CH) {
				resultingPoints.ppoints[i].y = 0;
			}
		}
	} else if (inputMode == INPUT_LCR) {

		resultingPoints.pointsCount = 3;

		std::vector<std::string> names = { "L","C","R" };
		if (frontSurroundPerspective) {
			CenterChannel = { (float)cos((azimuth + 0.0f) * PI * 2) * diverge, 0, (float)sin((azimuth + 0.0f) * PI * 2) * diverge };
		} else {
			CenterChannel = { 0, 0, 0 };
		}
		std::vector<Mach1Point3DCore> pnts = {
			{ (float)cos((azimuth - 0.125f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth - 0.125f) * PI * 2) * normalisedOutputDiverge },
			CenterChannel,
			{ (float)cos((azimuth + 0.125f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth + 0.125f) * PI * 2) * normalisedOutputDiverge },
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
	} else if (inputMode == INPUT_FIVE_ZERO) {

		resultingPoints.pointsCount = 5;

		std::vector<std::string> names = { "L","C","R","Ls","Rs" };
		if (frontSurroundPerspective) {
			CenterChannel = { (float)cos((azimuth + 0.0f) * PI * 2) * diverge, 0, (float)sin((azimuth + 0.0f) * PI * 2) * diverge };
		} else {
			CenterChannel = { 0, 0, 0 };
		}
		std::vector<Mach1Point3DCore> pnts = {
			{ (float)cos((azimuth - 0.125f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth - 0.125f) * PI * 2) * normalisedOutputDiverge },
			CenterChannel,
			{ (float)cos((azimuth + 0.125f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth + 0.125f) * PI * 2) * normalisedOutputDiverge },
			{ (float)cos((azimuth + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge },
			{ (float)cos((azimuth + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge },
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
	} else if (inputMode == INPUT_FIVE_ONE_FILM) {

		resultingPoints.pointsCount = 6;

		std::vector<std::string> names = { "L","C","R","Ls","Rs","LFE" };
		if (frontSurroundPerspective) {
			CenterChannel = { (float)cos((azimuth + 0.0f) * PI * 2) * diverge, 0, (float)sin((azimuth + 0.0f) * PI * 2) * diverge };
		} else {
			CenterChannel = { 0, 0, 0 };
		}
		std::vector<Mach1Point3DCore> pnts = {
			{ (float)cos((azimuth - 0.125f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth - 0.125f) * PI * 2) * normalisedOutputDiverge },
			CenterChannel,
			{ (float)cos((azimuth + 0.125f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth + 0.125f) * PI * 2) * normalisedOutputDiverge },
			{ (float)cos((azimuth + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge },
			{ (float)cos((azimuth + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge },
			{ 0, 0, 0 },
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
	} else if (inputMode == INPUT_FIVE_ONE_DTS) {

		resultingPoints.pointsCount = 6;

		std::vector<std::string> names = { "L","R","Ls","Rs","C","LFE" };
		if (frontSurroundPerspective) {
			CenterChannel = { (float)cos((azimuth + 0.0f) * PI * 2) * diverge, 0, (float)sin((azimuth + 0.0f) * PI * 2) * diverge };
		} else {
			CenterChannel = { 0, 0, 0 };
		}
		std::vector<Mach1Point3DCore> pnts = {
			{ (float)cos((azimuth - 0.125f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth - 0.125f) * PI * 2) * normalisedOutputDiverge },
			{ (float)cos((azimuth + 0.125f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth + 0.125f) * PI * 2) * normalisedOutputDiverge },
			{ (float)cos((azimuth + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge },
			{ (float)cos((azimuth + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge },
			CenterChannel,
			{ 0, 0, 0 },
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
	} else if (inputMode == INPUT_FIVE_ONE_SMPTE) {

		resultingPoints.pointsCount = 6;

		std::vector<std::string> names = { "L","R","C","LFE","Ls","Rs" };
		if (frontSurroundPerspective) {
			CenterChannel = { (float)cos((azimuth + 0.0f) * PI * 2) * diverge, 0, (float)sin((azimuth + 0.0f) * PI * 2) * diverge };
		} else {
			CenterChannel = { 0, 0, 0 };
		}
		std::vector<Mach1Point3DCore> pnts = {
			{ (float)cos((azimuth - 0.125f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth - 0.125f) * PI * 2) * normalisedOutputDiverge },
			{ (float)cos((azimuth + 0.125f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth + 0.125f) * PI * 2) * normalisedOutputDiverge },
			CenterChannel,
			{ 0, 0, 0 },
			{ (float)cos((azimuth + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge },
			{ (float)cos((azimuth + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge },
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
		resultingPoints.ppoints[i].x = clamp(resultingPoints.ppoints[i].x / (1 / 0.707), -1, 1);
		resultingPoints.ppoints[i].y = clamp(resultingPoints.ppoints[i].y / (1 / 0.707), -1, 1);
		resultingPoints.ppoints[i].z = clamp(resultingPoints.ppoints[i].z / (1 / 0.707), -1, 1);
	}

	// Generating channel gains

	resultingPoints.gains.resize(resultingPoints.pointsCount);
	for (int i = 0; i < resultingPoints.pointsCount; i++) {

		resultingPoints.ppoints[i].x = resultingPoints.ppoints[i].x / 2 + 0.5f;
		resultingPoints.ppoints[i].y = resultingPoints.ppoints[i].y / 2 + 0.5f;
		resultingPoints.ppoints[i].z = resultingPoints.ppoints[i].z / 2 + 0.5f;

		resultingPoints.gains[i].resize(getOutputChannelsCount());

		// Generating gains
		std::vector<float> gains;
		if (outputMode == OUTPUT_HORIZON_4CH) {
			// force ppoints to center of soundfield's elevation
			processGainsChannels(resultingPoints.ppoints[i].z, resultingPoints.ppoints[i].x, 0, gains);
		} else {
			// process 3d ppoints
			processGainsChannels(resultingPoints.ppoints[i].z, resultingPoints.ppoints[i].x, resultingPoints.ppoints[i].y, gains);
		}
		// applying output gain to gains and assigning to the current index channel
		for (int j = 0; j < getOutputChannelsCount(); j++) {
			resultingPoints.gains[i][j] = gains[j] * outputGainLinearMultipler;
		}
	}
	timeLastCalculation = getCurrentTime() - tStart;
}


void M1EncodeCore::getResultingCoeffsDecoded(Mach1DecodeAlgoType decodeType, float* decodeResult, float* result)
{
		// clear
		for (int i = 0; i < resultingPoints.pointsCount * 2; i++) result[i] = 0;

		// TODO: check on these numbers

		int decodeResultSize = 0;
		switch (decodeType)
		{
            case Mach1DecodeAlgoSpatial_8:
				decodeResultSize = 16;
				break;
            case Mach1DecodeAlgoSpatialAlt_8:
				decodeResultSize = 16;
				break;
            case Mach1DecodeAlgoHorizon_4:
				decodeResultSize = 8;
				break;
            case Mach1DecodeAlgoHorizonPairs:
				decodeResultSize = 8;
				break;
            case Mach1DecodeAlgoSpatialPairs:
				decodeResultSize = 16;
				break;
            case Mach1DecodeAlgoSpatial_12:
				decodeResultSize = 24;
				break;
            case Mach1DecodeAlgoSpatial_14:
				decodeResultSize = 28;
				break;
            case Mach1DecodeAlgoSpatial_16:
				decodeResultSize = 32;
				break;
            case Mach1DecodeAlgoSpatial_18:
				decodeResultSize = 36;
				break;
			case Mach1DecodeAlgoSpatial_20:
				decodeResultSize = 40;
				break;
			case Mach1DecodeAlgoSpatial_32:
				decodeResultSize = 64;
				break;
			case Mach1DecodeAlgoSpatial_36:
				decodeResultSize = 72;
				break;
			case Mach1DecodeAlgoSpatial_48:
				decodeResultSize = 96;
				break;
			case Mach1DecodeAlgoSpatial_60:
				decodeResultSize = 120;
				break;
			default:
				break;
		}

		// decode - 8, 16
		if (getOutputChannelsCount() * 2 != decodeResultSize) {
				std::cout << "[MACH1] Warning: The Mach1EncodeOutputModeType in use is not suitable for the Mach1DecodeAlgoType selected" << std::endl;
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

int M1EncodeCore::getInputChannelsCount() {
	switch (inputMode) {
		case INPUT_MONO: return 1;
		case INPUT_STEREO: return 2;
		case INPUT_LCR: return 3;
		case INPUT_QUAD: return 4;
		case INPUT_LCRS: return 4;
		case INPUT_AFORMAT: return 4;
		case INPUT_FIVE_ZERO: return 5;
		case INPUT_FIVE_ONE_FILM: return 6;
		case INPUT_FIVE_ONE_DTS: return 6; 
		case INPUT_FIVE_ONE_SMPTE: return 6;
		case INPUT_1OAACN: return 4;
		case INPUT_1OAFUMA: return 4;
		case INPUT_2OAACN: return 9;
		case INPUT_2OAFUMA: return 9;
		case INPUT_3OAACN: return 16;
		case INPUT_3OAFUMA: return 16;
	}
	return 0;
}

int M1EncodeCore::getOutputChannelsCount() {
	switch (outputMode) {
		case OUTPUT_SPATIAL_8CH: return 8;
		case OUTPUT_HORIZON_4CH: return 4;
		case OUTPUT_SPATIAL_12CH: return 12;
		case OUTPUT_SPATIAL_14CH: return 14;
		case OUTPUT_SPATIAL_16CH: return 16;
		case OUTPUT_SPATIAL_18CH: return 18;
		case OUTPUT_SPATIAL_20CH: return 20;
		case OUTPUT_SPATIAL_32CH: return 32;
		case OUTPUT_SPATIAL_36CH: return 36;
		case OUTPUT_SPATIAL_48CH: return 48;
		case OUTPUT_SPATIAL_60CH: return 60;
	}
	return 0;
}

void M1EncodeCore::setInputMode(InputMode inputMode) {
	this->inputMode = inputMode;
}

void M1EncodeCore::setOutputMode(OutputMode outputMode) {
	this->outputMode = outputMode;
}

void M1EncodeCore::setAzimuth(float azimuthFromMinus1To1) {
	this->azimuth = azimuthFromMinus1To1;
}

void M1EncodeCore::setAzimuthDegrees(float azimuthDegrees) {
	azimuthDegrees = fmod(azimuthDegrees, 360.0); //protect a 360 cycle
	if (azimuthDegrees < 0) { //check if -180 to 180, convert to 0-360
		azimuthDegrees += 360.0;
	}
	this->azimuth = azimuthDegrees / 360.0;
}

void M1EncodeCore::setAzimuthRadians(float azimuthRadians) {
	azimuthRadians = fmod(azimuthRadians, PI*2.0); //protect a 360 cycle
	if (azimuthRadians < 0.0) { //check if -180 to 180, convert to 0-360
		azimuthRadians += PI*2.0;
	}
	this->azimuth = azimuthRadians / (PI*2.0);
}

void M1EncodeCore::setDiverge(float divergeFromMinus1To1) {
	divergeFromMinus1To1 = clamp(divergeFromMinus1To1, -1, 1);
	this->diverge = divergeFromMinus1To1;
}

void M1EncodeCore::setElevation(float elevationFromMinus1to1) {
	elevationFromMinus1to1 = clamp(elevationFromMinus1to1, -1, 1);
	this->elevation = elevationFromMinus1to1;
}

void M1EncodeCore::setElevationDegrees(float elevationFromMinus90to90) {
	elevationFromMinus90to90 = clamp(elevationFromMinus90to90, -90, 90);
	this->elevation = elevationFromMinus90to90 / 90;
}

void M1EncodeCore::setElevationRadians(float elevationFromMinusHalfPItoHalfPI) {
	elevationFromMinusHalfPItoHalfPI = clamp(elevationFromMinusHalfPItoHalfPI, -PI / 2, PI / 2);
	this->elevation = elevationFromMinusHalfPItoHalfPI / (PI / 2);
}

void M1EncodeCore::setPannerMode(PannerMode pannerMode){
	this->pannerMode = pannerMode;
}

void M1EncodeCore::setFrontSurroundPerspective(bool frontSurroundPerspective){
	this->frontSurroundPerspective = frontSurroundPerspective;
}

void M1EncodeCore::setOutputGain(float outputGainMultipler, bool isDecibel){
	if (isDecibel) {
		this->outputGainLinearMultipler = std::pow(10.0f, outputGainMultipler/20.0f);
	} else {
		this->outputGainLinearMultipler = outputGainMultipler;
	}
}

void M1EncodeCore::setAutoOrbit(bool autoOrbit) {
	this->autoOrbit = autoOrbit;
}

void M1EncodeCore::setOrbitRotation(float orbitRotationFromMinusOnetoOne) {
	this->orbitRotation = orbitRotationFromMinusOnetoOne;
}

void M1EncodeCore::setOrbitRotationDegrees(float orbitRotationDegrees) {
	orbitRotationDegrees = fmod(orbitRotationDegrees, 360.0); //protect a 360 cycle
	if (orbitRotationDegrees < 0) { //check if -180 to 180, convert to 0-360
		orbitRotationDegrees += 360.0;
	}
	this->orbitRotation = orbitRotationDegrees / 360.0;
}

void M1EncodeCore::setOrbitRotationRadians(float orbitRotationRadians) {
	orbitRotationRadians = fmod(orbitRotationRadians, PI*2.0); //protect a 360 cycle
	if (orbitRotationRadians < 0.0) { //check if -180 to 180, convert to 0-360
		orbitRotationRadians += PI*2.0;
	}
	this->orbitRotation = orbitRotationRadians / (PI*2.0);
}

void M1EncodeCore::setStereoSpread(float sSpreadFrom0to1) {
	sSpreadFrom0to1 = clamp(sSpreadFrom0to1, -1, 1);
	this->sSpread = sSpreadFrom0to1;
}

long M1EncodeCore::getCurrentTime()
{
	return (long)(duration_cast<milliseconds>(system_clock::now().time_since_epoch()) - ms).count();
}

long M1EncodeCore::getLastCalculationTime()
{
	return timeLastCalculation;
}
