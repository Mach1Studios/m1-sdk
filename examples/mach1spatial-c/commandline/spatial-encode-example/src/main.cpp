//
//  main.cpp
//  spatial-encode-example
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
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include <chrono>

#include "Mach1Encode.h"

#define DELTA_RADIAN 0.0174533 // equivalent of 1 degrees in radians
#define DELTA_DEGREE 1.0
#define DELTA_DIVERGE 0.01

#ifndef PI
#define PI 3.14159265358979323846
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

#ifdef _WIN32
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

void* encode(void* v);
static pthread_t thread;
bool done = false;
Mach1Encode m1Encode;
std::vector< std::vector<float> > m1Coeffs; //2D array, [input channel][input channel's coeff]
Mach1EncodeInputModeType inputMode;
Mach1EncodeOutputModeType outputMode;
std::string inputName;
std::string outputName;

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
float azimuth = 0.0;
float diverge = 0.0;
float elevation = 0.0;
bool isIsotroptic = true;
bool isAutoOrbit = true;
float stereoOrbitRotation = 0.0;
float stereoSpread = 0.0;

// variables for time logs
float timeReturned = 0;

float radToDeg (float input){
    float output = input * (180/PI);
    return output;
}

int main(int argc, const char * argv[]) {
    // time increment for input value updates to encode
    struct timespec ts;
    ts.tv_sec =  0;
    ts.tv_nsec = (long)1e7;
    
    printf("Setting up\n");
    inputMode = Mach1EncodeInputModeMono;
    outputMode = Mach1EncodeOutputModeM1Spatial_8;
    inputName = "MONO";
    outputName = "MACH1 SPATIAL";
    done = false;
    pthread_create(&thread, NULL, &encode, NULL);
    
    while (!done) {
        nanosleep(&ts, NULL);
        auto start = std::chrono::high_resolution_clock::now();
        m1Encode.setAzimuthDegrees(azimuth);
        m1Encode.setDiverge(diverge);
        m1Encode.setElevationDegrees(elevation);
        m1Encode.setInputMode(inputMode);
        m1Encode.setOutputMode(outputMode);
        m1Encode.setPannerMode((isIsotroptic) ? Mach1EncodePannerModeIsotropicLinear : Mach1EncodePannerModePeriphonicLinear);
        m1Encode.setAutoOrbit(isAutoOrbit);
        if (!isAutoOrbit){
            m1Encode.setOrbitRotationDegrees(stereoOrbitRotation);
        }
        m1Encode.setStereoSpread(stereoSpread);
        m1Encode.generatePointResults();
        m1Coeffs = m1Encode.getGains();
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        timeReturned = (float)elapsed.count();
    }
    
    return 0;
}

