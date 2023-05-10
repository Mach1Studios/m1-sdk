//  Mach1 Spatial SDK
//  Copyright © 2017-2022 Mach1. All rights reserved.

#include "Mach1TranscodeCore.h"
#include "Mach1EncodeCore.h"
#include "Mach1GenerateCoeffs.h"
#include "Mach1TranscodeAmbisonicFormats.h"
#include "Mach1TranscodeMicArrayFormats.h"
#include "Mach1TranscodeSurroundFormats.h"
#include "Mach1TranscodeVectorFormats.h"
#include "json/json.h"
#include <cstring>
#include <string.h>

#define _USE_MATH_DEFINES
#include <cmath>

#ifndef M_LN10
#    define M_LN10 2.30258509299404568402
#endif

Mach1TranscodeCore::Mach1TranscodeCore() {
    inFmt = -1;
    outFmt = -1;

    bufferSize = 4096;
    for (int i = 0; i < Mach1TranscodeConstants::MAXCHANS; i++) {
        buffers[i] = new float[bufferSize];
    }

    {
        std::vector<Mach1TranscodeMatrix> m = Mach1TranscodeConstants::MatricesAmbisonic::getData();
        matrices.insert(matrices.end(), m.begin(), m.end());
    }
    {
        std::vector<Mach1TranscodeMatrix> m = Mach1TranscodeConstants::MatricesSurround::getData();
        matrices.insert(matrices.end(), m.begin(), m.end());
    }
    {
        std::vector<Mach1TranscodeMatrix> m = Mach1TranscodeConstants::MatricesVector::getData();
        matrices.insert(matrices.end(), m.begin(), m.end());
    }
    {
        std::vector<Mach1TranscodeMatrix> m = Mach1TranscodeConstants::MatricesMicArray::getData();
        matrices.insert(matrices.end(), m.begin(), m.end());
    }
}

Mach1TranscodeCore::~Mach1TranscodeCore() {
    for (int i = 0; i < Mach1TranscodeConstants::MAXCHANS; i++) {
        delete[] buffers[i];
    }
}

int Mach1TranscodeCore::getInputNumChannels() {
    return getNumChannels(inFmt, true);
}

int Mach1TranscodeCore::getOutputNumChannels() {
    return getNumChannels(outFmt, false);
}

int Mach1TranscodeCore::getFormatFromString(const char *str) {
    for (auto it = Mach1TranscodeConstants::formats.begin(); it != Mach1TranscodeConstants::formats.end(); ++it) {
        if (std::strcmp(str, it->name.data()) == 0) {
            return it - Mach1TranscodeConstants::formats.begin();
        }
    }
    return -1;
}

const char *Mach1TranscodeCore::getFormatName(int fmt) {
    if (fmt < Mach1TranscodeConstants::formats.size()) {
        return Mach1TranscodeConstants::formats[fmt].name.data();
    }
    return "";
}

std::vector<std::string> Mach1TranscodeCore::getAllFormatNames() {
    std::vector<std::string> formatNames;
    for (auto it = Mach1TranscodeConstants::formats.begin(); it != Mach1TranscodeConstants::formats.end(); ++it) {
        formatNames.push_back(it->name.data());
    }
    return formatNames;
}

float Mach1TranscodeCore::processNormalization(float **bufs, int numSamples) {
    float peak = 0;

    int nChannels = getOutputNumChannels();
    for (int i = 0; i < nChannels; i++) {
        for (int sample = 0; sample < numSamples; sample++) {
            float tmp = fabs(bufs[i][sample]);
            if (tmp > peak)
                peak = tmp;
        }
    }
    return peak;
}

void Mach1TranscodeCore::processMasterGain(float **bufs, int numSamples, float masterGain) {
    if (masterGain == 1.0)
        return;
    if (outFmt == -1)
        return;

    int nChannels = getOutputNumChannels();
    for (int i = 0; i < nChannels; i++) {
        for (int sample = 0; sample < numSamples; sample++) {
            bufs[i][sample] *= masterGain;
        }
    }
}

