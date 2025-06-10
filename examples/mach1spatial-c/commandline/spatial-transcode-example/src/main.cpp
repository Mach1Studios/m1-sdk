//
//  main.cpp
//  spatial-transcode-example
//
//  Created by Dylan Marcus on 6/16/20.
//  Copyright © 2020 Mach1. All rights reserved.
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
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <thread>
#include <chrono>

#include <Mach1Transcode.h>

#define DELTA_RADIAN 0.0174533 // equivalent of 1 degrees in radians
#define DELTA_DEGREE 1.0
#define DELTA_DIVERGE 0.01

#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
BOOLEAN nanosleep(struct timespec* ts, void* p) {
    /* Declarations */
    HANDLE timer;    /* Timer handle */
    LARGE_INTEGER li;    /* Time defintion */
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
Mach1Transcode<float> m1Transcode;
static std::vector< std::vector<float> > m1Coeffs; //2D array, [input channel][input channel's coeff]
int inputMode;
int outputMode;
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

// variables for time logs
static float timeReturned = 0;

float radToDeg (float input){
    float output = input * (180/PI);
    return output;
}

int main(int argc, const char * argv[]) {
    // time increment for Yaw/Pitch/Roll updates to decode
    struct timespec ts;
    ts.tv_sec =  0;
    ts.tv_nsec = (long)1e7;

    printf("Setting up\n");
    inputMode = m1Transcode.getFormatFromString("ACNSN3DmaxRE3oa");
    outputMode = m1Transcode.getFormatFromString("M1Spatial-8");
    inputName = "ACNSN3D-3OA";
    outputName = "MACH1SPATIAL-8";
    done = false;
    thread = std::thread(decode, nullptr);

    while (!done) {
        nanosleep(&ts, NULL);
        auto start = std::chrono::high_resolution_clock::now();
        m1Transcode.setInputFormat(inputMode);
        m1Transcode.setOutputFormat(outputMode);
        m1Transcode.setSpatialDownmixer();

        m1Transcode.processConversionPath();
        m1Coeffs = m1Transcode.getMatrixConversion();
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
            case 'i':
                if(inputMode==m1Transcode.getFormatFromString("ACNSN3DmaxRE3oa")){
                    inputMode=m1Transcode.getFormatFromString("FiveOneFilm");
                    inputName="FiveOneFilm";
                }else if(inputMode==m1Transcode.getFormatFromString("FiveOneFilm")){
                    inputMode=m1Transcode.getFormatFromString("SevenOneFour");
                    inputName="SevenOneFour";
                }else if(inputMode==m1Transcode.getFormatFromString("SevenOneFour")){
                    inputMode=m1Transcode.getFormatFromString("ACNSN3DmaxRE3oa");
                    inputName="ACNSN3D-3OA";
                }else{
                    printf("Input out of scope.");
                }
                break;
            case 'o':
                if(outputMode==m1Transcode.getFormatFromString("M1Spatial")){
                    outputMode=m1Transcode.getFormatFromString("FiveOneFour");
                    outputName="FiveOneFour";
                }else if(outputMode==m1Transcode.getFormatFromString("FiveOneFour")){
                    outputMode=m1Transcode.getFormatFromString("TBE");
                    outputName="TBE-2OAHybrid";
                }else if(outputMode==m1Transcode.getFormatFromString("TBE")){
                    outputMode=m1Transcode.getFormatFromString("M1Spatial");
                    outputName="MACH1 SPATIAL";
                }else{
                    printf("Input out of scope.");
                }
                //resize coeffs array to the size of the current output
                m1Transcode.setOutputFormat(outputMode);
                for (int i = 0; i < m1Coeffs.size(); i++){
                    m1Coeffs[i].resize(m1Transcode.getOutputNumChannels(), 0.0f);
                }
            default:
                printf("Input not recognized.\n");
        }

        // Mach1TranscodeAPI Log:
        printf("\n");
        printf("Input: %s\n", inputName.c_str());
        printf("Output: %s\n", outputName.c_str());
        printf("\n");
        printf("Transcode Coeffs:\n");
        for (int i = 0; i < m1Coeffs.size(); i++){
            printf("Number of Channels %i\n", i);
            printf("Channel %i Coeffs: \n", i);
            for (int j = 0; j < m1Coeffs[i].size(); j++) {
                printf("%f ", m1Coeffs[i][j]);
            }
            printf("\n\n");
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
