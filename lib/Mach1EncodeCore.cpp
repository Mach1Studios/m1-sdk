//  Mach1 SDK
//  Copyright © 2018 Mach1. All rights reserved.
//
//  Implementation

/*
DISCLAIMER:
This header file is not an example of use but an decoder that will require periodic
updates and should not be integrated in sections but remain as an update-able factored file.
*/

#include "Mach1EncodeCore.h"
#include <math.h>

M1EncodeCorePointResults::M1EncodeCorePointResults() {

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
	result.resize(gains[0].size());
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
	outputMode = OutputMode::OUTPUT_8CH;

	rotation = 0;
	diverge = 0;
	pitch = 0;

	sRotate = 0;
	sSpread = 0;
	autoOrbit = false;
	isotropicEncode = true; 

	outputChannelCount = 8;
}

M1EncodeCore::~M1EncodeCore() {

}

void M1EncodeCore::generatePointResults() {

    float normalisedOutputDiverge = diverge * (1 / cos(PI * 0.25));
    
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
                        resultingPoints.ppoints[0].x = cos((rotation) * PI * 2) * sin((-pitch + 1) * PI * 0.5) * normalisedOutputDiverge; // X
                        resultingPoints.ppoints[0].y = cos((-pitch + 1) * PI * 0.5) * normalisedOutputDiverge; // Y
                        resultingPoints.ppoints[0].z = sin((rotation) * PI * 2)  * sin((-pitch + 1) * PI * 0.5)* normalisedOutputDiverge; // Z
                        
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
                        float sRotationInRadians = sRotate * DEG_TO_RAD - PI / 2.;
                        
                        resultingPoints.ppoints[0].x = center[0] + cos((sRotationInRadians)) * sSpread; // X
                        resultingPoints.ppoints[0].y = 0; // Y
                        resultingPoints.ppoints[0].z = center[2] + sin((sRotationInRadians)) * sSpread; // Z
                        
                        resultingPoints.ppoints[1].x = center[0] - cos((sRotationInRadians)) * sSpread; // X
                        resultingPoints.ppoints[1].y = 0; // Y
                        resultingPoints.ppoints[1].z = center[2] - sin((sRotationInRadians)) * sSpread; // Z
                        
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
                        sRotationInRadians = sRotate * DEG_TO_RAD - PI / 2.;
                    }
                    
                    if (isotropicEncode) {
                        center[0] = cos((rotation) * PI * 2) * sin((pitch + 1) * PI * 0.5) * normalisedOutputDiverge; // X
                        center[1] = cos((pitch + 1) * PI * 0.5) * normalisedOutputDiverge; // Y
                        center[2] = sin((rotation) * PI * 2)  * sin((pitch + 1) * PI * 0.5) * normalisedOutputDiverge; // Z
                        
                        //                            resultingPoints.ppoints[0][0] = center[0] + cos((sRotationInRadians)) * sSpread; // X
                        //                            resultingPoints.ppoints[0][1] = center[1]; // Y
                        //                            resultingPoints.ppoints[0][2] = center[2] + sin((sRotationInRadians)) * sSpread; // Z
                        //
                        //                            resultingPoints.ppoints[1][0] = center[0] - cos((sRotationInRadians)) * sSpread; // X
                        //                            resultingPoints.ppoints[1][1] = pitch; // Y
                        //                            resultingPoints.ppoints[1][2] = center[2] - sin((sRotationInRadians)) * sSpread; // Z
                        
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
                    resultingPoints.ppoints[0].x = cos((rotation + 0.125 - 0.25) * PI * 2) * normalisedOutputDiverge; // X
                    resultingPoints.ppoints[0].y = 0; // Y
                    resultingPoints.ppoints[0].z = sin((rotation + 0.125 - 0.25) * PI * 2) * normalisedOutputDiverge; // Z
                    // R
                    resultingPoints.ppoints[1].x = cos((rotation + 0.125) * PI * 2) * normalisedOutputDiverge; // X
                    resultingPoints.ppoints[1].y = 0; // Y
                    resultingPoints.ppoints[1].z = sin((rotation + 0.125) * PI * 2) * normalisedOutputDiverge; // Z
                    // Rs
                    resultingPoints.ppoints[2].x = cos((rotation + 0.125 + 0.25) * PI * 2) * normalisedOutputDiverge; // X
                    resultingPoints.ppoints[2].y = 0; // Y
                    resultingPoints.ppoints[2].z = sin((rotation + 0.125 + 0.25) * PI * 2) * normalisedOutputDiverge; // Z
                    // Ls
                    resultingPoints.ppoints[3].x = cos((rotation + 0.125 + 0.5) * PI * 2) * normalisedOutputDiverge; // X
                    resultingPoints.ppoints[3].y = 0; // Y
                    resultingPoints.ppoints[3].z = sin((rotation + 0.125 + 0.5) * PI * 2) * normalisedOutputDiverge; // Z
                    
                    break;
                case OUTPUT_8CH:
                    // Quad input >> 8ch output
                    
                    resultingPoints.pointsCount = 4;
                    resultingPoints.pointsNames[0] = "L";
                    resultingPoints.pointsNames[1] = "R";
                    resultingPoints.pointsNames[2] = "Rs";
                    resultingPoints.pointsNames[3] = "Ls";
                    
                    // L
                    resultingPoints.ppoints[0].x = cos((rotation + 0.125 - 0.25) * PI * 2) * normalisedOutputDiverge; // X
                    resultingPoints.ppoints[0].y = pitch; // Y
                    resultingPoints.ppoints[0].z = sin((rotation + 0.125 - 0.25) * PI * 2) * normalisedOutputDiverge; // Z
                    // R
                    resultingPoints.ppoints[1].x = cos((rotation + 0.125) * PI * 2) * normalisedOutputDiverge; // X
                    resultingPoints.ppoints[1].y = pitch; // Y
                    resultingPoints.ppoints[1].z = sin((rotation + 0.125) * PI * 2) * normalisedOutputDiverge; // Z
                    // Rs
                    resultingPoints.ppoints[2].x = cos((rotation + 0.125 + 0.25) * PI * 2) * normalisedOutputDiverge; // X
                    resultingPoints.ppoints[2].y = pitch; // Y
                    resultingPoints.ppoints[2].z = sin((rotation + 0.125 + 0.25) * PI * 2) * normalisedOutputDiverge; // Z
                    // Ls
                    resultingPoints.ppoints[3].x = cos((rotation + 0.125 + 0.5) * PI * 2) * normalisedOutputDiverge; // X
                    resultingPoints.ppoints[3].y = pitch; // Y
                    resultingPoints.ppoints[3].z = sin((rotation + 0.125 + 0.5) * PI * 2) * normalisedOutputDiverge; // Z
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
                    resultingPoints.ppoints[0].x = cos((rotation + 0.125 - 0.25) * PI * 2) * diverge; // X
                    resultingPoints.ppoints[0].y = 0; // Y
                    resultingPoints.ppoints[0].z = sin((rotation + 0.125 - 0.25) * PI * 2) * diverge; // Z
                    // FRD
                    resultingPoints.ppoints[1].x = cos((rotation + 0.125) * PI * 2) * diverge; // X
                    resultingPoints.ppoints[1].y = 0; // Y
                    resultingPoints.ppoints[1].z = sin((rotation + 0.125) * PI * 2) * diverge; // Z
                    // BLD
                    resultingPoints.ppoints[2].x = cos((rotation + 0.125 + 0.5) * PI * 2) * diverge; // X
                    resultingPoints.ppoints[2].y = 0; // Y
                    resultingPoints.ppoints[2].z = sin((rotation + 0.125 + 0.5) * PI * 2) * diverge; // Z
                    // BRU
                    resultingPoints.ppoints[3].x = cos((rotation + 0.125 + 0.25) * PI * 2) * diverge; // X
                    resultingPoints.ppoints[3].y = 0; // Y
                    resultingPoints.ppoints[3].z = sin((rotation + 0.125 + 0.25) * PI * 2) * diverge; // Z
                    
                    break;
                case OUTPUT_8CH:
                    // AFormat input >> 8ch output
                    
                    resultingPoints.pointsCount = 4;
                    resultingPoints.pointsNames[0] = "FLU";
                    resultingPoints.pointsNames[1] = "FRD";
                    resultingPoints.pointsNames[2] = "BLD";
                    resultingPoints.pointsNames[3] = "BRU";
                    
                    // FLU
                    resultingPoints.ppoints[0].x = cos((rotation + 0.125 - 0.25) * PI * 2) * diverge; // X
                    resultingPoints.ppoints[0].y = (1.0 * diverge); // Y
                    resultingPoints.ppoints[0].z = sin((rotation + 0.125 - 0.25) * PI * 2) * diverge; // Z
                    // FRD
                    resultingPoints.ppoints[1].x = cos((rotation + 0.125) * PI * 2) * diverge; // X
                    resultingPoints.ppoints[1].y = (-1.0 * diverge); // Y
                    resultingPoints.ppoints[1].z = sin((rotation + 0.125) * PI * 2) * diverge; // Z
                    // BLD
                    resultingPoints.ppoints[2].x = cos((rotation + 0.125 + 0.5) * PI * 2) * diverge; // X
                    resultingPoints.ppoints[2].y = (-1.0 * diverge); // Y
                    resultingPoints.ppoints[2].z = sin((rotation + 0.125 + 0.5) * PI * 2) * diverge; // Z
                    // BRU
                    resultingPoints.ppoints[3].x = cos((rotation + 0.125 + 0.25) * PI * 2) * diverge; // X
                    resultingPoints.ppoints[3].y = (1.0 * diverge); // Y
                    resultingPoints.ppoints[3].z = sin((rotation + 0.125 + 0.25) * PI * 2) * diverge; // Z
                    break;
                default:
                    break;
            }
            break;
            
        case INPUT_BFORMAT:
            switch (outputMode) {
                case OUTPUT_4CH:
                    // BFormat input >> Quad output
                    
                    resultingPoints.pointsCount = 7;
                    resultingPoints.pointsNames[0] = "W"; //hide from UI unless there is 3D view
                    resultingPoints.pointsNames[1] = "X";
                    resultingPoints.pointsNames[2] = "Y";
                    resultingPoints.pointsNames[3] = "Z"; //hide from UI unless there is 3D view
                    resultingPoints.pointsNames[4] = "-X"; //hide from UI
                    resultingPoints.pointsNames[5] = "-Y"; //hide from UI
                    resultingPoints.pointsNames[6] = "-Z"; //hide from UI
                    
                    // W
                    resultingPoints.ppoints[0].x = 0; // X x
                    resultingPoints.ppoints[0].y = 0; // Y z
                    resultingPoints.ppoints[0].z = 0; // Z y
                    // X
                    resultingPoints.ppoints[1].x = cos((rotation + 0.250) * PI * 2) * diverge; // X
                    resultingPoints.ppoints[1].y = 0; // Y
                    resultingPoints.ppoints[1].z = sin((rotation + 0.250) * PI * 2) * diverge; // Z
                    // Y
                    resultingPoints.ppoints[2].x = cos((rotation + 0.0) * PI * 2) * diverge; // X
                    resultingPoints.ppoints[2].y = 0; // Y
                    resultingPoints.ppoints[2].z = sin((rotation + 0.0) * PI * 2) * diverge; // Z
                    // Z
                    resultingPoints.ppoints[3].x = 0; // X
                    resultingPoints.ppoints[3].y = 0; // Y
                    resultingPoints.ppoints[3].z = 0; // Z
                    // -X
                    resultingPoints.ppoints[4].x = cos((rotation + 0.75) * PI * 2) * diverge; // X
                    resultingPoints.ppoints[4].y = 0; // Y
                    resultingPoints.ppoints[4].z = sin((rotation + 0.75) * PI * 2) * diverge; // Z
                    // -Y
                    resultingPoints.ppoints[5].x = cos((rotation + 0.5) * PI * 2) * diverge; // X
                    resultingPoints.ppoints[5].y = 0; // Y
                    resultingPoints.ppoints[5].z = sin((rotation + 0.5) * PI * 2) * diverge; // Z
                    // -Z
                    resultingPoints.ppoints[6].x = 0; // X
                    resultingPoints.ppoints[6].y = 0; // Y
                    resultingPoints.ppoints[6].z = 0; // Z
                    
                    break;
                case OUTPUT_8CH:
                    // BFormat input >> 7.1 output
                    
                    resultingPoints.pointsCount = 7;
                    resultingPoints.pointsNames[0] = "W"; //hide from UI unless there is 3D view
                    resultingPoints.pointsNames[1] = "X";
                    resultingPoints.pointsNames[2] = "Y";
                    resultingPoints.pointsNames[3] = "Z"; //hide from UI unless there is 3D view
                    resultingPoints.pointsNames[4] = "-X"; //hide from UI
                    resultingPoints.pointsNames[5] = "-Y"; //hide from UI
                    resultingPoints.pointsNames[6] = "-Z"; //hide from UI
                    
                    // W
                    resultingPoints.ppoints[0].x = 0; // X x
                    resultingPoints.ppoints[0].y = 0; // Y z
                    resultingPoints.ppoints[0].z = 0; // Z y
                    // X
                    resultingPoints.ppoints[1].x = cos((rotation + 0.25) * PI * 2) * diverge; // X
                    resultingPoints.ppoints[1].y = 0; // Y
                    resultingPoints.ppoints[1].z = sin((rotation + 0.25) * PI * 2) * diverge; // Z
                    // Y
                    resultingPoints.ppoints[2].x = cos((rotation + 0.0) * PI * 2) * diverge; // X
                    resultingPoints.ppoints[2].y = 0; // Y
                    resultingPoints.ppoints[2].z = sin((rotation + 0.0) * PI * 2) * diverge; // Z
                    // Z
                    resultingPoints.ppoints[3].x = 0; // X
                    resultingPoints.ppoints[3].y = 1.0 * diverge; // Y
                    resultingPoints.ppoints[3].z = 0; // Z
                    // -X
                    resultingPoints.ppoints[4].x = cos((rotation + 0.75) * PI * 2) * diverge; // X
                    resultingPoints.ppoints[4].y = 0; // Y
                    resultingPoints.ppoints[4].z = sin((rotation + 0.75) * PI * 2) * diverge; // Z
                    // -Y
                    resultingPoints.ppoints[5].x = cos((rotation + 0.5) * PI * 2) * diverge; // X
                    resultingPoints.ppoints[5].y = 0; // Y
                    resultingPoints.ppoints[5].z = sin((rotation + 0.5) * PI * 2) * diverge; // Z
                    // -Z
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

		resultingPoints.ppoints[i].x = resultingPoints.ppoints[i].x / 2 + 0.5;
		resultingPoints.ppoints[i].y = resultingPoints.ppoints[i].y / 2 + 0.5;
		resultingPoints.ppoints[i].z = resultingPoints.ppoints[i].z / 2 + 0.5;

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