void Mach1TranscodeCore::setLFESub(std::vector<int> subChannelIndices, int sampleRate) {
    this->subChannelIndices = subChannelIndices;

    lpFilters.resize(subChannelIndices.size());
    for (int k = 0; k < lpFilters.size(); k++) {
        lpFilters[k].SetSampleRate(sampleRate);
        lpFilters[k].Setup(100, 0.0f);
    }
}

void Mach1TranscodeCore::setSpatialDownmixer(float corrThreshold) {
    spatialDownmixChecker.Setup(corrThreshold);
}

bool Mach1TranscodeCore::getSpatialDownmixerPossibility() {
    return spatialDownmixChecker.bShouldDownmix();
}

const std::vector<float> Mach1TranscodeCore::getAvgSamplesDiff() {
    return spatialDownmixChecker.getAvgSamplesDiff();
}

void Mach1TranscodeCore::setInputFormat(int inFmt) {
    this->inFmt = inFmt;
}

std::vector<Mach1Point3DCore> parseCustomPointsJson(std::string srtJson) {
    std::vector<Mach1Point3DCore> points;

    auto doc = JSON::parse(srtJson);
    if (doc.size() > 0) {
        auto jsonPoints = JSON::getChildren(doc, "points");
        if (jsonPoints.size() > 0) {
            for (int i = 0;; i++) {
                auto jsonPoint = JSON::getElement(jsonPoints, 3, i);
                if (jsonPoint.size() > 0) {
                    points.push_back(
                        Mach1Point3DCore(
                            std::stof(JSON::getChildren(jsonPoint, "x")[0]->value),
                            std::stof(JSON::getChildren(jsonPoint, "y")[0]->value),
                            std::stof(JSON::getChildren(jsonPoint, "z")[0]->value)));
                }
            }
        }
    }
    for (int i = 0; i < doc.size(); i++)
        delete doc[i];
    return points;
}

void Mach1TranscodeCore::setInputFormatCustomPointsJson(std::string strJson) {
    inFmt = getFormatFromString("CustomPoints");
    inCustomPoints = parseCustomPointsJson(strJson);
}

void Mach1TranscodeCore::setInputFormatCustomPoints(std::vector<Mach1Point3DCore> points) {
    inFmt = getFormatFromString("CustomPoints");
    inCustomPoints = points;
}

void Mach1TranscodeCore::setOutputFormat(int outFmt) {
    this->outFmt = outFmt;
}

void Mach1TranscodeCore::setOutputFormatCustomPointsJson(std::string strJson) {
    outFmt = getFormatFromString("CustomPoints");
    outCustomPoints = parseCustomPointsJson(strJson);
}

void Mach1TranscodeCore::setOutputFormatCustomPoints(std::vector<Mach1Point3DCore> points) {
    outFmt = getFormatFromString("CustomPoints");
    outCustomPoints = points;
}

void Mach1TranscodeCore::setCustomPointsSamplerCallback(Mach1Point3D *(*callback)(long long, int &)) {
    customPointsSamplerCallback = callback;
}

bool Mach1TranscodeCore::processConversionPath() {
    // compute tree of pathes from inFmt to all others
    struct Node {
        int prev;
        int fmt;
        bool processed;
    };

    std::vector<Node> tree;
    Node node;
    node.fmt = inFmt;
    node.prev = -1;
    node.processed = false;
    tree.push_back(node);

    for (int i = 0; i < tree.size(); i++) {
        if (tree[i].processed == false) {
            int fmt = tree[i].fmt;
            for (int j = 0; j < Mach1TranscodeConstants::formats.size(); j++) {
                if (findMatrix(fmt, j) >= 0) {

                    // check if this format already exist on that path
                    int k = i;
                    bool exist = false;
                    while (!exist && k != -1) {
                        if (tree[k].fmt == j) {
                            exist = true;
                            break;
                        } else {
                            k = tree[k].prev;
                        }
                    }

                    if (exist == false) {
                        Node node;
                        node.fmt = (int)j;
                        node.prev = i;
                        node.processed = false;
                        tree.push_back(node);
                    }
                }
            }
            tree[i].processed = true;
        }
    }

    // find shortest path first
    std::vector<int> intermediateFormatsList;
    intermediateFormatsList.reserve(100);
    for (int i = 0; i < tree.size(); i++) {
        if (tree[i].fmt == outFmt) {
            for (int k = i; k != -1; k = tree[k].prev) {
                intermediateFormatsList.insert(intermediateFormatsList.begin(), tree[k].fmt); // push_front
            }
            break;
        }
    }

    if (intermediateFormatsList.size() < 2 && inFmt != outFmt) {
        formatConversionPath.clear();
        return false;
    } else {
        formatConversionPath = intermediateFormatsList;
        return true;
    }
}

