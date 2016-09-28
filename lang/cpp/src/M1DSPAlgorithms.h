//
//  Mach1 SDK
//
//  Multichannel audio format family
//
//  Mixing algorithms v 0.8.2
//

#pragma once

#ifndef M1DSPAlgorithms_h
#define M1DSPAlgorithms_h


#include <vector>
#include <cmath>

#define PI       3.14159265358979323846

//
// Map utility
//

static float mmap(float value, float inputMin, float inputMax, float outputMin, float outputMax, bool clamp) {
    
    if (fabs(inputMin - inputMax) < __FLT_EPSILON__){
        return outputMin;
    } else {
        float outVal = ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin);
        
        if( clamp ){
            if(outputMax < outputMin){
                if( outVal < outputMax )outVal = outputMax;
                else if( outVal > outputMin )outVal = outputMin;
            }else{
                if( outVal > outputMax )outVal = outputMax;
                else if( outVal < outputMin )outVal = outputMin;
            }
        }
        return outVal;
    }
    
}

//--------------------------------------------------

//
//  Four channel audio format.
//
//  X = Yaw in angles
//  Y = Pitch in angles
//  Z = Roll in angles
//

static std::vector<float> fourChannelAlgorithm(float X, float Y, float Z) {
    float coefficients[4];
    coefficients[0] = 1. - std::min(1., std::min((float)360. - Y, Y) / 90.);
    coefficients[1] = 1. - std::min(1., std::abs((float)90. - Y) / 90.);
    coefficients[2] = 1. - std::min(1., std::abs((float)180. - Y) / 90.);
    coefficients[3] = 1. - std::min(1., std::abs((float)270. - Y) / 90.);
    
    
    std::vector<float> result;
    result.resize(8);
    result[0] = coefficients[0]; // 1 left
    result[1] = coefficients[3]; //   right
    result[2] = coefficients[1]; // 2 left
    result[3] = coefficients[0]; //   right
    result[4] = coefficients[3]; // 3 left
    result[5] = coefficients[2]; //   right
    result[6] = coefficients[2]; // 4 left
    result[7] = coefficients[1]; //   right
    return result;
}

// ------------------------------------------------------------------

//
//  Four pairs audio format.
//
//  X = Yaw in angles
//  Y = Pitch in angles
//  Z = Roll in angles
//

static std::vector<float> fourPairsAlgorithm(float X, float Y, float Z) {
    float volumes[4];
    volumes[0] = 1. - std::min(1., std::min((float)360. - Y, Y) / 90.);
    volumes[1] = 1. - std::min(1., std::abs((float)90. - Y) / 90.);
    volumes[2] = 1. - std::min(1., std::abs((float)180. - Y) / 90.);
    volumes[3] = 1. - std::min(1., std::abs((float)270. - Y) / 90.);
    
    std::vector<float> result;
    result.push_back(volumes[0]);
    result.push_back(volumes[1]);
    result.push_back(volumes[2]);
    result.push_back(volumes[3]);
    return result;
}

// ------------------------------------------------------------------

//
//  Eight channel audio format.
//
//  X = Yaw in angles
//  Y = Pitch in angles
//  Z = Roll in angles
//

static std::vector<float> eightChannelsAlgorithm(float X, float Y, float Z) {
    float coefficients[8];
    coefficients[0] = 1. - std::min(1., std::min((float)360. - Y, Y) / 90.);
    coefficients[1] = 1. - std::min(1., std::abs((float)90. - Y) / 90.);
    coefficients[2] = 1. - std::min(1., std::abs((float)180. - Y) / 90.);
    coefficients[3] = 1. - std::min(1., std::abs((float)270. - Y) / 90.);
    
    fourChannelAlgorithm(X, Y, Z);
    
    float tiltAngle = mmap(Z, -90, 90, 0., 1., true);
//    float tiltHigh = cos(tiltAngle * (0.5 * PI));
//    float tiltLow = cos((1.0 - tiltAngle) * (0.5 * PI));
    float tiltHigh = tiltAngle;
    float tiltLow = 1 - tiltHigh;
    
    //ISSUE//
    //Able to kill stereo by making both pitch and tilt at max or min values together
    
    std::vector<float> result;
    result.resize(16);
    result[0] = coefficients[0] * tiltHigh; // 1 left
    result[1] = coefficients[3] * tiltHigh; //   right
    result[2] = coefficients[1] * tiltLow; // 2 left
    result[3] = coefficients[0] * tiltLow; //   right
    result[4] = coefficients[3] * tiltLow; // 3 left
    result[5] = coefficients[2] * tiltLow; //   right
    result[6] = coefficients[2] * tiltHigh; // 4 left
    result[7] = coefficients[1] * tiltHigh; //   right
    
    result[0 + 8] = coefficients[0] * tiltLow; // 1 left
    result[1 + 8] = coefficients[3] * tiltLow ; //   right
    result[2 + 8] = coefficients[1] * tiltHigh ; // 2 left
    result[3 + 8] = coefficients[0] * tiltHigh ; //   right
    result[4 + 8] = coefficients[3] * tiltHigh ; // 3 left
    result[5 + 8] = coefficients[2] * tiltHigh ; //   right
    result[6 + 8] = coefficients[2] * tiltLow ; // 4 left
    result[7 + 8] = coefficients[1] * tiltLow ; //   right
    
    float pitchAngle = mmap(X, 90, -90, 0., 1., true);
    //float pitchHigherHalf = cos(pitchAngle * (0.5*PI));
    //float pitchLowerHalf = cos((1.0 - pitchAngle) * (0.5*PI));
    float pitchHigherHalf = pitchAngle;
    float pitchLowerHalf = 1 - pitchHigherHalf;
    
    for (int i = 0; i < 8; i++) {
        result[i] *= pitchLowerHalf;
        result[i + 8] *= pitchHigherHalf;
    }
    
    return result;
}

// ------------------------------------------------------------------

//
//  Eight pairs audio format.
//
//  X = Yaw in angles
//  Y = Pitch in angles
//  Z = Roll in angles
//

static std::vector<float> eightPairsAlgorithm(float X, float Y, float Z) {
    float volumes[4];
    volumes[0] = 1. - std::min(1., std::min((float)360. - Y, Y) / 90.);
    volumes[1] = 1. - std::min(1., std::abs((float)90. - Y) / 90.);
    volumes[2] = 1. - std::min(1., std::abs((float)180. - Y) / 90.);
    volumes[3] = 1. - std::min(1., std::abs((float)270. - Y) / 90.);
    
    std::vector<float> result;
    result.push_back(volumes[0]);
    result.push_back(volumes[1]);
    result.push_back(volumes[2]);
    result.push_back(volumes[3]);
    return result;
}

// ------------------------------------------------------------------

#endif /* M1DSPAlgorithms_h */
