//  Mach1 Spatial SDK
//  Copyright © 2017-2020 Mach1. All rights reserved.

#include "Mach1TranscodeCAPI.h"
#include "Mach1Point3DCore.h"
#include "Mach1TranscodeCore.h"

void *Mach1TranscodeCAPI_create() {
    return new Mach1TranscodeCore();
}

void Mach1TranscodeCAPI_delete(void *M1obj) {
    if (M1obj != nullptr) {
        delete (Mach1TranscodeCore *)M1obj;
        M1obj = nullptr;
    }
}

int Mach1TranscodeCAPI_getInputNumChannels(void *M1obj) {
    return ((Mach1TranscodeCore *)M1obj)->getInputNumChannels();
}

int Mach1TranscodeCAPI_getOutputNumChannels(void *M1obj) {
    return ((Mach1TranscodeCore *)M1obj)->getOutputNumChannels();
}

int Mach1TranscodeCAPI_getFormatFromString(void *M1obj, const char *str) {
    return (int)((Mach1TranscodeCore *)M1obj)->getFormatFromString(str);
}

const char* Mach1TranscodeCAPI_getFormatName(void *M1obj, int fmt) {
    return ((Mach1TranscodeCore *)M1obj)->getFormatName((int)fmt);
}

const char** Mach1TranscodeCAPI_getAllFormatNames(void *M1obj) {
    return ((Mach1TranscodeCore *)M1obj)->getAllFormatNames();
}

int Mach1TranscodeCAPI_getFormatsCount(void *M1obj) {
    return ((Mach1TranscodeCore *)M1obj)->getFormatsCount();
}

float Mach1TranscodeCAPI_processNormalization(void *M1obj, float **bufs, int numSamples) {
    return ((Mach1TranscodeCore *)M1obj)->processNormalization(bufs, numSamples);
}

void Mach1TranscodeCAPI_processMasterGain(void *M1obj, float **bufs, int numSamples, float masterGain) {
    ((Mach1TranscodeCore *)M1obj)->processMasterGain(bufs, numSamples, masterGain);
}

float Mach1TranscodeCAPI_db2level(void *M1obj, float db) {
    return ((Mach1TranscodeCore *)M1obj)->db2level(db);
}

float Mach1TranscodeCAPI_level2db(void *M1obj, float level) {
    return ((Mach1TranscodeCore *)M1obj)->level2db(level);
}

void Mach1TranscodeCAPI_setLFESub(void *M1obj, int *subChannelIndices, int numChannels, int sampleRate) {
    std::vector<int> values(subChannelIndices, subChannelIndices + numChannels);
    ((Mach1TranscodeCore *)M1obj)->setLFESub(values, sampleRate);
}

void Mach1TranscodeCAPI_setSpatialDownmixer(void *M1obj, float corrThreshold) {
    ((Mach1TranscodeCore *)M1obj)->setSpatialDownmixer(corrThreshold);
}

bool Mach1TranscodeCAPI_getSpatialDownmixerPossibility(void *M1obj) {
    return ((Mach1TranscodeCore *)M1obj)->getSpatialDownmixerPossibility();
}

float *Mach1TranscodeCAPI_getAvgSamplesDiff(void *M1obj) {
    const std::vector<float> &avgSamplesDiff = ((Mach1TranscodeCore *)M1obj)->getAvgSamplesDiff();
    return (float *)avgSamplesDiff.data();
}

void Mach1TranscodeCAPI_setInputFormat(void *M1obj, int inFmt) {
    ((Mach1TranscodeCore *)M1obj)->setInputFormat((int)inFmt);
}

void Mach1TranscodeCAPI_setInputFormatCustomPointsJson(void *M1obj, char *strJson) {
    ((Mach1TranscodeCore *)M1obj)->setInputFormatCustomPointsJson(strJson);
}

void Mach1TranscodeCAPI_setInputFormatCustomPoints(void *M1obj, Mach1Point3D *points, int count) {
    ((Mach1TranscodeCore *)M1obj)->setInputFormatCustomPoints(std::vector<Mach1Point3DCore>((Mach1Point3DCore *)points, (Mach1Point3DCore *)points + count));
}

void Mach1TranscodeCAPI_setOutputFormat(void *M1obj, int outFmt) {
    ((Mach1TranscodeCore *)M1obj)->setOutputFormat((int)outFmt);
}

void Mach1TranscodeCAPI_setOutputFormatCustomPointsJson(void *M1obj, char *strJson) {
    ((Mach1TranscodeCore *)M1obj)->setOutputFormatCustomPointsJson(strJson);
}

void Mach1TranscodeCAPI_setOutputFormatCustomPoints(void *M1obj, Mach1Point3D *points, int count) {
    ((Mach1TranscodeCore *)M1obj)->setOutputFormatCustomPoints(std::vector<Mach1Point3DCore>((Mach1Point3DCore *)points, (Mach1Point3DCore *)points + count));
}

void Mach1TranscodeCAPI_setCustomPointsSamplerCallback(void *M1obj, Mach1Point3D *(*callback)(long long, int &)) {
    ((Mach1TranscodeCore *)M1obj)->setCustomPointsSamplerCallback(callback);
}

bool Mach1TranscodeCAPI_processConversionPath(void *M1obj) {
    return ((Mach1TranscodeCore *)M1obj)->processConversionPath();
}

void Mach1TranscodeCAPI_processConversion(void *M1obj, float **inBufs, float **outBufs, int numSamples) {
    ((Mach1TranscodeCore *)M1obj)->processConversion(inBufs, outBufs, numSamples);
}

void Mach1TranscodeCAPI_getMatrixConversion(void *M1obj, float *matrix) {
    ((Mach1TranscodeCore *)M1obj)->getMatrixConversion(matrix);
}

int *Mach1TranscodeCAPI_getFormatConversionPath(void *M1obj, int *count) {
    std::vector<int> &formatsPath = ((Mach1TranscodeCore *)M1obj)->getFormatConversionPath();
    *count = (int)formatsPath.size();
    return (int *)formatsPath.data();
}