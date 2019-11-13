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

float clamp(float n, float lower, float upper) {
	return std::max(lower, std::min(n, upper));
}

M1EncodeCorePointResults::M1EncodeCorePointResults() {
	for (int i = 0; i < 7; i++) {
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

void M1EncodeCore::process2OAAmbisonicGains(float input[9], float(*transcoded)[9]){
        /*
    0   0   1                                           1
    1   1   sin(a)cos(e)                                y
    2   1   sin(e)                                      z
    3   1   cos(a)cos(e)                                x
    4   2   sqrt(3/4)sin(2a)cos(e)cos(e)                sqrt(3)xy
    5   2   sqrt(3/4)sin(a)sin(2e)                      sqrt(3)yz
    6   2   (1/2)(3sin(e)sin(e)-1)                      (1/2)(3zz-1)
    7   2   sqrt(3/4)cos(a)sin(2e)                      sqrt(3)xz
    8   2   sqrt(3/4)cos(2a)cos(e)cos(e)                sqrt(3/4)(xx-yy)
    9   3   sqrt(5/8)sin(3a)cos(e)cos(e)cos(e)          sqrt(5/8)y(3xx-yy)
    10  3   sqrt(15/4)sin(2a)sin(e)cos(e)cos(e)         sqrt(15)xyz
    11  3   sqrt(3/8)sin(a)cos(e)(5sin(e)sin(e)-1)      sqrt(3/8)y(5zz-1)
    12  3   (1/2)sin(e)(5sin(e)sin(e)-3)                (1/2)z(5zz-3)
    13  3   sqrt(3/8)cos(a)cos(e)(5sin(e)sin(e)-1)      sqrt(3/8)x(5zz-1)
    14  3   sqrt(15/4)cos(2a)sin(e)cos(e)cos(e)         sqrt(15/4)z(xx-yy)
    15  3   sqrt(5/8)cos(3a)cos(e)cos(e)cos(e)          sqrt(5/8)x(xx-3yy)
     */

    result[0] = 0.3727f*input[0] +  0.2795f*input[1] +  0.3953f*input[2] +  0.2795f*input[3] +  0.0807f*input[4] +  0.1141f*input[5] +  0.0466f*input[6] +  0.1141f*input[7] +  0.0000f*input[8];
    result[1] = 0.3727f*input[0] +  0.2795f*input[1] +  0.3953f*input[2] + -0.2795f*input[3] + -0.0807f*input[4] +  0.1141f*input[5] +  0.0466f*input[6] + -0.1141f*input[7] + -0.0000f*input[8];
    result[2] = 0.3727f*input[0] + -0.2795f*input[1] +  0.3953f*input[2] + -0.2795f*input[3] +  0.0807f*input[4] + -0.1141f*input[5] +  0.0466f*input[6] + -0.1141f*input[7] +  0.0000f*input[8];
    result[3] = 0.3727f*input[0] + -0.2795f*input[1] +  0.3953f*input[2] +  0.2795f*input[3] + -0.0807f*input[4] + -0.1141f*input[5] +  0.0466f*input[6] +  0.1141f*input[7] + -0.0000f*input[8];
    result[4] = 0.3727f*input[0] +  0.2795f*input[1] + -0.3953f*input[2] +  0.2795f*input[3] +  0.0807f*input[4] + -0.1141f*input[5] +  0.0466f*input[6] + -0.1141f*input[7] +  0.0000f*input[8];
    result[5] = 0.3727f*input[0] +  0.2795f*input[1] + -0.3953f*input[2] + -0.2795f*input[3] + -0.0807f*input[4] + -0.1141f*input[5] +  0.0466f*input[6] +  0.1141f*input[7] + -0.0000f*input[8];
    result[6] = 0.3727f*input[0] + -0.2795f*input[1] + -0.3953f*input[2] + -0.2795f*input[3] +  0.0807f*input[4] +  0.1141f*input[5] +  0.0466f*input[6] +  0.1141f*input[7] +  0.0000f*input[8];
    result[7] = 0.3727f*input[0] + -0.2795f*input[1] + -0.3953f*input[2] +  0.2795f*input[3] + -0.0807f*input[4] +  0.1141f*input[5] +  0.0466f*input[6] + -0.1141f*input[7] + -0.0000f*input[8];
}

M1EncodeCore::M1EncodeCore() {
	inputMode = InputMode::INPUT_MONO;
	outputMode = OutputMode::OUTPUT_8CH;

	rotation = 0;
	diverge = 0;
	pitch = 0;

	sRotate = 0;
	sSpread = 0;
	autoOrbit = false;
	isotropicEncode = true; 

	outputChannelCount = 8;

	ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	timeLastCalculation = 0;
}

M1EncodeCore::~M1EncodeCore() {

}

void M1EncodeCore::generatePointResults() {
	long tStart = getCurrentTime();

    float normalisedOutputDiverge = diverge * (1 / cos(PI * 0.25f));
    
    switch (inputMode) {
        case INPUT_MONO:
            switch (outputMode) {
                case OUTPUT_4CH:
                    resultingPoints.pointsCount = 1;
                    resultingPoints.pointsNames[0] = "M";
                    resultingPoints.ppoints[0].x = cos((rotation) * PI * 2) * normalisedOutputDiverge; // X
                    resultingPoints.ppoints[0].y = 0; // Y
                    resultingPoints.ppoints[0].z = sin((rotation) * PI * 2) * normalisedOutputDiverge; // Z
                    break;
                case OUTPUT_8CH:
                    resultingPoints.pointsCount = 1;
                    resultingPoints.pointsNames[0] = "M";
                    if (isotropicEncode) {
                        resultingPoints.ppoints[0].x = cos((rotation) * PI * 2) * sin((-pitch + 1) * PI / 2) * normalisedOutputDiverge; // X
                        resultingPoints.ppoints[0].y = cos((-pitch + 1) * PI / 2) * normalisedOutputDiverge; // Y
                        resultingPoints.ppoints[0].z = sin((rotation) * PI * 2)  * sin((-pitch + 1) * PI / 2)* normalisedOutputDiverge; // Z
                    } else {
                        resultingPoints.ppoints[0].x = cos((rotation) * PI * 2) * normalisedOutputDiverge; // X
                        resultingPoints.ppoints[0].y = pitch; // Y
                        resultingPoints.ppoints[0].z = sin((rotation) * PI * 2) * normalisedOutputDiverge; // Z
                    }
                    break;
                default:
                    break;
            }
            break;
            
        case INPUT_STEREO:
            switch (outputMode) {
                case OUTPUT_4CH:
                    resultingPoints.pointsCount = 2;
                    resultingPoints.pointsNames[0] = "L";
                    resultingPoints.pointsNames[1] = "R";
                    float center[3];
                    center[0] = cos((rotation) * PI * 2) * normalisedOutputDiverge;
                    center[1] = 0;
                    center[2] = sin((rotation) * PI * 2) * normalisedOutputDiverge;
                    
                    if (autoOrbit) {
                        float sRotationInRadians = rotation * PI * 2 - PI / 2;

                        resultingPoints.ppoints[0].x = center[0] + cos((sRotationInRadians)) * sSpread; // X
                        resultingPoints.ppoints[0].y = 0; // Y
                        resultingPoints.ppoints[0].z = center[2] + sin((sRotationInRadians)) * sSpread; // Z
                        
                        resultingPoints.ppoints[1].x = center[0] - cos((sRotationInRadians)) * sSpread; // X
                        resultingPoints.ppoints[1].y = 0; // Y
                        resultingPoints.ppoints[1].z = center[2] - sin((sRotationInRadians)) * sSpread; // Z
                        
                    } else {
                        float sRotationInRadians = sRotate * DEG_TO_RAD - PI / 2;
                        
                        resultingPoints.ppoints[0].x = center[0] + cos((sRotationInRadians)) * sSpread; // X
                        resultingPoints.ppoints[0].y = 0; // Y
                        resultingPoints.ppoints[0].z = center[2] + sin((sRotationInRadians)) * sSpread; // Z
                        
                        resultingPoints.ppoints[1].x = center[0] - cos((sRotationInRadians)) * sSpread; // X
                        resultingPoints.ppoints[1].y = 0; // Y
                        resultingPoints.ppoints[1].z = center[2] - sin((sRotationInRadians)) * sSpread; // Z
                        
                    }
                    
					// Fixing it if we got outside the bounds
					for (int i = 0; i < resultingPoints.pointsCount; i++) {
						resultingPoints.ppoints[i].x = clamp(resultingPoints.ppoints[i].x, -1, 1);
						resultingPoints.ppoints[i].y = clamp(resultingPoints.ppoints[i].y, -1, 1);
						resultingPoints.ppoints[i].z = clamp(resultingPoints.ppoints[i].z, -1, 1);
					}

                    break;
                case OUTPUT_8CH:
                    resultingPoints.pointsCount = 2;
                    resultingPoints.pointsNames[0] = "L";
                    resultingPoints.pointsNames[1] = "R";
                    
                    float sRotationInRadians;
                    if (autoOrbit) {
                        sRotationInRadians = rotation * PI * 2 - PI / 2;
                    } else {
                        sRotationInRadians = sRotate * DEG_TO_RAD - PI / 2;
                    }
                    
                    if (isotropicEncode) {
                        center[0] = cos((rotation) * PI * 2) * sin((pitch + 1) * PI / 2) * normalisedOutputDiverge; // X
                        center[1] = cos((pitch + 1) * PI / 2) * normalisedOutputDiverge; // Y
                        center[2] = sin((rotation) * PI * 2)  * sin((pitch + 1) * PI / 2) * normalisedOutputDiverge; // Z
                        
                        resultingPoints.ppoints[0].x = center[0] + cos((sRotationInRadians)) * sSpread; // X
                        resultingPoints.ppoints[0].y = pitch; // Y
                        resultingPoints.ppoints[0].z = center[2] + sin((sRotationInRadians)) * sSpread; // Z
                        
                        resultingPoints.ppoints[1].x = center[0] - cos((sRotationInRadians)) * sSpread; // X
                        resultingPoints.ppoints[1].y = pitch; // Y
                        resultingPoints.ppoints[1].z = center[2] - sin((sRotationInRadians)) * sSpread; // Z
                    } else {
                        center[0] = cos((rotation) * PI * 2) * normalisedOutputDiverge;
                        center[1] = 0;
                        center[2] = sin((rotation) * PI * 2) * normalisedOutputDiverge;
                        
                        resultingPoints.ppoints[0].x = center[0] + cos((sRotationInRadians)) * sSpread; // X
                        resultingPoints.ppoints[0].y = pitch; // Y
                        resultingPoints.ppoints[0].z = center[2] + sin((sRotationInRadians)) * sSpread; // Z
                        
                        resultingPoints.ppoints[1].x = center[0] - cos((sRotationInRadians)) * sSpread; // X
                        resultingPoints.ppoints[1].y = pitch; // Y
                        resultingPoints.ppoints[1].z = center[2] - sin((sRotationInRadians)) * sSpread; // Z
                    }
                    
					// Fixing it if we got outside the bounds
					for (int i = 0; i < resultingPoints.pointsCount; i++) {
						resultingPoints.ppoints[i].x = clamp(resultingPoints.ppoints[i].x, -1, 1);
						resultingPoints.ppoints[i].y = clamp(resultingPoints.ppoints[i].y, -1, 1);
						resultingPoints.ppoints[i].z = clamp(resultingPoints.ppoints[i].z, -1, 1);
					}

                    break;
                default:
                    break;
            }
            break;
            
        case INPUT_LCR:
            switch (outputMode) {
                case OUTPUT_4CH:
                    // Quad input >> 4ch output
                    
                    resultingPoints.pointsCount = 3;
                    resultingPoints.pointsNames[0] = "L";
                    resultingPoints.pointsNames[1] = "C";
                    resultingPoints.pointsNames[2] = "R";
                    
                    // L
                    resultingPoints.ppoints[0].x = cos((rotation - 0.125) * PI * 2) * normalisedOutputDiverge; // X
                    resultingPoints.ppoints[0].y = 0; // Y
                    resultingPoints.ppoints[0].z = sin((rotation - 0.125) * PI * 2) * normalisedOutputDiverge; // Z
                    // C
                    resultingPoints.ppoints[1].x = (resultingPoints.ppoints[0].x + resultingPoints.ppoints[2].x) / 2; // X
                    resultingPoints.ppoints[1].y = 0; // Y
                    resultingPoints.ppoints[1].z = (resultingPoints.ppoints[0].z + resultingPoints.ppoints[2].z) / 2; // Z
                    // R
                    resultingPoints.ppoints[2].x = cos((rotation + 0.125) * PI * 2) * normalisedOutputDiverge; // X
                    resultingPoints.ppoints[2].y = 0; // Y
                    resultingPoints.ppoints[2].z = sin((rotation + 0.125) * PI * 2) * normalisedOutputDiverge; // Z
                    
                    break;
                case OUTPUT_8CH:
                    // Quad input >> 8ch output
                    
                    resultingPoints.pointsCount = 3;
                    resultingPoints.pointsNames[0] = "L";
                    resultingPoints.pointsNames[1] = "C";
                    resultingPoints.pointsNames[2] = "R";
                    
                    // L
                    resultingPoints.ppoints[0].x = cos((rotation + 0.125 - 0.25) * PI * 2) * normalisedOutputDiverge; // X
                    resultingPoints.ppoints[0].y = pitch; // Y
                    resultingPoints.ppoints[0].z = sin((rotation + 0.125 - 0.25) * PI * 2) * normalisedOutputDiverge; // Z
                    // C
                    resultingPoints.ppoints[1].x = (resultingPoints.ppoints[0].x + resultingPoints.ppoints[2].x) / 2; // X
                    resultingPoints.ppoints[1].y = pitch; // Y
                    resultingPoints.ppoints[1].z = (resultingPoints.ppoints[0].z + resultingPoints.ppoints[2].z) / 2; // Z
                    // R
                    resultingPoints.ppoints[2].x = cos((rotation + 0.125) * PI * 2) * normalisedOutputDiverge; // X
                    resultingPoints.ppoints[2].y = pitch; // Y
                    resultingPoints.ppoints[2].z = sin((rotation + 0.125) * PI * 2) * normalisedOutputDiverge; // Z
                    break;
                default:
                    break;
            }
            break;

        case INPUT_QUAD:
            switch (outputMode) {
                case OUTPUT_4CH:
                    // Quad input >> 4ch output
                    
                    resultingPoints.pointsCount = 4;
                    resultingPoints.pointsNames[0] = "L";
                    resultingPoints.pointsNames[1] = "R";
                    resultingPoints.pointsNames[2] = "Rs";
                    resultingPoints.pointsNames[3] = "Ls";
                    
                    // L
                    resultingPoints.ppoints[0].x = cos((rotation + 0.125f - 0.25f) * PI * 2) * normalisedOutputDiverge; // X
                    resultingPoints.ppoints[0].y = 0; // Y
                    resultingPoints.ppoints[0].z = sin((rotation + 0.125f - 0.25f) * PI * 2) * normalisedOutputDiverge; // Z
                    // R
                    resultingPoints.ppoints[1].x = cos((rotation + 0.125f) * PI * 2) * normalisedOutputDiverge; // X
                    resultingPoints.ppoints[1].y = 0; // Y
                    resultingPoints.ppoints[1].z = sin((rotation + 0.125f) * PI * 2) * normalisedOutputDiverge; // Z
                    // Rs
                    resultingPoints.ppoints[2].x = cos((rotation + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge; // X
                    resultingPoints.ppoints[2].y = 0; // Y
                    resultingPoints.ppoints[2].z = sin((rotation + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge; // Z
                    // Ls
                    resultingPoints.ppoints[3].x = cos((rotation + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge; // X
                    resultingPoints.ppoints[3].y = 0; // Y
                    resultingPoints.ppoints[3].z = sin((rotation + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge; // Z
                    
                    break;
                case OUTPUT_8CH:
                    // Quad input >> 8ch output
                    
                    resultingPoints.pointsCount = 4;
                    resultingPoints.pointsNames[0] = "L";
                    resultingPoints.pointsNames[1] = "R";
                    resultingPoints.pointsNames[2] = "Rs";
                    resultingPoints.pointsNames[3] = "Ls";
                    
                    // L
                    resultingPoints.ppoints[0].x = cos((rotation + 0.125f - 0.25f) * PI * 2) * normalisedOutputDiverge; // X
                    resultingPoints.ppoints[0].y = pitch; // Y
                    resultingPoints.ppoints[0].z = sin((rotation + 0.125f - 0.25f) * PI * 2) * normalisedOutputDiverge; // Z
                    // R
                    resultingPoints.ppoints[1].x = cos((rotation + 0.125f) * PI * 2) * normalisedOutputDiverge; // X
                    resultingPoints.ppoints[1].y = pitch; // Y
                    resultingPoints.ppoints[1].z = sin((rotation + 0.125f) * PI * 2) * normalisedOutputDiverge; // Z
                    // Rs
                    resultingPoints.ppoints[2].x = cos((rotation + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge; // X
                    resultingPoints.ppoints[2].y = pitch; // Y
                    resultingPoints.ppoints[2].z = sin((rotation + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge; // Z
                    // Ls
                    resultingPoints.ppoints[3].x = cos((rotation + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge; // X
                    resultingPoints.ppoints[3].y = pitch; // Y
                    resultingPoints.ppoints[3].z = sin((rotation + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge; // Z
                    break;
                default:
                    break;
            }
            break;
            
        case INPUT_AFORMAT:
            switch (outputMode) {
                case OUTPUT_4CH:
                    // AFormat input >> 4ch output
                    
                    resultingPoints.pointsCount = 4;
                    resultingPoints.pointsNames[0] = "FLU";
                    resultingPoints.pointsNames[1] = "FRD";
                    resultingPoints.pointsNames[2] = "BLD";
                    resultingPoints.pointsNames[3] = "BRU";
                    
                    // FLU
                    resultingPoints.ppoints[0].x = cos((rotation + 0.125f - 0.25f) * PI * 2) * diverge; // X
                    resultingPoints.ppoints[0].y = 0; // Y
                    resultingPoints.ppoints[0].z = sin((rotation + 0.125f - 0.25f) * PI * 2) * diverge; // Z
                    // FRD
                    resultingPoints.ppoints[1].x = cos((rotation + 0.125f) * PI * 2) * diverge; // X
                    resultingPoints.ppoints[1].y = 0; // Y
                    resultingPoints.ppoints[1].z = sin((rotation + 0.125f) * PI * 2) * diverge; // Z
                    // BLD
                    resultingPoints.ppoints[2].x = cos((rotation + 0.125f + 0.5f) * PI * 2) * diverge; // X
                    resultingPoints.ppoints[2].y = 0; // Y
                    resultingPoints.ppoints[2].z = sin((rotation + 0.125f + 0.5f) * PI * 2) * diverge; // Z
                    // BRU
                    resultingPoints.ppoints[3].x = cos((rotation + 0.125f + 0.25f) * PI * 2) * diverge; // X
                    resultingPoints.ppoints[3].y = 0; // Y
                    resultingPoints.ppoints[3].z = sin((rotation + 0.125f + 0.25f) * PI * 2) * diverge; // Z
                    
                    break;
                case OUTPUT_8CH:
                    // AFormat input >> 8ch output
                    
                    resultingPoints.pointsCount = 4;
                    resultingPoints.pointsNames[0] = "FLU";
                    resultingPoints.pointsNames[1] = "FRD";
                    resultingPoints.pointsNames[2] = "BLD";
                    resultingPoints.pointsNames[3] = "BRU";
                    
                    // FLU
                    resultingPoints.ppoints[0].x = cos((rotation + 0.125f - 0.25f) * PI * 2) * diverge; // X
                    resultingPoints.ppoints[0].y = (1 * diverge); // Y
                    resultingPoints.ppoints[0].z = sin((rotation + 0.125f - 0.25f) * PI * 2) * diverge; // Z
                    // FRD
                    resultingPoints.ppoints[1].x = cos((rotation + 0.125f) * PI * 2) * diverge; // X
                    resultingPoints.ppoints[1].y = (-1 * diverge); // Y
                    resultingPoints.ppoints[1].z = sin((rotation + 0.125f) * PI * 2) * diverge; // Z
                    // BLD
                    resultingPoints.ppoints[2].x = cos((rotation + 0.125f + 0.5f) * PI * 2) * diverge; // X
                    resultingPoints.ppoints[2].y = (-1 * diverge); // Y
                    resultingPoints.ppoints[2].z = sin((rotation + 0.125f + 0.5f) * PI * 2) * diverge; // Z
                    // BRU
                    resultingPoints.ppoints[3].x = cos((rotation + 0.125f + 0.25f) * PI * 2) * diverge; // X
                    resultingPoints.ppoints[3].y = (1 * diverge); // Y
                    resultingPoints.ppoints[3].z = sin((rotation + 0.125f + 0.25f) * PI * 2) * diverge; // Z
                    break;
                default:
                    break;
            }
            break;
            
        case INPUT_FOAACN:
            switch (outputMode) {
                case OUTPUT_4CH:
                    // BFormat input >> Quad output
                    
                    resultingPoints.pointsCount = 7;
                    resultingPoints.pointsNames[0] = "W";
                    resultingPoints.pointsNames[1] = "1";
                    resultingPoints.pointsNames[2] = "2";
                    resultingPoints.pointsNames[3] = "3";
                    resultingPoints.pointsNames[4] = "-1"; 
                    resultingPoints.pointsNames[5] = "-2";
                    resultingPoints.pointsNames[6] = "-3";
                    
                    // W
                    resultingPoints.ppoints[0].x = 0; // X
                    resultingPoints.ppoints[0].y = 0; // Y
                    resultingPoints.ppoints[0].z = 0; // Z
                    // Left/Right / L
                    resultingPoints.ppoints[1].x = cos((rotation + 0.250) * PI * 2) * diverge; // X
                    resultingPoints.ppoints[1].y = 0; // Y
                    resultingPoints.ppoints[1].z = sin((rotation + 0.250) * PI * 2) * diverge; // Z
                    // Top/Bottom / T
                    resultingPoints.ppoints[2].x = 0; // X
                    resultingPoints.ppoints[2].y = 0; // Y
                    resultingPoints.ppoints[2].z = 0; // Z
                    // Front/Back / F
                    resultingPoints.ppoints[3].x = cos((rotation + 0.0) * PI * 2) * diverge; // X
                    resultingPoints.ppoints[3].y = 0; // Y
                    resultingPoints.ppoints[3].z = sin((rotation + 0.0) * PI * 2) * diverge; // Z
                    // -Left/Right / R
                    resultingPoints.ppoints[4].x = cos((rotation + 0.75) * PI * 2) * diverge; // X
                    resultingPoints.ppoints[4].y = 0; // Y
                    resultingPoints.ppoints[4].z = sin((rotation + 0.75) * PI * 2) * diverge; // Z
                    // -Top/Bottom / B
                    resultingPoints.ppoints[5].x = 0; // X
                    resultingPoints.ppoints[5].y = 0; // Y
                    resultingPoints.ppoints[5].z = 0; // Z
                    // -Front/Back / B
                    resultingPoints.ppoints[6].x = cos((rotation + 0.5) * PI * 2) * diverge; // X
                    resultingPoints.ppoints[6].y = 0; // Y
                    resultingPoints.ppoints[6].z = sin((rotation + 0.5) * PI * 2) * diverge; // Z
                    
                    break;
                case OUTPUT_8CH:
                    // BFormat input >> 7.1 output
                    
                    resultingPoints.pointsCount = 7;
                    resultingPoints.pointsNames[0] = "W";
                    resultingPoints.pointsNames[1] = "1";
                    resultingPoints.pointsNames[2] = "2";
                    resultingPoints.pointsNames[3] = "3";
                    resultingPoints.pointsNames[4] = "-1";
                    resultingPoints.pointsNames[5] = "-2";
                    resultingPoints.pointsNames[6] = "-3";
                    
                    /*
                    X = left/right angle
                    Y = up/down angle 
                    Z = front/back angle
                     */

                    // W
                    resultingPoints.ppoints[0].x = 0; // X
                    resultingPoints.ppoints[0].y = 0; // Y
                    resultingPoints.ppoints[0].z = 0; // Z
                    // Left/Right / L
                    resultingPoints.ppoints[1].x = cos((rotation + 0.250) * PI * 2) * diverge; // X 1
                    resultingPoints.ppoints[1].y = 0; // Y
                    resultingPoints.ppoints[1].z = sin((rotation + 0.250) * PI * 2) * diverge; // Z 0
                    // Top/Bottom / T
                    resultingPoints.ppoints[2].x = 0; // X
                    resultingPoints.ppoints[2].y = 1.0 * diverge; // Y
                    resultingPoints.ppoints[2].z = 0; // Z
                    // Front/Back / F
                    resultingPoints.ppoints[3].x = cos((rotation + 0.0) * PI * 2) * diverge; // X 1
                    resultingPoints.ppoints[3].y = 0; // Y
                    resultingPoints.ppoints[3].z = sin((rotation + 0.0) * PI * 2) * diverge; // Z 0 
                    // -Left/Right / R
                    resultingPoints.ppoints[4].x = cos((rotation + 0.75) * PI * 2) * diverge; // X
                    resultingPoints.ppoints[4].y = 0; // Y
                    resultingPoints.ppoints[4].z = sin((rotation + 0.75) * PI * 2) * diverge; // Z
                    // -Top/Bottom / B
                    resultingPoints.ppoints[5].x = 0; // X
                    resultingPoints.ppoints[5].y = -1.0 * diverge; // Y
                    resultingPoints.ppoints[5].z = 0; // Z
                    // -Front/Back / B
                    resultingPoints.ppoints[6].x = cos((rotation + 0.5) * PI * 2) * diverge; // X
                    resultingPoints.ppoints[6].y = 0; // Y
                    resultingPoints.ppoints[6].z = sin((rotation + 0.5) * PI * 2) * diverge; // Z
                    break;
                default:
                    break;
            }
            break;
            
        case INPUT_FOAFUMA:
            switch (outputMode) {
                case OUTPUT_4CH:
                    // BFormat input >> Quad output
                    
                    resultingPoints.pointsCount = 7;
                    resultingPoints.pointsNames[0] = "W";
                    resultingPoints.pointsNames[1] = "X";
                    resultingPoints.pointsNames[2] = "Y";
                    resultingPoints.pointsNames[3] = "Z";
                    resultingPoints.pointsNames[4] = "-X";
                    resultingPoints.pointsNames[5] = "-Y";
                    resultingPoints.pointsNames[6] = "-Z";
                    
                    // W
                    resultingPoints.ppoints[0].x = 0; // X x
                    resultingPoints.ppoints[0].y = 0; // Y z
                    resultingPoints.ppoints[0].z = 0; // Z y
                    // Front/Back / F
                    resultingPoints.ppoints[1].x = cos((rotation + 0.0) * PI * 2) * diverge; // X
                    resultingPoints.ppoints[1].y = 0; // Y
                    resultingPoints.ppoints[1].z = sin((rotation + 0.0) * PI * 2) * diverge; // Z
                    // Left/Right / L
                    resultingPoints.ppoints[2].x = cos((rotation + 0.250) * PI * 2) * diverge; // X
                    resultingPoints.ppoints[2].y = 0; // Y
                    resultingPoints.ppoints[2].z = sin((rotation + 0.250) * PI * 2) * diverge; // Z
                    // Top/Bottom / T
                    resultingPoints.ppoints[3].x = 0; // X
                    resultingPoints.ppoints[3].y = 0; // Y
                    resultingPoints.ppoints[3].z = 0; // Z
                    // -Front/Back / B
                    resultingPoints.ppoints[4].x = cos((rotation + 0.5) * PI * 2) * diverge; // X
                    resultingPoints.ppoints[4].y = 0; // Y
                    resultingPoints.ppoints[4].z = sin((rotation + 0.5) * PI * 2) * diverge; // Z
                    // -Left/Right / R
                    resultingPoints.ppoints[5].x = cos((rotation + 0.75) * PI * 2) * diverge; // X
                    resultingPoints.ppoints[5].y = 0; // Y
                    resultingPoints.ppoints[5].z = sin((rotation + 0.75) * PI * 2) * diverge; // Z
                    // -Top/Bottom / B
                    resultingPoints.ppoints[6].x = 0; // X
                    resultingPoints.ppoints[6].y = 0; // Y
                    resultingPoints.ppoints[6].z = 0; // Z
                    
                    break;
                case OUTPUT_8CH:
                    // BFormat input >> 7.1 output
                    
                    resultingPoints.pointsCount = 7;
                    resultingPoints.pointsNames[0] = "W";
                    resultingPoints.pointsNames[1] = "Y";
                    resultingPoints.pointsNames[2] = "Z";
                    resultingPoints.pointsNames[3] = "X";
                    resultingPoints.pointsNames[4] = "-Y";
                    resultingPoints.pointsNames[5] = "-Z";
                    resultingPoints.pointsNames[6] = "-X";
                    
                    // W
                    resultingPoints.ppoints[0].x = 0; // X x
                    resultingPoints.ppoints[0].y = 0; // Y z
                    resultingPoints.ppoints[0].z = 0; // Z y
                    // Front/Back / F
                    resultingPoints.ppoints[1].x = cos((rotation + 0.0) * PI * 2) * diverge; // X
                    resultingPoints.ppoints[1].y = 0; // Y
                    resultingPoints.ppoints[1].z = sin((rotation + 0.0) * PI * 2) * diverge; // Z
                    // Left/Right / L
                    resultingPoints.ppoints[2].x = cos((rotation + 0.25) * PI * 2) * diverge; // X
                    resultingPoints.ppoints[2].y = 0; // Y
                    resultingPoints.ppoints[2].z = sin((rotation + 0.25) * PI * 2) * diverge; // Z
                    // Top/Bottom / T
                    resultingPoints.ppoints[3].x = 0; // X
                    resultingPoints.ppoints[3].y = 1.0 * diverge; // Y
                    resultingPoints.ppoints[3].z = 0; // Z
                    // -Front/Back / B
                    resultingPoints.ppoints[4].x = cos((rotation + 0.5) * PI * 2) * diverge; // X
                    resultingPoints.ppoints[4].y = 0; // Y
                    resultingPoints.ppoints[4].z = sin((rotation + 0.5) * PI * 2) * diverge; // Z
                    // -Left/Right / R
                    resultingPoints.ppoints[5].x = cos((rotation + 0.75) * PI * 2) * diverge; // X
                    resultingPoints.ppoints[5].y = 0; // Y
                    resultingPoints.ppoints[5].z = sin((rotation + 0.75) * PI * 2) * diverge; // Z
                    // -Top/Bottom / B
                    resultingPoints.ppoints[6].x = 0; // X
                    resultingPoints.ppoints[6].y = -1.0 * diverge; // Y
                    resultingPoints.ppoints[6].z = 0; // Z
                    break;
                default:
                    break;
        }
        break;

        case INPUT_LCRS:
            switch (outputMode) {
                case OUTPUT_4CH:
                    // Quad input >> 4ch output
                    
                    resultingPoints.pointsCount = 4;
                    resultingPoints.pointsNames[0] = "L";
                    resultingPoints.pointsNames[1] = "C";
                    resultingPoints.pointsNames[2] = "R";
                    resultingPoints.pointsNames[3] = "S";
                    
                    // L
                    resultingPoints.ppoints[0].x = cos((rotation - 0.125) * PI * 2) * normalisedOutputDiverge; // X
                    resultingPoints.ppoints[0].y = 0; // Y
                    resultingPoints.ppoints[0].z = sin((rotation - 0.125) * PI * 2) * normalisedOutputDiverge; // Z
                    // C
                    resultingPoints.ppoints[1].x = (resultingPoints.ppoints[0].x + resultingPoints.ppoints[2].x) / 2; // X
                    resultingPoints.ppoints[1].y = 0; // Y
                    resultingPoints.ppoints[1].z = (resultingPoints.ppoints[0].z + resultingPoints.ppoints[2].z) / 2; // Z
                    // R
                    resultingPoints.ppoints[2].x = cos((rotation + 0.125) * PI * 2) * normalisedOutputDiverge; // X
                    resultingPoints.ppoints[2].y = 0; // Y
                    resultingPoints.ppoints[2].z = sin((rotation + 0.125) * PI * 2) * normalisedOutputDiverge; // Z
                    // S
                    resultingPoints.ppoints[3].x = -resultingPoints.ppoints[1].x; // X
                    resultingPoints.ppoints[3].y = 0; // Y
                    resultingPoints.ppoints[3].z = -resultingPoints.ppoints[1].z; // Z
                    
                    break;
                case OUTPUT_8CH:
                    // Quad input >> 8ch output
                    
                    resultingPoints.pointsCount = 4;
                    resultingPoints.pointsNames[0] = "L";
                    resultingPoints.pointsNames[1] = "C";
                    resultingPoints.pointsNames[2] = "R";
                    resultingPoints.pointsNames[3] = "S";
                    
                    // L
                    resultingPoints.ppoints[0].x = cos((rotation + 0.125 - 0.25) * PI * 2) * normalisedOutputDiverge; // X
                    resultingPoints.ppoints[0].y = pitch; // Y
                    resultingPoints.ppoints[0].z = sin((rotation + 0.125 - 0.25) * PI * 2) * normalisedOutputDiverge; // Z
                    // C
                    resultingPoints.ppoints[1].x = (resultingPoints.ppoints[0].x + resultingPoints.ppoints[2].x) / 2; // X
                    resultingPoints.ppoints[1].y = pitch; // Y
                    resultingPoints.ppoints[1].z = (resultingPoints.ppoints[0].z + resultingPoints.ppoints[2].z) / 2; // Z
                    // R
                    resultingPoints.ppoints[2].x = cos((rotation + 0.125) * PI * 2) * normalisedOutputDiverge; // X
                    resultingPoints.ppoints[2].y = pitch; // Y
                    resultingPoints.ppoints[2].z = sin((rotation + 0.125) * PI * 2) * normalisedOutputDiverge; // Z
                    // S
                    resultingPoints.ppoints[3].x = -resultingPoints.ppoints[1].x; // X
                    resultingPoints.ppoints[3].y = pitch; // Y
                    resultingPoints.ppoints[3].z = -resultingPoints.ppoints[1].z; // Z
                    break;
                default:
                    break;
            }
            break;
        

        case INPUT_2OAACN:
            switch (outputMode) {
                case OUTPUT_4CH:
                    // BFormat Second Order input >> Quad output
                    
                    resultingPoints.pointsCount = 9;
                    resultingPoints.pointsNames[0] = "W";
                    resultingPoints.pointsNames[1] = "1";
                    resultingPoints.pointsNames[2] = "2";
                    resultingPoints.pointsNames[3] = "3";
                    resultingPoints.pointsNames[4] = "4"; 
                    resultingPoints.pointsNames[5] = "5";
                    resultingPoints.pointsNames[6] = "6"; 
                    resultingPoints.pointsNames[7] = "7"; 
                    resultingPoints.pointsNames[8] = "8";

                    process2OAAmbisonicGains(resultingPoints.ppoints, transcoded);

                    resultingPoints.transcoded[0].x = cos((rotation + 0.125f - 0.25f) * PI * 2) * normalisedOutputDiverge; // X
                    resultingPoints.transcoded[0].y = 0; // Y
                    resultingPoints.transcoded[0].z = sin((rotation + 0.125f - 0.25f) * PI * 2) * normalisedOutputDiverge; // Z
                    resultingPoints.transcoded[1].x = cos((rotation + 0.125f) * PI * 2) * normalisedOutputDiverge; // X
                    resultingPoints.transcoded[1].y = 0; // Y
                    resultingPoints.transcoded[1].z = sin((rotation + 0.125f) * PI * 2) * normalisedOutputDiverge; // Z
                    resultingPoints.transcoded[2].x = cos((rotation + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge; // X
                    resultingPoints.transcoded[2].y = 0; // Y
                    resultingPoints.transcoded[2].z = sin((rotation + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge; // Z
                    resultingPoints.transcoded[3].x = cos((rotation + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge; // X
                    resultingPoints.transcoded[3].y = 0; // Y
                    resultingPoints.transcoded[3].z = sin((rotation + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge; // Z
                    resultingPoints.transcoded[4].x = cos((rotation + 0.125f - 0.25f) * PI * 2) * normalisedOutputDiverge; // X
                    resultingPoints.transcoded[4].y = 0; // Y
                    resultingPoints.transcoded[4].z = sin((rotation + 0.125f - 0.25f) * PI * 2) * normalisedOutputDiverge; // Z
                    resultingPoints.transcoded[5].x = cos((rotation + 0.125f) * PI * 2) * normalisedOutputDiverge; // X
                    resultingPoints.transcoded[5].y = 0; // Y
                    resultingPoints.transcoded[5].z = sin((rotation + 0.125f) * PI * 2) * normalisedOutputDiverge; // Z
                    resultingPoints.transcoded[6].x = cos((rotation + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge; // X
                    resultingPoints.transcoded[6].y = 0; // Y
                    resultingPoints.transcoded[6].z = sin((rotation + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge; // Z
                    resultingPoints.transcoded[7].x = cos((rotation + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge; // X
                    resultingPoints.transcoded[7].y = 0; // Y
                    resultingPoints.transcoded[7].z = sin((rotation + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge; // Z
                    
                    break;
                case OUTPUT_8CH:
                    // BFormat Second Order input >> 7.1 output
                    
                    resultingPoints.pointsCount = 9;
                    resultingPoints.pointsNames[0] = "W";
                    resultingPoints.pointsNames[1] = "1";
                    resultingPoints.pointsNames[2] = "2";
                    resultingPoints.pointsNames[3] = "3";
                    resultingPoints.pointsNames[4] = "4"; 
                    resultingPoints.pointsNames[5] = "5";                     
                    resultingPoints.pointsNames[6] = "6"; 
                    resultingPoints.pointsNames[7] = "7"; 
                    resultingPoints.pointsNames[8] = "8"; 

                    process2OAAmbisonicGains(resultingPoints.ppoints, transcoded);

                    resultingPoints.transcoded[0].x = cos((rotation + 0.125f - 0.25f) * PI * 2) * normalisedOutputDiverge; // X
                    resultingPoints.transcoded[0].y = 1.0 * diverge; // Y
                    resultingPoints.transcoded[0].z = sin((rotation + 0.125f - 0.25f) * PI * 2) * normalisedOutputDiverge; // Z
                    resultingPoints.transcoded[1].x = cos((rotation + 0.125f) * PI * 2) * normalisedOutputDiverge; // X
                    resultingPoints.transcoded[1].y = 1.0 * diverge; // Y
                    resultingPoints.transcoded[1].z = sin((rotation + 0.125f) * PI * 2) * normalisedOutputDiverge; // Z
                    resultingPoints.transcoded[2].x = cos((rotation + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge; // X
                    resultingPoints.transcoded[2].y = 1.0 * diverge; // Y
                    resultingPoints.transcoded[2].z = sin((rotation + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge; // Z
                    resultingPoints.transcoded[3].x = cos((rotation + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge; // X
                    resultingPoints.transcoded[3].y = 1.0 * diverge; // Y
                    resultingPoints.transcoded[3].z = sin((rotation + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge; // Z
                    resultingPoints.transcoded[4].x = cos((rotation + 0.125f - 0.25f) * PI * 2) * normalisedOutputDiverge; // X
                    resultingPoints.transcoded[4].y = -1.0 * diverge; // Y
                    resultingPoints.transcoded[4].z = sin((rotation + 0.125f - 0.25f) * PI * 2) * normalisedOutputDiverge; // Z
                    resultingPoints.transcoded[5].x = cos((rotation + 0.125f) * PI * 2) * normalisedOutputDiverge; // X
                    resultingPoints.transcoded[5].y = -1.0 * diverge; // Y
                    resultingPoints.transcoded[5].z = sin((rotation + 0.125f) * PI * 2) * normalisedOutputDiverge; // Z
                    resultingPoints.transcoded[6].x = cos((rotation + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge; // X
                    resultingPoints.transcoded[6].y = -1.0 * diverge; // Y
                    resultingPoints.transcoded[6].z = sin((rotation + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge; // Z
                    resultingPoints.transcoded[7].x = cos((rotation + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge; // X
                    resultingPoints.transcoded[7].y = -1.0 * diverge; // Y
                    resultingPoints.transcoded[7].z = sin((rotation + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge; // Z
                    break;
                default:
                    break;
            }
            break;
            
        case INPUT_2OAFUMA:
            switch (outputMode) {
                case OUTPUT_4CH:
                    // BFormat Second Order input >> Quad output
                    
                    resultingPoints.pointsCount = 9;
                    resultingPoints.pointsNames[0] = "W";
                    resultingPoints.pointsNames[1] = "1";
                    resultingPoints.pointsNames[2] = "2";
                    resultingPoints.pointsNames[3] = "3";
                    resultingPoints.pointsNames[4] = "4"; 
                    resultingPoints.pointsNames[5] = "5";
                    resultingPoints.pointsNames[6] = "6"; 
                    resultingPoints.pointsNames[7] = "7"; 
                    resultingPoints.pointsNames[8] = "8";

                    process2OAAmbisonicGains(resultingPoints.ppoints, transcoded);

                    resultingPoints.transcoded[0].x = cos((rotation + 0.125f - 0.25f) * PI * 2) * normalisedOutputDiverge; // X
                    resultingPoints.transcoded[0].y = 0; // Y
                    resultingPoints.transcoded[0].z = sin((rotation + 0.125f - 0.25f) * PI * 2) * normalisedOutputDiverge; // Z
                    resultingPoints.transcoded[1].x = cos((rotation + 0.125f) * PI * 2) * normalisedOutputDiverge; // X
                    resultingPoints.transcoded[1].y = 0; // Y
                    resultingPoints.transcoded[1].z = sin((rotation + 0.125f) * PI * 2) * normalisedOutputDiverge; // Z
                    resultingPoints.transcoded[2].x = cos((rotation + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge; // X
                    resultingPoints.transcoded[2].y = 0; // Y
                    resultingPoints.transcoded[2].z = sin((rotation + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge; // Z
                    resultingPoints.transcoded[3].x = cos((rotation + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge; // X
                    resultingPoints.transcoded[3].y = 0; // Y
                    resultingPoints.transcoded[3].z = sin((rotation + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge; // Z
                    resultingPoints.transcoded[4].x = cos((rotation + 0.125f - 0.25f) * PI * 2) * normalisedOutputDiverge; // X
                    resultingPoints.transcoded[4].y = 0; // Y
                    resultingPoints.transcoded[4].z = sin((rotation + 0.125f - 0.25f) * PI * 2) * normalisedOutputDiverge; // Z
                    resultingPoints.transcoded[5].x = cos((rotation + 0.125f) * PI * 2) * normalisedOutputDiverge; // X
                    resultingPoints.transcoded[5].y = 0; // Y
                    resultingPoints.transcoded[5].z = sin((rotation + 0.125f) * PI * 2) * normalisedOutputDiverge; // Z
                    resultingPoints.transcoded[6].x = cos((rotation + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge; // X
                    resultingPoints.transcoded[6].y = 0; // Y
                    resultingPoints.transcoded[6].z = sin((rotation + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge; // Z
                    resultingPoints.transcoded[7].x = cos((rotation + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge; // X
                    resultingPoints.transcoded[7].y = 0; // Y
                    resultingPoints.transcoded[7].z = sin((rotation + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge; // Z
                    
                    break;
                case OUTPUT_8CH:
                    // BFormat Second Order input >> 7.1 output
                    
                    resultingPoints.pointsCount = 9;
                    resultingPoints.pointsNames[0] = "W";
                    resultingPoints.pointsNames[1] = "1";
                    resultingPoints.pointsNames[2] = "2";
                    resultingPoints.pointsNames[3] = "3";
                    resultingPoints.pointsNames[4] = "4"; 
                    resultingPoints.pointsNames[5] = "5";                     
                    resultingPoints.pointsNames[6] = "6"; 
                    resultingPoints.pointsNames[7] = "7"; 
                    resultingPoints.pointsNames[8] = "8"; 

                    process2OAAmbisonicGains(resultingPoints.ppoints, transcoded);

                    resultingPoints.transcoded[0].x = cos((rotation + 0.125f - 0.25f) * PI * 2) * normalisedOutputDiverge; // X
                    resultingPoints.transcoded[0].y = 1.0 * diverge; // Y
                    resultingPoints.transcoded[0].z = sin((rotation + 0.125f - 0.25f) * PI * 2) * normalisedOutputDiverge; // Z
                    resultingPoints.transcoded[1].x = cos((rotation + 0.125f) * PI * 2) * normalisedOutputDiverge; // X
                    resultingPoints.transcoded[1].y = 1.0 * diverge; // Y
                    resultingPoints.transcoded[1].z = sin((rotation + 0.125f) * PI * 2) * normalisedOutputDiverge; // Z
                    resultingPoints.transcoded[2].x = cos((rotation + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge; // X
                    resultingPoints.transcoded[2].y = 1.0 * diverge; // Y
                    resultingPoints.transcoded[2].z = sin((rotation + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge; // Z
                    resultingPoints.transcoded[3].x = cos((rotation + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge; // X
                    resultingPoints.transcoded[3].y = 1.0 * diverge; // Y
                    resultingPoints.transcoded[3].z = sin((rotation + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge; // Z
                    resultingPoints.transcoded[4].x = cos((rotation + 0.125f - 0.25f) * PI * 2) * normalisedOutputDiverge; // X
                    resultingPoints.transcoded[4].y = -1.0 * diverge; // Y
                    resultingPoints.transcoded[4].z = sin((rotation + 0.125f - 0.25f) * PI * 2) * normalisedOutputDiverge; // Z
                    resultingPoints.transcoded[5].x = cos((rotation + 0.125f) * PI * 2) * normalisedOutputDiverge; // X
                    resultingPoints.transcoded[5].y = -1.0 * diverge; // Y
                    resultingPoints.transcoded[5].z = sin((rotation + 0.125f) * PI * 2) * normalisedOutputDiverge; // Z
                    resultingPoints.transcoded[6].x = cos((rotation + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge; // X
                    resultingPoints.transcoded[6].y = -1.0 * diverge; // Y
                    resultingPoints.transcoded[6].z = sin((rotation + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge; // Z
                    resultingPoints.transcoded[7].x = cos((rotation + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge; // X
                    resultingPoints.transcoded[7].y = -1.0 * diverge; // Y
                    resultingPoints.transcoded[7].z = sin((rotation + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge; // Z
                    break;
                default:
                    break;
        }
        break;


        case INPUT_3OAACN:
            switch (outputMode) {
                case OUTPUT_4CH:
                    // BFormat input >> Quad output
                    
                    resultingPoints.pointsCount = 7;
                    resultingPoints.pointsNames[0] = "W";
                    resultingPoints.pointsNames[1] = "1";
                    resultingPoints.pointsNames[2] = "2";
                    resultingPoints.pointsNames[3] = "3";
                    resultingPoints.pointsNames[4] = "-1";
                    resultingPoints.pointsNames[5] = "-2";
                    resultingPoints.pointsNames[6] = "-3";
                    
                    // W
                    resultingPoints.ppoints[0].x = 0; // X
                    resultingPoints.ppoints[0].y = 0; // Y
                    resultingPoints.ppoints[0].z = 0; // Z
                    // Left/Right / L
                    resultingPoints.ppoints[1].x = cos((rotation + 0.250) * PI * 2) * diverge; // X
                    resultingPoints.ppoints[1].y = 0; // Y
                    resultingPoints.ppoints[1].z = sin((rotation + 0.250) * PI * 2) * diverge; // Z
                    // Top/Bottom / T
                    resultingPoints.ppoints[2].x = 0; // X
                    resultingPoints.ppoints[2].y = 0; // Y
                    resultingPoints.ppoints[2].z = 0; // Z
                    // Front/Back / F
                    resultingPoints.ppoints[3].x = cos((rotation + 0.0) * PI * 2) * diverge; // X
                    resultingPoints.ppoints[3].y = 0; // Y
                    resultingPoints.ppoints[3].z = sin((rotation + 0.0) * PI * 2) * diverge; // Z
                    // -Left/Right / R
                    resultingPoints.ppoints[4].x = cos((rotation + 0.75) * PI * 2) * diverge; // X
                    resultingPoints.ppoints[4].y = 0; // Y
                    resultingPoints.ppoints[4].z = sin((rotation + 0.75) * PI * 2) * diverge; // Z
                    // -Top/Bottom / B
                    resultingPoints.ppoints[5].x = 0; // X
                    resultingPoints.ppoints[5].y = 0; // Y
                    resultingPoints.ppoints[5].z = 0; // Z
                    // -Front/Back / B
                    resultingPoints.ppoints[6].x = cos((rotation + 0.5) * PI * 2) * diverge; // X
                    resultingPoints.ppoints[6].y = 0; // Y
                    resultingPoints.ppoints[6].z = sin((rotation + 0.5) * PI * 2) * diverge; // Z
                    
                    break;
                case OUTPUT_8CH:
                    // BFormat input >> 7.1 output
                    
                    resultingPoints.pointsCount = 7;
                    resultingPoints.pointsNames[0] = "W";
                    resultingPoints.pointsNames[1] = "1";
                    resultingPoints.pointsNames[2] = "2";
                    resultingPoints.pointsNames[3] = "3";
                    resultingPoints.pointsNames[4] = "-1";
                    resultingPoints.pointsNames[5] = "-2";
                    resultingPoints.pointsNames[6] = "-3";
                    
                    // W
                    resultingPoints.ppoints[0].x = 0; // X
                    resultingPoints.ppoints[0].y = 0; // Y
                    resultingPoints.ppoints[0].z = 0; // Z
                    // Left/Right / L
                    resultingPoints.ppoints[1].x = cos((rotation + 0.250) * PI * 2) * diverge; // X
                    resultingPoints.ppoints[1].y = 0; // Y
                    resultingPoints.ppoints[1].z = sin((rotation + 0.250) * PI * 2) * diverge; // Z
                    // Top/Bottom / T
                    resultingPoints.ppoints[2].x = 0; // X
                    resultingPoints.ppoints[2].y = 1.0 * diverge; // Y
                    resultingPoints.ppoints[2].z = 0; // Z
                    // Front/Back / F
                    resultingPoints.ppoints[3].x = cos((rotation + 0.0) * PI * 2) * diverge; // X
                    resultingPoints.ppoints[3].y = 0; // Y
                    resultingPoints.ppoints[3].z = sin((rotation + 0.0) * PI * 2) * diverge; // Z
                    // -Left/Right / R
                    resultingPoints.ppoints[4].x = cos((rotation + 0.75) * PI * 2) * diverge; // X
                    resultingPoints.ppoints[4].y = 0; // Y
                    resultingPoints.ppoints[4].z = sin((rotation + 0.75) * PI * 2) * diverge; // Z
                    // -Top/Bottom / B
                    resultingPoints.ppoints[5].x = 0; // X
                    resultingPoints.ppoints[5].y = -1.0 * diverge; // Y
                    resultingPoints.ppoints[5].z = 0; // Z
                    // -Front/Back / B
                    resultingPoints.ppoints[6].x = cos((rotation + 0.5) * PI * 2) * diverge; // X
                    resultingPoints.ppoints[6].y = 0; // Y
                    resultingPoints.ppoints[6].z = sin((rotation + 0.5) * PI * 2) * diverge; // Z
                    break;
                default:
                    break;
            }
            break;
            
        case INPUT_3OAFUMA:
            switch (outputMode) {
                case OUTPUT_4CH:
                    // BFormat input >> Quad output
                    
                    resultingPoints.pointsCount = 7;
                    resultingPoints.pointsNames[0] = "W";
                    resultingPoints.pointsNames[1] = "X";
                    resultingPoints.pointsNames[2] = "Y";
                    resultingPoints.pointsNames[3] = "Z";
                    resultingPoints.pointsNames[4] = "-X";
                    resultingPoints.pointsNames[5] = "-Y";
                    resultingPoints.pointsNames[6] = "-Z";
                    
                    // W
                    resultingPoints.ppoints[0].x = 0; // X x
                    resultingPoints.ppoints[0].y = 0; // Y z
                    resultingPoints.ppoints[0].z = 0; // Z y
                    // Front/Back / F
                    resultingPoints.ppoints[1].x = cos((rotation + 0.0) * PI * 2) * diverge; // X
                    resultingPoints.ppoints[1].y = 0; // Y
                    resultingPoints.ppoints[1].z = sin((rotation + 0.0) * PI * 2) * diverge; // Z
                    // Left/Right / L
                    resultingPoints.ppoints[2].x = cos((rotation + 0.250) * PI * 2) * diverge; // X
                    resultingPoints.ppoints[2].y = 0; // Y
                    resultingPoints.ppoints[2].z = sin((rotation + 0.250) * PI * 2) * diverge; // Z
                    // Top/Bottom / T
                    resultingPoints.ppoints[3].x = 0; // X
                    resultingPoints.ppoints[3].y = 0; // Y
                    resultingPoints.ppoints[3].z = 0; // Z
                    // -Front/Back / B
                    resultingPoints.ppoints[4].x = cos((rotation + 0.5) * PI * 2) * diverge; // X
                    resultingPoints.ppoints[4].y = 0; // Y
                    resultingPoints.ppoints[4].z = sin((rotation + 0.5) * PI * 2) * diverge; // Z
                    // -Left/Right / R
                    resultingPoints.ppoints[5].x = cos((rotation + 0.75) * PI * 2) * diverge; // X
                    resultingPoints.ppoints[5].y = 0; // Y
                    resultingPoints.ppoints[5].z = sin((rotation + 0.75) * PI * 2) * diverge; // Z
                    // -Top/Bottom / B
                    resultingPoints.ppoints[6].x = 0; // X
                    resultingPoints.ppoints[6].y = 0; // Y
                    resultingPoints.ppoints[6].z = 0; // Z
                    
                    break;
                case OUTPUT_8CH:
                    // BFormat input >> 7.1 output
                    
                    resultingPoints.pointsCount = 7;
                    resultingPoints.pointsNames[0] = "W";
                    resultingPoints.pointsNames[1] = "Y";
                    resultingPoints.pointsNames[2] = "Z";
                    resultingPoints.pointsNames[3] = "X";
                    resultingPoints.pointsNames[4] = "-Y";
                    resultingPoints.pointsNames[5] = "-Z";
                    resultingPoints.pointsNames[6] = "-X";
                    
                    // W
                    resultingPoints.ppoints[0].x = 0; // X x
                    resultingPoints.ppoints[0].y = 0; // Y z
                    resultingPoints.ppoints[0].z = 0; // Z y
                    // Front/Back / F
                    resultingPoints.ppoints[1].x = cos((rotation + 0.0) * PI * 2) * diverge; // X
                    resultingPoints.ppoints[1].y = 0; // Y
                    resultingPoints.ppoints[1].z = sin((rotation + 0.0) * PI * 2) * diverge; // Z
                    // Left/Right / L
                    resultingPoints.ppoints[2].x = cos((rotation + 0.25) * PI * 2) * diverge; // X
                    resultingPoints.ppoints[2].y = 0; // Y
                    resultingPoints.ppoints[2].z = sin((rotation + 0.25) * PI * 2) * diverge; // Z
                    // Top/Bottom / T
                    resultingPoints.ppoints[3].x = 0; // X
                    resultingPoints.ppoints[3].y = 1.0 * diverge; // Y
                    resultingPoints.ppoints[3].z = 0; // Z
                    // -Front/Back / B
                    resultingPoints.ppoints[4].x = cos((rotation + 0.5) * PI * 2) * diverge; // X
                    resultingPoints.ppoints[4].y = 0; // Y
                    resultingPoints.ppoints[4].z = sin((rotation + 0.5) * PI * 2) * diverge; // Z
                    // -Left/Right / R
                    resultingPoints.ppoints[5].x = cos((rotation + 0.75) * PI * 2) * diverge; // X
                    resultingPoints.ppoints[5].y = 0; // Y
                    resultingPoints.ppoints[5].z = sin((rotation + 0.75) * PI * 2) * diverge; // Z
                    // -Top/Bottom / B
                    resultingPoints.ppoints[6].x = 0; // X
                    resultingPoints.ppoints[6].y = -1.0 * diverge; // Y
                    resultingPoints.ppoints[6].z = 0; // Z
                    break;
                default:
                    break;
        }
        break;

        default:
            break;
    };

	// Generating channel gains
	if (outputMode == OUTPUT_4CH) outputChannelCount = 4;
	if (outputMode == OUTPUT_8CH) outputChannelCount = 8;

	resultingPoints.gains.resize(resultingPoints.pointsCount);
	for (int i = 0; i < resultingPoints.pointsCount; i++) {

		resultingPoints.ppoints[i].x = resultingPoints.ppoints[i].x / 2 + 0.5f;
		resultingPoints.ppoints[i].y = resultingPoints.ppoints[i].y / 2 + 0.5f;
		resultingPoints.ppoints[i].z = resultingPoints.ppoints[i].z / 2 + 0.5f;

		resultingPoints.gains[i].resize(outputChannelCount);

		// Generating gains for 4 channel output
		if (outputMode == OUTPUT_4CH) {
			float gains[4];
			processGains4Channels(resultingPoints.ppoints[i].x,
				resultingPoints.ppoints[i].z, gains);
			for (int j = 0; j < outputChannelCount; j++) {
				resultingPoints.gains[i][j] = gains[j];
			}
		}

		// Generating gains for 8 channel output
		if (outputMode == OUTPUT_8CH) {
			float gains[8];
			processGains8Channels(resultingPoints.ppoints[i].x,
				resultingPoints.ppoints[i].z,
				resultingPoints.ppoints[i].y, gains);
			for (int j = 0; j < outputChannelCount; j++) {
				resultingPoints.gains[i][j] = gains[j];
			}
		}
	}
	timeLastCalculation = getCurrentTime() - tStart;
}

void M1EncodeCore::getResultingVolumesDecoded(Mach1DecodeAlgoType decodeType, float* decodeResult, float* result)
{
	// clear
	for (int i = 0; i < 14; i++) result[i] = 0;

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
	default:
		break;
	}

	// decode - 8, 16
	
	if (outputChannelCount * 2 != decodeResultSize) {
		std::cout << "This encode type is not suitable for decode type!" << std::endl;
	}

	for (int j = 0; j < resultingPoints.pointsCount; j++) {
		for (int i = 0; i < outputChannelCount; i++) {
			result[j * 2 + 0] += decodeResult[i * 2 + 0] * resultingPoints.gains[j][i]; // left
			result[j * 2 + 1] += decodeResult[i * 2 + 1] * resultingPoints.gains[j][i]; // right
		}
	}
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