std::vector<Mach1Point3DCore> Mach1TranscodeCore::getPointsSet(int fmt) {
    /*
     * X = Left(-) to Right(+) (from a top down perspective)
     * Y = Front(+) to Back(-) (from a top down perspective)
     * Z = Top(+) to Bottom(-) (from a top down perspective)
     */

    static std::vector<Mach1Point3DCore> m1Spatial_4_Def = {
        {-1, 1, 0},
        {1, 1, 0},
        {-1, -1, 0},
        {1, -1, 0}};

    static std::vector<Mach1Point3DCore> m1Spatial_8_Def = {
        {-1, 1, 1},
        {1, 1, 1},
        {-1, -1, 1},
        {1, -1, 1},

        {-1, 1, -1},
        {1, 1, -1},
        {-1, -1, -1},
        {1, -1, -1}};

    static std::vector<Mach1Point3DCore> m1Spatial_12_Def = {
        {-1, 1, 1},
        {1, 1, 1},
        {-1, -1, 1},
        {1, -1, 1},

        {-1, 1, -1},
        {1, 1, -1},
        {-1, -1, -1},
        {1, -1, -1},

        {0, 1 / 0.707, 0},
        {1 / 0.707, 0, 0},
        {0, -1 / 0.707, 0},
        {-1 / 0.707, 0, 0}};

    static std::vector<Mach1Point3DCore> m1Spatial_14_Def = {
        {-1, 1, 1},
        {1, 1, 1},
        {-1, -1, 1},
        {1, -1, 1},

        {-1, 1, -1},
        {1, 1, -1},
        {-1, -1, -1},
        {1, -1, -1},

        {0, 1 / 0.707, 0},
        {1 / 0.707, 0, 0},
        {0, -1 / 0.707, 0},
        {-1 / 0.707, 0, 0},

        {0, 0, 1 / 0.707},
        {0, 0, -1 / 0.707}};

    static std::vector<Mach1Point3DCore> m1Spatial_32_Def = {
        {0.00000, 1.32048, 0.50689},
        {-0.74953, 1.19950, 0.00000},
        {0.00000, 1.32048, -0.50689},
        {0.74953, 1.19950, 0.00000},
        {0.00000, 0.74953, 1.19950},
        {-0.81928, 0.81928, 0.81128},
        {-1.32048, 0.50689, 0.00000},
        {-0.81928, 0.81928, -0.81128},
        {0.00000, 0.74953, -1.19950},
        {0.81928, 0.81928, -0.81128},
        {1.32048, 0.50689, 0.00000},
        {0.81928, 0.81928, 0.81128},
        {-0.50681, -0.00885, 1.32048},
        {-1.19950, -0.00000, 0.74953},
        {-1.21240, -0.00000, -0.72848},
        {-0.50681, 0.00885, -1.32048},
        {0.00000, -1.32048, 0.50689},
        {0.74953, -1.19950, 0.00000},
        {0.00000, -1.32048, -0.50689},
        {-0.74953, -1.19950, 0.00000},
        {0.00000, -0.74953, 1.19950},
        {0.81928, -0.81928, 0.81128},
        {1.32048, -0.50689, 0.00000},
        {0.81928, -0.81928, -0.81128},
        {0.00000, -0.74953, -1.19950},
        {-0.81928, -0.81928, -0.81128},
        {-1.32048, -0.50689, 0.00000},
        {-0.81928, -0.81928, 0.81128},
        {0.50681, -0.00885, 1.32048},
        {1.19950, 0.00000, 0.74953},
        {1.19950, 0.00000, -0.74953},
        {0.50681, 0.00885, -1.32048}};

    static std::vector<Mach1Point3DCore> m1Spatial_36_Def = {
        {-0.43310, 0.71779, 1.13922},
        {1.13922, -0.43310, 0.71779},
        {0.43310, -0.71779, 1.13922},
        {0.71779, 1.13922, -0.43310},
        {1.13922, 0.43310, -0.71779},
        {-0.71779, 1.13922, 0.43310},
        {-1.13922, 0.43310, 0.71779},
        {-0.71779, -1.13922, -0.43310},
        {-1.13922, -0.43310, -0.71779},
        {0.71779, -1.13922, 0.43310},
        {0.43310, 0.71779, -1.13922},
        {-0.43310, -0.71779, -1.13922},
        {-0.34445, 0.88595, -1.04740},
        {-1.04740, -0.34445, 0.88595},
        {0.34445, -0.88595, -1.04740},
        {0.88595, -1.04740, -0.34445},
        {-1.04740, 0.34445, -0.88595},
        {-0.88595, -1.04740, 0.34445},
        {1.04740, 0.34445, 0.88595},
        {-0.88595, 1.04740, -0.34445},
        {1.04740, -0.34445, -0.88595},
        {0.88595, 1.04740, 0.34445},
        {0.34445, 0.88595, 1.04740},
        {-0.34445, -0.88595, 1.04740},
        {1.35378, -0.40488, -0.06298},
        {-0.06298, 1.35378, -0.40488},
        {-1.35378, 0.40488, -0.06298},
        {-0.40488, -0.06298, 1.35378},
        {-0.06298, -1.35378, 0.40488},
        {0.40488, -0.06298, -1.35378},
        {0.06298, -1.35378, -0.40488},
        {0.40488, 0.06298, 1.35378},
        {0.06298, 1.35378, 0.40488},
        {-0.40488, 0.06298, -1.35378},
        {-1.35378, -0.40488, 0.06298},
        {1.35378, 0.40488, 0.06298}};

    static std::vector<Mach1Point3DCore> m1Spatial_48_Def = {
        {0.50006, 1.32018, -0.08763},
        {-0.50006, 1.32018, 0.08763},
        {-0.08763, 1.32018, -0.50006},
        {0.08763, 1.32018, 0.50006},
        {0.50006, -1.32018, 0.08763},
        {-0.50006, -1.32018, -0.08763},
        {-0.08763, -1.32018, 0.50006},
        {0.08763, -1.32018, -0.50006},
        {1.32018, -0.08763, 0.50006},
        {1.32018, 0.08763, -0.50006},
        {1.32018, -0.50006, -0.08763},
        {1.32018, 0.50006, 0.08763},
        {-1.32018, 0.08763, 0.50006},
        {-1.32018, -0.08763, -0.50006},
        {-1.32018, 0.50006, -0.08763},
        {-1.32018, -0.50006, 0.08763},
        {-0.08763, 0.50006, 1.32018},
        {0.08763, -0.50006, 1.32018},
        {-0.50006, -0.08763, 1.32018},
        {0.50006, 0.08763, 1.32018},
        {0.08763, 0.50006, -1.32018},
        {-0.08763, -0.50006, -1.32018},
        {0.50006, -0.08763, -1.32018},
        {-0.50006, 0.08763, -1.32018},
        {0.90487, 0.99978, 0.42693},
        {-0.90487, 0.99978, -0.42693},
        {0.42693, 0.99978, -0.90487},
        {-0.42693, 0.99978, 0.90487},
        {0.90487, -0.99978, -0.42693},
        {-0.90487, -0.99978, 0.42693},
        {0.42693, -0.99978, 0.90487},
        {-0.42693, -0.99978, -0.90487},
        {0.99978, 0.42693, 0.90487},
        {0.99978, -0.42693, -0.90487},
        {0.99978, -0.90487, 0.42693},
        {0.99978, 0.90487, -0.42693},
        {-0.99978, -0.42693, 0.90487},
        {-0.99978, 0.42693, -0.90487},
        {-0.99978, 0.90487, 0.42693},
        {-0.99978, -0.90487, -0.42693},
        {0.42693, 0.90487, 0.99978},
        {-0.42693, -0.90487, 0.99978},
        {-0.90487, 0.42693, 0.99978},
        {0.90487, -0.42693, 0.99978},
        {-0.42693, 0.90487, -0.99978},
        {0.42693, -0.90487, -0.99978},
        {0.90487, 0.42693, -0.99978},
        {-0.90487, -0.42693, -0.99978}};

    static std::vector<Mach1Point3DCore> m1Spatial_60_Def = {
        {0.77221, -1.06624, -0.51714},
        {-0.51714, 0.77221, -1.06624},
        {-0.77221, 1.06624, -0.51714},
        {-1.06624, -0.51714, 0.77221},
        {-0.51714, -0.77221, 1.06624},
        {1.06624, -0.51714, -0.77221},
        {0.51714, -0.77221, -1.06624},
        {1.06624, 0.51714, 0.77221},
        {0.51714, 0.77221, 1.06624},
        {-1.06624, 0.51714, -0.77221},
        {-0.77221, -1.06624, 0.51714},
        {0.77221, 1.06624, 0.51714},
        {-1.00870, 0.99036, 0.04822},
        {0.04822, -1.00870, 0.99035},
        {1.00870, -0.99036, 0.04822},
        {0.99036, 0.04822, -1.00870},
        {0.04822, 1.00870, -0.99036},
        {-0.99036, 0.04822, 1.00870},
        {-0.04822, 1.00870, 0.99036},
        {-0.99036, -0.04822, -1.00870},
        {-0.04822, -1.00870, -0.99035},
        {0.99036, -0.04822, 1.00870},
        {1.00870, 0.99036, -0.04822},
        {-1.00870, -0.99036, -0.04822},
        {0.10898, 0.39071, -1.35502},
        {-1.35502, 0.10898, 0.39071},
        {-0.10898, -0.39071, -1.35502},
        {0.39071, -1.35502, 0.10898},
        {-1.35502, -0.10898, -0.39071},
        {-0.39071, -1.35502, -0.10898},
        {1.35502, -0.10898, 0.39071},
        {-0.39071, 1.35502, 0.10898},
        {1.35502, 0.10898, -0.39071},
        {0.39071, 1.35502, -0.10898},
        {-0.10898, 0.39071, 1.35502},
        {0.10898, -0.39071, 1.35502},
        {-1.10800, 0.63907, 0.60377},
        {0.60377, -1.10800, 0.63907},
        {1.10800, -0.63907, 0.60377},
        {0.63907, 0.60377, -1.10800},
        {0.60377, 1.10800, -0.63907},
        {-0.63907, 0.60377, 1.10800},
        {-0.60377, 1.10800, 0.63907},
        {-0.63907, -0.60377, -1.10800},
        {-0.60377, -1.10800, -0.63907},
        {0.63907, -0.60377, 1.10800},
        {1.10800, 0.63907, -0.60377},
        {-1.10800, -0.63907, -0.60377},
        {-1.31996, -0.47890, 0.17020},
        {0.17020, -1.31996, -0.47890},
        {1.31996, 0.47890, 0.17020},
        {-0.47890, 0.17020, -1.31996},
        {0.17020, 1.31996, 0.47890},
        {0.47890, 0.17020, 1.31996},
        {-0.17020, 1.31996, -0.47890},
        {0.47890, -0.17020, -1.31996},
        {-0.17020, -1.31996, 0.47890},
        {-0.47890, -0.17020, 1.31996},
        {1.31996, -0.47890, -0.17020},
        {-1.31996, 0.47890, -0.17020}};

    static std::map<int, std::vector<Mach1Point3DCore>> standards = {
        {getFormatFromString("M1Horizon"), m1Spatial_4_Def},
        {getFormatFromString("M1Spatial-4"), m1Spatial_4_Def},
        {getFormatFromString("M1Spatial"), m1Spatial_8_Def},
        {getFormatFromString("M1Spatial-8"), m1Spatial_8_Def},
        {getFormatFromString("M1Spatial-12"), m1Spatial_12_Def},
        {getFormatFromString("M1Spatial-14"), m1Spatial_14_Def},
        {getFormatFromString("M1Spatial-32"), m1Spatial_32_Def},
        {getFormatFromString("M1Spatial-36"), m1Spatial_36_Def},
        {getFormatFromString("M1Spatial-48"), m1Spatial_48_Def},
        {getFormatFromString("M1Spatial-60"), m1Spatial_60_Def},
    };

    std::vector<Mach1Point3DCore> vec;
    if (standards.find(fmt) != standards.end()) {
        vec = standards[fmt];
    }
    return vec;
}

