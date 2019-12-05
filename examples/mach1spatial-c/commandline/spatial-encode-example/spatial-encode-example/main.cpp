//
//  main.cpp
//  spatial-encode-example
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
#include "Mach1Encode.h"

#define DELTA_ANGLE 0.0174533 // equivalent of 1 degrees in radians
#define DELTA_DIVERGE 0.01
#ifndef PI
#define PI 3.14159265358979323846
#endif

static void* decode(void* v);
static pthread_t thread;
static bool done = false;
Mach1Encode m1Encode;
static std::vector<std::vector<float>> m1Coeffs; //2D array, [input channel][input channel's coeff]
Mach1EncodeInputModeType inputMode;
Mach1EncodeOutputModeType outputMode;

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
static float rotation = 0.0;
static float diverge = 0.0;
static float height = 0.0;
static bool isIsotroptic = true;
static bool isAutoOrbit = true;
static float stereoRotation = 0.0;
static float stereoSpread = 0.0;

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
    ts.tv_nsec = (long)1e7; // 1/100 seconds
    
    printf("Setting up\n");
    inputMode = Mach1EncodeInputModeMono;
    outputMode = Mach1EncodeOutputMode8Ch;
    done = false;
    pthread_create(&thread, NULL, &decode, NULL);
    
    while (!done) {
        nanosleep(&ts, NULL);
        auto start = std::chrono::high_resolution_clock::now();
        m1Encode.setRotation(radToDeg(rotation));
        m1Encode.setDiverge(diverge);
        m1Encode.setPitch(radToDeg(height));
        m1Encode.setInputMode(inputMode);
        m1Encode.setOutputMode(outputMode);
        m1Encode.setIsotropicEncode(isIsotroptic);
        m1Encode.setAutoOrbit(isAutoOrbit);
        if (!isAutoOrbit){
            m1Encode.setStereoRotate(stereoRotation);
            m1Encode.setStereoSpread(stereoSpread);
        }
        m1Encode.generatePointResults();
        m1Coeffs = m1Encode.getGains();
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
                rotation += DELTA_ANGLE;
                break;
            case 'a':
                rotation -= DELTA_ANGLE;
                break;
            case 'w':
                diverge += DELTA_DIVERGE;
                break;
            case 's':
                diverge -= DELTA_DIVERGE;
                break;
            case 'x':
                height += DELTA_ANGLE;
                break;
            case 'z':
                height -= DELTA_ANGLE;
                break;
            case 'i':
                if(inputMode==Mach1EncodeInputModeMono){
                    inputMode=Mach1EncodeInputModeStereo;
                }else if(inputMode==Mach1EncodeInputModeStereo){
                    inputMode=Mach1EncodeInputModeQuad;
                }else if(inputMode==Mach1EncodeInputModeQuad){
                    inputMode=Mach1EncodeInputModeLCRS;
                }else if(inputMode==Mach1EncodeInputModeLCRS){
                    inputMode=Mach1EncodeInputModeAFormat;
                }else if(inputMode==Mach1EncodeInputModeAFormat){
                    inputMode=Mach1EncodeInputModeBFormat;
                }else if(inputMode==Mach1EncodeInputModeBFormat){
                    inputMode=Mach1EncodeInputModeBFOAACN;
                }else if(inputMode==Mach1EncodeInputModeBFOAACN){
                    inputMode=Mach1EncodeInputModeBFOAFUMA;
                }else if(inputMode==Mach1EncodeInputModeBFOAFUMA){
                    inputMode=Mach1EncodeInputModeMono;
                }else{
                    printf("Input out of scope.");
                }
                break;
            case 'o':
                if(outputMode==Mach1EncodeOutputMode8Ch){
                    outputMode=Mach1EncodeOutputMode4Ch;
                } else if (outputMode==Mach1EncodeOutputMode4Ch){
                    outputMode=Mach1EncodeOutputMode8Ch;
                }
                break;
            case 'e': //isotropic encode enable
                isIsotroptic = !isIsotroptic;
                break;
            case 't':
                isAutoOrbit = !isAutoOrbit;
                break;
            case 'c':
                stereoRotation += DELTA_ANGLE;
                break;
            case 'v':
                stereoRotation -= DELTA_ANGLE;
                break;
            case 'b':
                stereoSpread += DELTA_DIVERGE;
                break;
            case 'n':
                stereoSpread -= DELTA_DIVERGE;
                break;
            default:
                printf("Input not recognized.\n");
        }
        
        // check that the values are in proper range
        if (diverge < -1.0) diverge = -1.0;
        else if (diverge > 1.0) diverge = 1.0;
        if (stereoSpread < -1.0) stereoSpread = -1.0;
        else if (stereoSpread > 1.0) stereoSpread = 1.0;
        if (rotation < 0) rotation = 2*M_PI;
        else if (rotation > 2*M_PI) rotation = 0;
        if (stereoRotation < 0) stereoRotation = 2*M_PI;
        else if (stereoRotation > 2*M_PI) stereoRotation = 0;
        if (height < -M_PI/2) height = M_PI/2;
        else if (height > M_PI/2) height = -M_PI/2;
        
        // Mach1EncodeCAPI Log:
        printf("\n");
        printf("Input: %u\n", inputMode);
        printf("Output: %u\n", outputMode);
        printf("\n");
        printf("Rotation: %f\n", radToDeg(rotation));
        printf("Diverge: %f\n", diverge);
        printf("Height: %f\n", radToDeg(height));
        printf("\n");
        if(isIsotroptic){
            printf("Isotropic Active\n");
        } else if (!isIsotroptic){
            printf("Periphonic Active\n");
        }
        if(inputMode==1){
            printf("Stereo Spread: %f\n", stereoSpread);
            if(!isAutoOrbit){
                printf("Stereo Rotation: %f\n", radToDeg(stereoRotation));
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
            printf("Number of Channels %i\n", i);
            printf("Channel %i Coeffs: \n", i);
            printf("%f %f %f %f %f %f %f %f\n", m1Coeffs[i][0], m1Coeffs[i][1], m1Coeffs[i][2], m1Coeffs[i][3], m1Coeffs[i][4], m1Coeffs[i][5], m1Coeffs[i][6], m1Coeffs[i][7]);
            printf("\n");
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
