//  Mach1 Spatial SDK
//  Copyright © 2017-2021 Mach1. All rights reserved.

#pragma once

#include "Mach1TranscodeCAPI.h"
#include <string>
#include <vector>

class Mach1Transcode {
  private:
    void *M1obj;

  public:
    Mach1Transcode();
    ~Mach1Transcode();

    int getInputNumChannels();
    int getOutputNumChannels();
    int getFormatFromString(std::string str);
    std::string getFormatName(int fmt);
    std::vector<std::string> getAllTranscodeFormats();

#ifndef __EMSCRIPTEN__
    float processNormalization(float **bufs, int numSamples);
    void processMasterGain(float **bufs, int numSamples, float masterGain = 1.0f);
#endif
    float processNormalization(std::vector<std::vector<float>> &bufs);
    void processMasterGain(std::vector<std::vector<float>> &bufs, float masterGain = 1.0f);

    float db2level(float db);
    float level2db(float level);

    void setLFESub(std::vector<int> subChannelIndices, int sampleRate);
    void setSpatialDownmixer(float corrThreshold = 0.1);
    bool getSpatialDownmixerPossibility();
    std::vector<float> getAvgSamplesDiff();

    void setInputFormat(int inFmt);
    void setInputFormatCustomPointsJson(std::string inJson);
    void setInputFormatCustomPoints(std::vector<Mach1Point3D> points);

    void setOutputFormat(int outFmt);
    void setOutputFormatCustomPointsJson(std::string outJson);
    void setOutputFormatCustomPoints(std::vector<Mach1Point3D> points);

    void setCustomPointsSamplerCallback(Mach1Point3D *(*callback)(long long, int &));

    bool processConversionPath();
    std::vector<std::vector<float>> getMatrixConversion();
#ifndef __EMSCRIPTEN__
    void processConversion(float **inBufs, float **outBufs, int numSamples);
#endif
    void processConversion(std::vector<std::vector<float>> &inBufs, std::vector<std::vector<float>> &outBufs);

    std::vector<int> getFormatConversionPath();
};
