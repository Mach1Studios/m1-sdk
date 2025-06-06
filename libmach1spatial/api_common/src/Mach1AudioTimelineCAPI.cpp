//  Mach1 Spatial SDK
//  Copyright © 2017 Mach1. All rights reserved.

#include "Mach1AudioTimelineCAPI.h"
#include "Mach1AudioTimelineCore.h"
#include "Mach1Point3D.h"

void *Mach1AudioObjectCAPI_create() {
    return new Mach1AudioObjectCore();
}

void Mach1AudioObjectCAPI_delete(void *M1obj) {
    if (M1obj != nullptr) {
        delete (Mach1AudioObjectCore *)M1obj;
        M1obj = nullptr;
    }
}

const char *Mach1AudioObjectCAPI_getName(void *M1obj) {
    return ((Mach1AudioObjectCore *)M1obj)->name.c_str();
}

int Mach1AudioObjectCAPI_getKeyPointsCount(void *M1obj) {
    return (int)((Mach1AudioObjectCore *)M1obj)->points.size();
}

Mach1KeyPoint Mach1AudioObjectCAPI_getKeyPoint(void *M1obj, int n) {
    return ((Mach1AudioObjectCore *)M1obj)->points[n];
}

Mach1KeyPoint *Mach1AudioObjectCAPI_getKeyPoints(void *M1obj) {
    return ((Mach1AudioObjectCore *)M1obj)->points.data();
}

void Mach1AudioObjectCAPI_setName(void *M1obj, const char *name) {
    ((Mach1AudioObjectCore *)M1obj)->name = name;
}

void Mach1AudioObjectCAPI_setKeyPoints(void *M1obj, Mach1KeyPoint *points, int n) {
    ((Mach1AudioObjectCore *)M1obj)->points = std::vector<Mach1KeyPoint>(points, points + n);
}

void *Mach1AudioTimelineCAPI_create() {
    return new Mach1AudioTimelineCore();
}

void Mach1AudioTimelineCAPI_delete(void *M1obj) {
    if (M1obj != nullptr) {
        delete (Mach1AudioTimelineCore *)M1obj;
        M1obj = nullptr;
    }
}

int Mach1AudioTimelineCAPI_getAudioObjectCount(void *M1obj) {
    return (int)((Mach1AudioTimelineCore *)M1obj)->getAudioObjects()->size();
}

void *Mach1AudioTimelineCAPI_getAudioObject(void *M1obj, int n) {
    return (void *)&(((Mach1AudioTimelineCore *)M1obj)->getAudioObjects()->operator[](n));
}

void Mach1AudioTimelineCAPI_parseADM(void *M1obj, char *inXml) {
    ((Mach1AudioTimelineCore *)M1obj)->parseADM(inXml);
}

void Mach1AudioTimelineCAPI_parseAtmos(void *M1obj, char *inDotAtmos, char *inDotAtmosDotMetadata) {
    ((Mach1AudioTimelineCore *)M1obj)->parseAtmos(inDotAtmos, inDotAtmosDotMetadata);
}
