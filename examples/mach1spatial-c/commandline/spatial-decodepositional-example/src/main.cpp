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

#include <Mach1DecodePositional.h>

#define DELTA_ANGLE 0.0174533 // equivalent of 1 degrees in radians
#define DELTA_VALUE 1.0 // used for incrementing in degrees directly
#define DELTA_POS_STEP 0.25
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
Mach1DecodePositional m1Decode;
std::vector<float> m1Coeffs;

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

static float rYaw = 0;
static float rPitch = 0;
static float rRoll = 0;
static float dYaw = 0;
static float dPitch = 0;
static float dRoll = 0;
static float x = 0;
static float y = 0;
static float z = -2;
static bool attenuationActive = true;
static float distance = 0;
static float attenuation = 0;

// variables for time logs
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
	m1Decode.setPlatformType(Mach1PlatformType::Mach1PlatformDefault);
	m1Decode.setDecodeMode(Mach1DecodeMode::M1DecodeSpatial_8);
	m1Decode.setFilterSpeed(1.0);

    m1Decode.setMuteWhenInsideObject(false);
    m1Decode.setMuteWhenOutsideObject(false);

    done = false;
    thread = std::thread(decode, nullptr);

    while (!done) {
		nanosleep(&ts, NULL);
		auto start = std::chrono::high_resolution_clock::now();
        m1Decode.setDecoderAlgoPosition(Mach1Point3D {0.0, 0.0, 0.0});
        m1Decode.setDecoderAlgoRotation(Mach1Point3D {0.0, 0.0, 0.0});
        m1Decode.setDecoderAlgoScale(Mach1Point3D {1.0, 1.0, 1.0});
		dYaw = rYaw;
		dPitch = rPitch;
        dRoll = rRoll;
        m1Decode.setListenerRotation(Mach1Point3D {dYaw, dPitch, dRoll});
        m1Decode.setListenerPosition(Mach1Point3D {x, y, z});
        m1Decode.setUseYawForRotation(true);
        m1Decode.setUsePitchForRotation(true);
        m1Decode.setUseRollForRotation(true);
        m1Decode.setUsePlaneCalculation(false);
        m1Decode.evaluatePositionResults();

        //Distance Application:
        distance = m1Decode.getDist();
        /*
         Mapping distance to arbitrary linear curve
         Design your own distance coefficient curve here
         This example: Linear curve of 100% -> 0% from 0 to 10 distance away
        */
        m1Decode.setUseAttenuation(attenuationActive);
        attenuation = mapFloat(distance, 0, 10, 1, 0);
        m1Decode.setAttenuationCurve(attenuation);

        m1Coeffs.resize(m1Decode.getFormatCoeffCount());
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
                rYaw += DELTA_VALUE;
                break;
            case 'a':
                rYaw -= DELTA_VALUE;
                break;
            case 'w':
                rPitch += DELTA_VALUE;
                break;
            case 's':
                rPitch -= DELTA_VALUE;
                break;
            case 'x':
                rRoll += DELTA_VALUE;
                break;
            case 'z':
                rRoll -= DELTA_VALUE;
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
            case 'n':
                attenuationActive = !attenuationActive;
                break;
            default:
                printf("Input not recognized.\n");
        }

        // check that the values are in proper range
        if (dYaw < 0.0) dYaw = 360.0;
        else if (dYaw > 360.0) dYaw = 0.0;
        if (dPitch < -90.0) dPitch = -90.0;
        else if (dPitch > 90.0) dPitch = 90.0;
        if (dRoll < -90.0) dRoll = -90.0;
        else if (dRoll > 90.0) dRoll = 90.0;

        // Mach1DecodeCAPI Log:
        printf("\n");
        printf("y / p / r: %f %f %f\n", dYaw, dPitch, dRoll);
        printf("\n");
        printf("x / y / z: %f %f %f\n", x, y, z);
        printf("\n");
        printf("Decode Coeffs:\n");
        for (int i = 0; i < (m1Coeffs.size())/2; i++){
            printf(" %iL: %f", i, m1Coeffs[i * 2]);
            printf(" %iR: %f\n", i, m1Coeffs[i * 2 + 1]);
        }
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