int Mach1TranscodeCore::findMatrix(int inFmt, int outFmt) {
    const char *inFmtName = getFormatName(inFmt);
    const char *outFmtName = getFormatName(outFmt);

    for (int i = 0; i < matrices.size(); i++) {
        if (matrices[i].formatFrom == inFmtName && matrices[i].formatTo == outFmtName) {
            return i;
        }
    }
    return -1;
}

std::vector<std::vector<float>> Mach1TranscodeCore::getCoeffs(int idxMatrix) {
    std::vector<std::vector<float>> coeffs;

    if (idxMatrix >= 0) {
        for (int i = 0; i < matrices[idxMatrix].channels.size(); i++) {
            Mach1TranscodeChannelBase *channel = matrices[idxMatrix].channels[i];
            if (Mach1TranscodeCoeffs *c = dynamic_cast<Mach1TranscodeCoeffs *>(channel)) {
                coeffs.push_back(c->data);
            } else if (Mach1TranscodePanner *p = dynamic_cast<Mach1TranscodePanner *>(channel)) {
                M1EncodeCore m1encode;

                int inputMode = m1encode.getInputModeFromString(matrices[idxMatrix].formatFrom);
                int outputMode = m1encode.getOutputModeFromString(matrices[idxMatrix].formatTo);

                m1encode.setInputMode((M1EncodeCore::InputMode)inputMode);
                m1encode.setOutputMode((M1EncodeCore::OutputMode)outputMode);
                m1encode.setDiverge(p->diverge);
                m1encode.setAzimuthDegrees(p->azimuth);
                m1encode.setElevationDegrees(p->elevation);
                m1encode.setOutputGain(p->outputGain.gain, p->outputGain.isDecibel);
                m1encode.generatePointResults();
                std::vector<std::vector<float>> gains = m1encode.resultingPoints.getGains();
                coeffs.push_back(gains[i]);
            }
        }
    }

    std::vector<std::vector<float>> coeffsFlipped(coeffs[0].size(), std::vector<float>(coeffs.size()));
    for (size_t i = 0; i < coeffs.size(); i++) {
        for (size_t j = 0; j < coeffs[0].size(); j++) {
            coeffsFlipped[j][i] = coeffs[i][j];
        }
    }
    return coeffsFlipped;
}

