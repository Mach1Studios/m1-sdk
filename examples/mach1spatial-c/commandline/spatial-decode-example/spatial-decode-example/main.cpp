//
//  main.cpp
//  spatial-decode-example
//
//  Created by Dylan Marcus on 10/31/19.
//  Copyright © 2019 Mach1. All rights reserved.
//

#define M1_STATIC

#if (defined(_WINDOWS) || defined(WIN32))
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
static float checkSum = 0;

float radToDeg (float input){
    float output = input * (180/PI);
    return output;
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
    done = false;
    pthread_create(&thread, NULL, &decode, NULL);
    
    while (!done) {
        nanosleep(&ts, NULL);
        auto start = std::chrono::high_resolution_clock::now();
        m1Coeffs = m1Decode.decode(radToDeg(yaw), radToDeg(pitch), radToDeg(roll), 0, 0);
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
        
        checkSumL = (m1Coeffs[0] + m1Coeffs[2] + m1Coeffs[4] + m1Coeffs[6] + m1Coeffs[8] + m1Coeffs[10] + m1Coeffs[12] + m1Coeffs[14]);
        checkSumR = (m1Coeffs[1] + m1Coeffs[3] + m1Coeffs[5] + m1Coeffs[7] + m1Coeffs[9] + m1Coeffs[11] + m1Coeffs[13] + m1Coeffs[15]);
        checkSum = (checkSumL+checkSumR)/2;
        
        // Mach1DecodeCAPI Log:
        printf("\n");
        printf("y / p / r: %f %f %f\n", radToDeg(yaw), radToDeg(pitch), radToDeg(roll));
        printf("\n");
        printf("Decode Coeffs:\n");
        printf(" 1L: %f 1R: %f\n 2L: %f 2R: %f\n 3L: %f 3R: %f\n 4L: %f 4R: %f\n\n 5L: %f 5R: %f\n 6L: %f 6R: %f\n 7L: %f 7R: %f\n 8L: %f 8R: %f\n", m1Coeffs[0], m1Coeffs[1], m1Coeffs[2], m1Coeffs[3], m1Coeffs[4], m1Coeffs[5], m1Coeffs[6], m1Coeffs[7], m1Coeffs[8], m1Coeffs[9], m1Coeffs[10], m1Coeffs[11], m1Coeffs[12], m1Coeffs[13], m1Coeffs[14], m1Coeffs[15]);
        printf("\n");
        printf("Headlock Stereo Coeffs:\n");
        printf("%f %f\n", m1Coeffs[16], m1Coeffs[17]);
        printf("\n");
        printf("Elapsed time: %f Seconds\n", timeReturned);
        printf("\n");
        printf("SUM CHECK L: %f\n", checkSumL);
        printf("SUM CHECK R: %f\n", checkSumR);
        printf("SUM CHECK  : %f\n", checkSum);
        printf("\n");
    }
    printf("\n");
    printf("Exiting\n");
    done = true;
    return NULL;
}
