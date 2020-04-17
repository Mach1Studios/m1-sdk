//
//  main.cpp
//  spatial-decode-example
//
//  Created by Dylan Marcus on 10/31/19.
//  Copyright © 2019 Mach1. All rights reserved.
//

#define M1_STATIC

#if defined(_WIN32)
#include <time.h>
#include <windows.h>
#include <conio.h>
#define _TIMESPEC_DEFINED
#else
#include <sys/time.h>
#include <unistd.h>
#include <termios.h>
#endif

#include <iostream>
#include <time.h>
#include <pthread.h>
#include <chrono>

#include <string.h>
#include <stdio.h>
#include <math.h>
#include "Mach1Decode.h"

#define DELTA_ANGLE 0.0174533 // equivalent of 1 degrees in radians
#define DELTA_VALUE 1.0 // used for incrementing in degrees directly
#ifndef PI
#define PI 3.14159265358979323846
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

#ifdef WIN32
BOOLEAN nanosleep(struct timespec* ts, void* p) {
	/* Declarations */
	HANDLE timer;	/* Timer handle */
	LARGE_INTEGER li;	/* Time defintion */
	/* Create timer */
	if (!(timer = CreateWaitableTimer(NULL, TRUE, NULL)))
		return FALSE;
	/* Set timer properties */
	li.QuadPart = -ts->tv_nsec;
	if (!SetWaitableTimer(timer, &li, 0, NULL, NULL, FALSE)) {
		CloseHandle(timer);
		return FALSE;
	}
	/* Start & wait for timer */
	WaitForSingleObject(timer, INFINITE);
	/* Clean resources */
	CloseHandle(timer);
	/* Slept without problems */
	return TRUE;
}
#endif


static void* decode(void* v);
static pthread_t thread;
static bool done = false;
Mach1Decode m1Decode;
Mach1DecodeAlgoType outputFormat;
std::string outputName;
static std::vector<float> m1Coeffs;

/*
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

// variables for time logs
static auto start = 0.0;
static auto end = 0.0;
static float timeReturned = 0;

// variables for debug internal
static float checkSumL = 0;
static float checkSumR = 0;

float radToDeg (float input){
    float output = input * (180.0/M_PI);
    return output;
}

int main(int argc, const char * argv[]) {
    // time increment for Yaw/Pitch/Roll updates to decode
    struct timespec ts;
    ts.tv_sec =  0;
    ts.tv_nsec = (long)1e7; // 1/100 seconds
    
    printf("Setting up\n");
    outputFormat = Mach1DecodeAlgoSpatial;
    outputName = "MACH1 SPATIAL";
    done = false;
    pthread_create(&thread, NULL, &decode, NULL);
    
    while (!done) {
        nanosleep(&ts, NULL);
        auto start = std::chrono::high_resolution_clock::now();
        m1Decode.setPlatformType(Mach1PlatformDefault);
        m1Decode.setDecodeAlgoType(outputFormat);
        m1Decode.setFilterSpeed(1.0);

        m1Decode.beginBuffer();
        m1Coeffs = m1Decode.decode(yaw, pitch, roll, 0, 0);
        m1Decode.endBuffer();
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        timeReturned = (float)elapsed.count();
    }
    
    return 0;
}

static void* decode(void* v)
{
    /* Allow Terminal to input chars without "Enter" */
#ifndef _WIN32
	struct termios info;
    tcgetattr(0, &info);
    info.c_lflag &= ~ICANON;
    info.c_cc[VMIN] = 1;
    info.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &info);