void Mach1TranscodeCore::processConversion(int inFmt, float **inBufs, int outFmt, float **outBufs, int numSamples) {
    int inChans = getNumChannels(inFmt, true);
    int outChans = getNumChannels(outFmt, false);
    std::vector<std::vector<float>> currentFormatConversionMatrix;

    if (inFmt == getFormatFromString("CustomPoints") && outFmt != getFormatFromString("CustomPoints")) {
        currentFormatConversionMatrix = generateCoeffSetForPoints(inCustomPoints, getPointsSet(outFmt));
    } else if (inFmt != getFormatFromString("CustomPoints") && outFmt == getFormatFromString("CustomPoints")) {
        currentFormatConversionMatrix = generateCoeffSetForPoints(getPointsSet(inFmt), outCustomPoints);
    } else if (inFmt == getFormatFromString("CustomPoints") && outFmt == getFormatFromString("CustomPoints")) {
        currentFormatConversionMatrix = generateCoeffSetForPoints(inCustomPoints, outCustomPoints);
    } else if (inFmt != getFormatFromString("CustomPoints") && outFmt != getFormatFromString("CustomPoints")) {
        currentFormatConversionMatrix = getCoeffs(findMatrix(inFmt, outFmt));
    }

    // copy pointers to local
    float *inPtrs[Mach1TranscodeConstants::MAXCHANS];
    if (inBufs != NULL) {
        for (int i = 0; i < inChans; i++)
            inPtrs[i] = inBufs[i];
    }
    float *outPtrs[Mach1TranscodeConstants::MAXCHANS];
    if (outBufs != NULL) {
        for (int i = 0; i < outChans; i++)
            outPtrs[i] = outBufs[i];
    }

    // matrix mult each sample
    float ins[Mach1TranscodeConstants::MAXCHANS];
    for (int sample = 0; sample < numSamples; sample++) {
        for (int inChannel = 0; inChannel < inChans; inChannel++)
            ins[inChannel] = inPtrs[inChannel][sample];
        for (int channel = inChans; channel < Mach1TranscodeConstants::MAXCHANS; channel++)
            ins[channel] = 0;

        if (inFmt == getFormatFromString("CustomPoints") && customPointsSamplerCallback != nullptr) {
            // updates the points
            inCustomPoints.clear();

            int cnt = 0;
            Mach1Point3D *points = customPointsSamplerCallback(sample, cnt);
            for (int i = 0; i < cnt; i++) {
                inCustomPoints.push_back(*(Mach1Point3DCore *)&points[i]);
            }

            currentFormatConversionMatrix = generateCoeffSetForPoints(inCustomPoints, outCustomPoints);
        }

        for (int outChannel = 0; outChannel < outChans; outChannel++) {
            float out = 0;
            for (int inChannel = 0; inChannel < inChans; inChannel++)
                out += currentFormatConversionMatrix[outChannel][inChannel] * ins[inChannel]; // check
            outPtrs[outChannel][sample] = out;
        }

        for (int outChannel = 0; outChannel < outChans; outChannel++)
            ins[outChannel] = outPtrs[outChannel][sample];
        for (int outChannel = outChans; outChannel < Mach1TranscodeConstants::MAXCHANS; outChannel++)
            ins[outChannel] = 0;
    }
}

