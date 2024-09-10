//
//  main.cpp
//  spatial-decode-example
//
//  Created by Dylan Marcus on 10/31/19.
//  Copyright © 2019 Mach1. All rights reserved.
//

#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
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
#include <thread>
#include <chrono>

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <Mach1Decode.h>

#define DELTA_ANGLE 0.0174533 // equivalent of 1 degrees in radians
#define DELTA_VALUE 1.0 // used for incrementing in degrees directly
#ifndef PI
#define PI 3.14159265358979323846
#endif

#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
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
static std::thread thread;
static bool done = false;
Mach1Decode m1Decode;
Mach1DecodeAlgoType outputFormat;
std::string outputName;
static std::vector<float> m1Coeffs;
static std::vector<float> m1PannedCoeffs;

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
Mach1Point3D orientation;

// variables for time logs
static auto start = 0.0;
static auto end = 0.0;
static float timeReturned = 0;

// variables for debug internal
static float checkSumL = 0;
static float checkSumR = 0;

float radToDeg (float input){
    float output = input * (180.0/PI);
    return output;
}

int main(int argc, const char * argv[]) {
    // time increment for Yaw/Pitch/Roll updates to decode
    struct timespec ts;
    ts.tv_sec =  0;
    ts.tv_nsec = (long)1e7; // 1/100 seconds

    printf("Setting up\n");
    outputFormat = Mach1DecodeAlgoSpatial_8;
    outputName = "MACH1 SPATIAL";
    done = false;
    thread = std::thread(decode, nullptr);

    while (!done) {
        nanosleep(&ts, NULL);
        auto start = std::chrono::high_resolution_clock::now();
        m1Decode.setPlatformType(Mach1PlatformDefault);
        m1Decode.setDecodeAlgoType(outputFormat);
        m1Decode.setFilterSpeed(1.0);

        orientation.x = yaw;
        orientation.y = pitch;
        orientation.z = roll;
        m1Decode.setRotationDegrees(orientation);
        m1Coeffs = m1Decode.decodeCoeffs();
        m1PannedCoeffs = m1Decode.decodePannedCoeffs();

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
                if(outputFormat==Mach1DecodeAlgoSpatial_8){
                    outputFormat=Mach1DecodeAlgoSpatial_4;
                    outputName="MACH1HORIZON-4";
                }else if(outputFormat==Mach1DecodeAlgoSpatial_4){
                    outputFormat=Mach1DecodeAlgoSpatial_14;
                    outputName="MACH1SPATIAL-14";
                }else if(outputFormat==Mach1DecodeAlgoSpatial_14){
                    outputFormat=Mach1DecodeAlgoSpatial_8;
                    outputName="MACH1SPATIAL-8";
                }else{
                    printf("Input out of scope.");
                }
                //resize coeffs array to the size of the current output
                m1Decode.setDecodeAlgoType(outputFormat);
                m1Coeffs.resize(m1Decode.getFormatCoeffCount(), 0.0f);
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
        for (int i = 0; i < m1Coeffs.size(); i++){
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
        for (int i = 0; i < (m1Coeffs.size())/2; i++){
            printf(" %iL: %f", i, m1Coeffs[i * 2]);
            printf(" %iR: %f\n", i, m1Coeffs[i * 2 + 1]);
        }
        printf("\n");
        printf("Number of Active Coeffs:\n");
        int activeCount = 0;
        for (int i = 0; i < (m1Coeffs.size()); i++){
            if (m1Coeffs[i] > 0.0) {
                activeCount++;
            }
        }
        printf("%i\n", activeCount);
        printf("\n");
        printf("Elapsed time: %f Seconds\n", timeReturned);
        printf("\n");
        printf("SUM CHECK L: %f    L REM: %f\n", checkSumL, abs(checkSumL-1.0f));
        printf("SUM CHECK R: %f    R REM: %f\n", checkSumR, abs(checkSumR-1.0f));
        printf("\n");
        printf("Decode Panned Coeffs:\n");
        for (int i = 0; i < (m1PannedCoeffs.size())/2; i++){
            // within each parent channel index of `m1PannedCoeffs` are two floats: [0]gain, [1]pan
            // - The `gain` float is normalized from 0.0 -> 1.0
            // - The `pan` float is normalized from -1.0 -> 1.0, from left -> right
            printf(" Channel[%i] Gain: %f | Pan: %f\n", i, m1PannedCoeffs[i * 2], m1PannedCoeffs[i * 2 + 1]);
        }
    }
    printf("\n");
    printf("Exiting\n");
    done = true;
    return NULL;
}
