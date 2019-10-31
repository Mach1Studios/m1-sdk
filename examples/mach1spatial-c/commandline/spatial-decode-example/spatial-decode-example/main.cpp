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
#include "Mach1Decode.h"

#define DELTA_ANGLE 0.0174533 // equivalent of 1 degrees in radians
#ifndef PI
#define PI 3.14159265358979323846
#endif

static void* decode(void* v);
static pthread_t thread;
static bool done = false;
Mach1Decode m1Decode;
std::vector<float> m1Coeffs;

// variables for setting yaw/pitch/roll
static float yaw = 0;
static float pitch = 0;
static float roll = 0;

// variables for time logs
float start = 0;
float end = 0;
float timeReturned = 0;

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
        start = m1Decode.getCurrentTime();
        m1Coeffs = m1Decode.decode(radToDeg(yaw), radToDeg(pitch), radToDeg(roll), 0, 0);
        end = m1Decode.getCurrentTime();
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
        printf("Decode Coeffs:\n");
        printf("%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n", m1Coeffs[0], m1Coeffs[1], m1Coeffs[2], m1Coeffs[3], m1Coeffs[4], m1Coeffs[5], m1Coeffs[6], m1Coeffs[7], m1Coeffs[8], m1Coeffs[9], m1Coeffs[10], m1Coeffs[11], m1Coeffs[12], m1Coeffs[13], m1Coeffs[14], m1Coeffs[15]);
        printf("\n");
        printf("Headlock Stereo Coeffs:\n");
        printf("%f %f\n", m1Coeffs[16], m1Coeffs[17]);
        printf("\n");
        printf("Time Elapsed: %f\n", start-end);
        printf("\n");
    }
    printf("\n");
    printf("Exiting\n");
    done = true;
    return NULL;
}