int Mach1TranscodeCore::getNumChannels(int fmt, bool isInput) {
    if (fmt == getFormatFromString("CustomPoints")) {
        return isInput ? (int)inCustomPoints.size() : (int)outCustomPoints.size();
    }
    return Mach1TranscodeConstants::formats[fmt].numChannels;
}

float Mach1TranscodeCore::db2level(float db) {
    return (float)exp(M_LN10 * ((double)db / 20.0));
}

float Mach1TranscodeCore::level2db(float level) {
    return (float)(20.0f * log((double)level) / M_LN10);
}

void Mach1TranscodeCore::getMatrixConversion(float *matrix) {
    float mPrev[Mach1TranscodeConstants::MAXCHANS][Mach1TranscodeConstants::MAXCHANS];
    float mCurrent[Mach1TranscodeConstants::MAXCHANS][Mach1TranscodeConstants::MAXCHANS];
    float mRes[Mach1TranscodeConstants::MAXCHANS][Mach1TranscodeConstants::MAXCHANS];
    int mSize = Mach1TranscodeConstants::MAXCHANS * Mach1TranscodeConstants::MAXCHANS * sizeof(float);

    int prevInChans = 0;
    int prevOutChans = 0;

    std::memset(mPrev, 0, mSize);
    std::memset(mCurrent, 0, mSize);
    std::memset(mRes, 0, mSize);

    for (int k = 0; k < formatConversionPath.size() - 1; k++) {
        int inFmt = formatConversionPath[k];
        int outFmt = formatConversionPath[k + 1];

        int inChans = getNumChannels(inFmt, true);
        int outChans = getNumChannels(outFmt, false);
        std::vector<std::vector<float>> currentFormatConversionMatrix;

        if (inFmt == getFormatFromString("CustomPoints") && outFmt != getFormatFromString("CustomPoints")) {
            currentFormatConversionMatrix = generateCoeffSetForPoints(inCustomPoints, getPointsSet(outFmt));
        } else if (inFmt != getFormatFromString("CustomPoints") && outFmt == getFormatFromString("CustomPoints")) {
            currentFormatConversionMatrix = generateCoeffSetForPoints(getPointsSet(inFmt), outCustomPoints);
        } else if (inFmt == getFormatFromString("CustomPoints") && outFmt == getFormatFromString("CustomPoints")) {
            currentFormatConversionMatrix = generateCoeffSetForPoints(inCustomPoints, outCustomPoints);
        } else if (inFmt != getFormatFromString("CustomPoints") && outFmt != getFormatFromString("CustomPoints")) {
            currentFormatConversionMatrix = getCoeffs(findMatrix(inFmt, outFmt));
        }

        std::memset(mCurrent, 0, mSize);
        for (int outChannel = 0; outChannel < outChans; outChannel++) {
            for (int inChannel = 0; inChannel < inChans; inChannel++) {
                mCurrent[outChannel][inChannel] = currentFormatConversionMatrix[outChannel][inChannel];
            }
        }

        if (k == 0) {
            std::memcpy(mRes, mCurrent, mSize);
        } else {
            std::memset(mRes, 0, mSize);
            // multiply matrix
            for (int i = 0; i < outChans; i++) {
                for (int j = 0; j < prevInChans; j++) {
                    for (int k = 0; k < inChans; k++) {
                        mRes[i][j] += mCurrent[i][k] * mPrev[k][j];
                    }
                }
            }
        }

        prevInChans = inChans;
        prevOutChans = outChans;
        std::memcpy(mPrev, mRes, Mach1TranscodeConstants::MAXCHANS * Mach1TranscodeConstants::MAXCHANS * sizeof(float));
    }

    int inChans = getInputNumChannels();
    int outChans = getOutputNumChannels();
    for (size_t i = 0; i < outChans; i++) {
        for (size_t j = 0; j < inChans; j++) {
            matrix[i * inChans + j] = mRes[i][j];
        }
    }

    // make identity matrix if the formats are equal
    if (inFmt == outFmt) {
        for (size_t i = 0; i < inChans; i++) {
            matrix[i * inChans + i] = 1;
        }
    }

    /*
    // debug output
    printf("std::vector<std::vector<float>> res = { \r\n");// , outChans * inChans);
    for (size_t i = 0; i < outChans; i++) {
        printf("{ ");
        for (size_t j = 0; j < inChans; j++) {
            printf(" %f", mRes[i][j]);
            if (j != inChans - 1) printf(",", mRes[i][j]);
        }
        printf("}");
        if (i != outChans - 1) printf(",");
        printf("\r\n");
    }
    printf("};\r\n");
    */
}