#endif

    printf("In the run thread\n");
    char c;
    printf("Enter a command:\n");
    while (1) {
        
#ifdef _WIN32
		c = _getch();
#else 
		c = getchar();
#endif     

		if (c == 'q') break;
        
        // delete entered character
        printf("\b");
        switch (c) {
            case 'd':
                yaw += DELTA_VALUE;
                break;
            case 'a':
                yaw -= DELTA_VALUE;
                break;
            case 'w':
                pitch += DELTA_VALUE;
                break;
            case 's':
                pitch -= DELTA_VALUE;
                break;
            case 'x':
                roll += DELTA_VALUE;
                break;
            case 'z':
                roll -= DELTA_VALUE;
                break;
            case 'o':
                if(outputFormat==Mach1DecodeAlgoSpatial){
                    outputFormat=Mach1DecodeAlgoHorizon;
                    outputName="MACH1 HORIZON";
                }else if(outputFormat==Mach1DecodeAlgoHorizon){
                    outputFormat=Mach1DecodeAlgoAltSpatial;
                    outputName="MACH1 SPATIAL Periphonic";
                }else if(outputFormat==Mach1DecodeAlgoAltSpatial){
                    outputFormat=Mach1DecodeAlgoSpatialPlus;
                    outputName="MACH1 SPATIAL+";
                }else if(outputFormat==Mach1DecodeAlgoSpatialPlus){
                    outputFormat=Mach1DecodeAlgoSpatialPlusPlus;
                    outputName="MACH1 SPATIAL++";
                }else if(outputFormat==Mach1DecodeAlgoSpatialPlusPlus){
                    outputFormat=Mach1DecodeAlgoSpatialExt;
                    outputName="MACH1 SPATIAL Extended";
                }else if(outputFormat==Mach1DecodeAlgoSpatialExt){
                    outputFormat=Mach1DecodeAlgoSpatialExtPlus;
                    outputName="MACH1 SPATIAL Extended+";
                }else if(outputFormat==Mach1DecodeAlgoSpatialExtPlus){
                    outputFormat=Mach1DecodeAlgoSpatial;
                    outputName="MACH1 SPATIAL";
                }else{
                    printf("Input out of scope.");
                }
                //resize coeffs array to the size of the current output
                m1Decode.setDecodeAlgoType(outputFormat);
                m1Coeffs.resize(m1Decode.getFormatChannelCount(), 0.0f);
                break;
            default:
                printf("Input not recognized.\n");
        }
        
        // check that the values are in proper range
        if (yaw < 0.0) yaw = 360.0;
        else if (yaw > 360.0) yaw = 0.0;
        if (pitch < -90.0) pitch = -90.0;
        else if (pitch > 90.0) pitch = 90.0;
        if (roll < -90.0) roll = -90.0;
        else if (roll > 90.0) roll = 90.0;
        
        checkSumL = 0; // zeroed for next loop
        checkSumR = 0; // zeroed for next loop
        for (int i = 0; i < m1Coeffs.size()-2; i++){ //minus 2 for removing the static stereo indices
            if(i % 2 == 0){
                checkSumL=checkSumL+m1Coeffs[i];
            } else {
                checkSumR=checkSumR+m1Coeffs[i];
            }
        }
        
        // Mach1DecodeCAPI Log:
        printf("\n");
        printf("y / p / r: %f %f %f\n", yaw, pitch, roll);
        printf("\n");
        printf("Output M1 Format: %s\n", outputName.c_str());
        printf("\n");
        printf("Decode Coeffs:\n");
        for (int i = 0; i < (m1Coeffs.size()-2)/2; i++){
            printf(" %iL: %f", i, m1Coeffs[i * 2]);
            printf(" %iR: %f\n", i, m1Coeffs[i * 2 + 1]);
        }
        printf("Headlock Stereo Coeffs:\n");
        printf("%f %f\n", m1Coeffs[m1Decode.getFormatChannelCount()-2], m1Coeffs[m1Decode.getFormatChannelCount()-1]);
        printf("\n");
        printf("Elapsed time: %f Seconds\n", timeReturned);
        printf("\n");
        printf("SUM CHECK L: %f    L REM: %f\n", checkSumL, abs(checkSumL-1.0f));
        printf("SUM CHECK R: %f    R REM: %f\n", checkSumR, abs(checkSumR-1.0f));
        printf("\n");
    }
    printf("\n");
    printf("Exiting\n");
    done = true;
    return NULL;
}
