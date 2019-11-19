//
//  main.cpp
//  spatial-decode-example
//
//  Created by Dylan Marcus on 10/31/19.
//  Copyright © 2019 Mach1. All rights reserved.
//

#include <iostream>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <unistd.h>
#include <termios.h>
#include <chrono>
#include "Mach1DecodePositional.h"

#define DELTA_ANGLE 0.0174533 // equivalent of 1 degrees in radians
#define DELTA_POS_STEP 0.25
#ifndef PI
#define PI 3.14159265358979323846
#endif

static void* decode(void* v);
static pthread_t thread;
static bool done = false;
Mach1DecodePositional m1Decode;
static std::vector<float> m1Coeffs;

/*
 Positional 3D Coords
 
 X+ = strafe right
 X- = strafe left
 Y+ = up
 Y- = down
 Z+ = forward
 Z- = backward
 
 Orientation Euler
 
 Yaw[0]+ = rotate right [Range: 0->360 | -180->180]
 Yaw[0]- = rotate left [Range: 0->360 | -180->180]
 Pitch[1]+ = rotate up [Range: -90->90]
 Pitch[1]- = rotate down [Range: -90->90]
 Roll[2]+ = tilt right [Range: -90->90]
 Roll[2]- = tilt left [Range: -90->90]

 http://dev.mach1.tech/#mach1-internal-angle-standard
*/

static float yaw = 0;
static float pitch = 0;
static float roll = 0;
static float x = 0;
static float y = 0;
static float z = 0;
static float distance = 0;
static float attenuation = 0;

// variables for time logs
static auto start = 0.0;
static auto end = 0.0;
static float timeReturned = 0;

float radToDeg (float input){
    float output = input * (180/PI);
    return output;
}

float mapFloat(float input, float inMin, float inMax, float outMin, float outMax){
    return (input - inMin) / (inMax - inMin) * (outMax - outMin) + outMin;
}

int main(int argc, const char * argv[]) {
    // time increment for Yaw/Pitch/Roll updates to decode
    struct timespec ts;
    ts.tv_sec =  0;
    ts.tv_nsec = (long)1e7; // 1/100 seconds
    
    printf("Setting up\n");
    m1Decode.setPlatformType(Mach1PlatformDefault);
    m1Decode.setDecodeAlgoType(Mach1DecodeAlgoSpatial);
    m1Decode.setFilterSpeed(1.0);
    m1Decode.setUseAttenuation(true);
    m1Decode.setUsePlaneCalculation(false);
    m1Decode.setUseYawForRotation(true);
    m1Decode.setUsePitchForRotation(true);
    m1Decode.setUseRollForRotation(true);
    
    done = false;
    pthread_create(&thread, NULL, &decode, NULL);
    
    while (!done) {
        nanosleep(&ts, NULL);
        auto start = std::chrono::high_resolution_clock::now();
        m1Decode.setDecoderAlgoRotation(Mach1Point3D {0.0, 0.0, 0.0});
        m1Decode.setDecoderAlgoPosition(Mach1Point3D {0.0, 0.0, 10.0});
        m1Decode.setDecoderAlgoScale(Mach1Point3D {0.1, 0.1, 0.1});
        m1Decode.setListenerPosition(Mach1Point3D {radToDeg(yaw), radToDeg(pitch), radToDeg(roll)});
        m1Decode.setListenerRotation(Mach1Point3D {x, y, z});
        m1Decode.evaluatePositionResults();
        //Distance Application:
        distance = m1Decode.getDist();
        /*
         Mapping distance to arbitrary linear curve
         Design your own distance coefficient curve here
        */
        attenuation = mapFloat(distance, 0, 10, 1, 0);
        m1Decode.setAttenuationCurve(attenuation);
        m1Decode.getCoefficients(m1Coeffs);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        timeReturned = (float)elapsed.count();
    }
    
    return 0;
}

static void* decode(void* v)
{
    /* Allow Terminal to input chars without "Enter" */
    struct termios info;
    tcgetattr(0, &info);
    info.c_lflag &= ~ICANON;
    info.c_cc[VMIN] = 1;
    info.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &info);
    
    printf("In the run thread\n");
    char c;
    printf("Enter a command:\n");
    while (1) {
        
        c = getchar();
        
        if (c == 'q') break;
        
        // delete entered character
        printf("\b");
        switch (c) {
            case 'd':
                yaw += DELTA_ANGLE;
                break;
            case 'a':
                yaw -= DELTA_ANGLE;
                break;
            case 'w':
                pitch += DELTA_ANGLE;
                break;
            case 's':
                pitch -= DELTA_ANGLE;
                break;
            case 'x':
                roll += DELTA_ANGLE;
                break;
            case 'z':
                roll -= DELTA_ANGLE;
                break;
            case 'i':
                z += DELTA_POS_STEP;
                break;
            case 'k':
                z -= DELTA_POS_STEP;
                break;
            case 'l':
                x += DELTA_POS_STEP;
                break;
            case 'j':
                x -= DELTA_POS_STEP;
                break;
            case 'y':
                y += DELTA_POS_STEP;
                break;
            case 'h':
                y -= DELTA_POS_STEP;
                break;
            default:
                printf("Input not recognized.\n");
        }
        
        // check that the values are in proper range
        if (pitch < -M_PI) pitch = M_PI;
        else if (pitch > M_PI) pitch = -M_PI;
        if (yaw < 0) yaw = 2*M_PI;
        else if (yaw > 2*M_PI) yaw = 0;
        if (roll < -M_PI) roll = M_PI;
        else if (roll > M_PI) roll = -M_PI;
        
        // Mach1DecodeCAPI Log:
        printf("\n");
        printf("y / p / r: %f %f %f\n", radToDeg(yaw), radToDeg(pitch), radToDeg(roll));
        printf("\n");
        printf("x / y / z: %f %f %f\n", x, y, z);
        printf("\n");
        printf("Decode Coeffs:\n");
        printf(" 1L: %f 1R: %f\n 2L: %f 2R: %f\n 3L: %f 3R: %f\n 4L: %f 4R: %f\n\n 5L: %f 5R: %f\n 6L: %f 6R: %f\n 7L: %f 7R: %f\n 8L: %f 8R: %f\n", m1Coeffs[0], m1Coeffs[1], m1Coeffs[2], m1Coeffs[3], m1Coeffs[4], m1Coeffs[5], m1Coeffs[6], m1Coeffs[7], m1Coeffs[8], m1Coeffs[9], m1Coeffs[10], m1Coeffs[11], m1Coeffs[12], m1Coeffs[13], m1Coeffs[14], m1Coeffs[15]);
        printf("\n");
        printf("Headlock Stereo Coeffs:\n");
        printf("%f %f\n", m1Coeffs[16], m1Coeffs[17]);
        printf("\n");
        printf("Distance:\n");
        printf("%f\n", distance);
        printf("Attenuation Curve:\n");
        printf("%f\n", attenuation);
        printf("\n");
        printf("Elapsed time: %f Seconds\n", timeReturned);
        printf("\n");
    }
    printf("\n");
    printf("Exiting\n");
    done = true;
    return NULL;
}