// TODO: add a helper function to transpose any matrix
// Mach1TranscodeMatrix Mach1TranscodeCore::transposeMatrixConversion(Mach1TranscodeMatrix &matrix)
// {
// }

void Mach1TranscodeCore::processConversion(float **inBufs, float **outBufs, int numSamples) {
    // reinit internal buffer
    if (numSamples > bufferSize) {
        bufferSize = numSamples;

        for (int i = 0; i < Mach1TranscodeConstants::MAXCHANS; i++) {
            delete[] buffers[i];
        }

        for (int i = 0; i < Mach1TranscodeConstants::MAXCHANS; i++) {
            buffers[i] = new float[bufferSize];
        }
    }

    // copy input
    int nInChannels = getInputNumChannels();
    for (int i = 0; i < nInChannels; i++) {
        std::memcpy(buffers[i], inBufs[i], (int)numSamples * sizeof(float));
    }

    // process input sub channels
    for (int k = 0; k < lpFilters.size(); k++) {
        for (int j = 0; j < numSamples; j++) {
            buffers[subChannelIndices[k]][j] = lpFilters[k].Run(buffers[subChannelIndices[k]][j]);
        }
    }

    for (int k = 0; k < formatConversionPath.size() - 1; k++) {
        processConversion(formatConversionPath[k], k == 0 ? inBufs : buffers, formatConversionPath[k + 1], buffers, (int)numSamples);
    }

    int nOutChannels = getOutputNumChannels();
    for (int i = 0; i < nOutChannels; i++) {
        std::memcpy(outBufs[i], buffers[i], (int)numSamples * sizeof(float));
    }

    // spatial downmix check
    if (outFmt == getFormatFromString("M1Spatial") || outFmt == getFormatFromString("M1Spatial-8")) {
        spatialDownmixChecker.ProcessBuffer(outBufs, numSamples);
    }
}

std::vector<int> &Mach1TranscodeCore::getFormatConversionPath() {
    return formatConversionPath;
}