void* encode(void* v)
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
                azimuth += DELTA_DEGREE;
                break;
            case 'a':
                azimuth -= DELTA_DEGREE;
                break;
            case 'w':
                diverge += DELTA_DIVERGE;
                break;
            case 's':
                diverge -= DELTA_DIVERGE;
                break;
            case 'x':
                elevation += DELTA_DEGREE;
                break;
            case 'z':
                elevation -= DELTA_DEGREE;
                break;
            case 'i':
                if(inputMode==Mach1EncodeInputModeMono){
                    inputMode=Mach1EncodeInputModeStereo;
                    inputName="STEREO";
                }else if(inputMode==Mach1EncodeInputModeStereo){
                    inputMode=Mach1EncodeInputModeLCR;
                    inputName="LCR";
                }else if(inputMode==Mach1EncodeInputModeLCR){
                    inputMode=Mach1EncodeInputModeQuad;
                    inputName="QUAD";
                }else if(inputMode==Mach1EncodeInputModeQuad){
                    inputMode=Mach1EncodeInputModeLCRS;
                    inputName="LCRS";
                }else if(inputMode==Mach1EncodeInputModeLCRS){
                    inputMode=Mach1EncodeInputModeAFormat;
                    inputName="AFORMAT";
                }else if(inputMode==Mach1EncodeInputModeAFormat){
                    inputMode=Mach1EncodeInputModeBFOAACN;
                    inputName="1OA ACN";
                }else if(inputMode==Mach1EncodeInputModeBFOAACN){
                    inputMode=Mach1EncodeInputModeBFOAACN;
                    inputName="1OA ACN";
                }else if(inputMode==Mach1EncodeInputModeBFOAACN){
                    inputMode=Mach1EncodeInputModeBFOAFUMA;
                    inputName="1OA FUMA";
                }else if(inputMode==Mach1EncodeInputModeBFOAFUMA){
                    inputMode=Mach1EncodeInputModeMono;
                    inputName="MONO";
                }else{
                    printf("Input out of scope.");
                }
                break;
            case 'o':
                if(outputMode==Mach1EncodeOutputModeM1Spatial_8){
                    outputMode=Mach1EncodeOutputModeM1Horizon_4;
                    outputName="MACH1HORIZON-4";
                }else if(outputMode==Mach1EncodeOutputModeM1Horizon_4){
                    outputMode=Mach1EncodeOutputModeM1Spatial_14;
                    outputName="MACH1SPATIAL-14";
                }else if(outputMode==Mach1EncodeOutputModeM1Spatial_14){
                    outputMode=Mach1EncodeOutputModeM1Spatial_32;
                    outputName="MACH1SPATIAL-32";
                }else if(outputMode==Mach1EncodeOutputModeM1Spatial_32){
                    outputMode=Mach1EncodeOutputModeM1Spatial_36;
                    outputName="MACH1SPATIAL-36";
                }else if(outputMode==Mach1EncodeOutputModeM1Spatial_36){
                    outputMode=Mach1EncodeOutputModeM1Spatial_48;
                    outputName="MACH1SPATIAL-48";
                }else if(outputMode==Mach1EncodeOutputModeM1Spatial_48){
                    outputMode=Mach1EncodeOutputModeM1Spatial_60;
                    outputName="MACH1SPATIAL-60";
                }else if(outputMode==Mach1EncodeOutputModeM1Spatial_60){
                    outputMode=Mach1EncodeOutputModeM1Spatial_8;
                    outputName="MACH1SPATIAL-8";
                }else{
                    printf("Input out of scope.");
                }
                //resize coeffs array to the size of the current output
                m1Encode.setOutputMode(outputMode);
                for (int i = 0; i < m1Coeffs.size(); i++){
                    m1Coeffs[i].resize(m1Encode.getOutputChannelsCount(), 0.0f);
                }

                break;
            case 'e': //isotropic encode enable
                isIsotroptic = !isIsotroptic;
                break;
            case 't':
                isAutoOrbit = !isAutoOrbit;
                break;
            case 'c':
                stereoOrbitRotation -= DELTA_DEGREE;
                break;
            case 'v':
                stereoOrbitRotation += DELTA_DEGREE;
                break;
            case 'b':
                stereoSpread -= DELTA_DIVERGE;
                break;
            case 'n':
                stereoSpread += DELTA_DIVERGE;
                break;
            default:
                printf("Input not recognized.\n");
        }
        
        // check that the values are in proper range
        if (diverge < -1.0) diverge = -1.0;
        else if (diverge > 1.0) diverge = 1.0;
        if (stereoSpread < -1.0) stereoSpread = -1.0;
        else if (stereoSpread > 1.0) stereoSpread = 1.0;
        if (azimuth < 0.0) azimuth = 360.0;
        else if (azimuth > 360.0) azimuth = 0.0;
        if (stereoOrbitRotation < 0.0) stereoOrbitRotation = 360.0;
        else if (stereoOrbitRotation > 360.0) stereoOrbitRotation = 0;
        if (elevation < -180.0) elevation = -180.0;
        else if (elevation > 180.0) elevation = 180.0;
        
        // Mach1EncodeCAPI Log:
        printf("\n");
        printf("Input: %s\n", inputName.c_str());
        printf("Output: %s\n", outputName.c_str());
        printf("\n");
        printf("Azimuth: %f\n", azimuth);
        printf("Diverge: %f\n", diverge);
        printf("Elevation: %f\n", elevation);
        printf("\n");
        if(isIsotroptic){
            printf("Isotropic Active\n");
        } else if (!isIsotroptic){
            printf("Periphonic Active\n");
        }
        if(inputMode==1){
            printf("Stereo Spread: %f\n", stereoSpread);
            if(!isAutoOrbit){
                printf("Stereo Rotation: %f\n", stereoOrbitRotation);
                printf("\n");
            }
            if(isAutoOrbit){
                printf("Stereo Auto Orbit Active\n");
            } else if (!isAutoOrbit){
                printf("Stereo Manual Orbit\n");
            }
        }
        printf("\n");
        printf("Encode Coeffs:\n");
        for (int i = 0; i < m1Coeffs.size(); i++){
            float coeffSum = 0;
            printf("Number of Channels %i\n", i);
            printf("Channel %i Coeffs: \n", i);
			for (int j = 0; j < m1Coeffs[i].size(); j++) {
				printf("%f ", m1Coeffs[i][j]);
                coeffSum += m1Coeffs[i][j];
			}
			printf("\n\n");
            printf("Channel %i Sum: %f \n", i, coeffSum);
		}
        printf("\n");
        printf("Elapsed time: %f Seconds\n", timeReturned);
        printf("\n");
    }
    printf("\n");
    printf("Exiting\n");
    done = true;
    return NULL;
}
