//  Mach1 Spatial SDK
//  Copyright © 2017-2022 Mach1. All rights reserved.

#ifdef M1TRANSCODE_INLINE_ENCODE
#    include "Mach1EncodeCore.h"
#endif
#include "AmbisonicFormats.h"
#include "Mach1GenerateCoeffs.h"
#include "Mach1TranscodeCore.h"
#include "MicArrayFormats.h"
#include "SurroundFormats.h"
#include "VectorFormats.h"
#include "json/json.h"
#include <cstring>
#include <string>
#include <queue>
#include <unordered_map>
#include <unordered_set>

#define _USE_MATH_DEFINES
#include <cmath>

#ifndef M_LN10
#    define M_LN10 2.30258509299404568402
#endif

// TODO: remove this and use a single instance of clamp in all apis
float clamp_local(float n, float lower, float upper) {
    return (std::max)(lower, (std::min)(n, upper));
}

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

int Mach1TranscodeCore::getInputFormat() {
    return inFmt;
}

int Mach1TranscodeCore::getOutputFormat() {
    return outFmt;
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
    return nullptr;
}

const char **Mach1TranscodeCore::getAllFormatNames() {
    static std::vector<char *> formatNames;

    if (formatNames.size() == 0) {
        for (auto it = Mach1TranscodeConstants::formats.begin(); it != Mach1TranscodeConstants::formats.end(); ++it) {
            formatNames.push_back((char *)it->name.data());
        }
        formatNames.push_back(nullptr);
    }
    numFormats = formatNames.size();
    return (const char **)formatNames.data();
}

int Mach1TranscodeCore::getFormatsCount() {
    return numFormats;
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
    // Assert that the format is not out of range (e.g., not -1)
    assert(inFmt >= 0 && inFmt < Mach1TranscodeConstants::formats.size());
    this->inFmt = inFmt;
}

std::vector<Mach1Point3D> parseCustomPointsJson(std::string srtJson) {
    std::vector<Mach1Point3D> points;

    auto doc = JSON::parse(srtJson);
    if (doc.size() > 0) {
        auto jsonPoints = JSON::getChildren(doc, "points");
        if (jsonPoints.size() > 0) {
            for (int i = 0;; i++) {
                auto jsonPoint = JSON::getElement(jsonPoints, 3, i);
                if (jsonPoint.size() > 0) {
                    /// TEST FOR SPHERICAL / POLAR DEFINITIONS
                    auto usePolar = JSON::getChildren(jsonPoint, "usePolar")[0]->value;
                    if ((usePolar.find("1") != std::string::npos || usePolar.find("true") != std::string::npos || usePolar.find("True") != std::string::npos || usePolar.find("TRUE") != std::string::npos || usePolar.find("yes") != std::string::npos || usePolar.find("YES") != std::string::npos) || JSON::getChildren(jsonPoint, "x").empty()) {
                        float diverge_radius = 0, azimuth = 0, elevation = 0;
                        if (!JSON::getChildren(jsonPoint, "diverge").empty()) {
                            diverge_radius = std::stof(JSON::getChildren(jsonPoint, "diverge")[0]->value);
                            if (diverge_radius > 1.0)
                                diverge_radius = 1.0;
                            if (diverge_radius < -1.0)
                                diverge_radius = -1.0;
                        }
                        if (!JSON::getChildren(jsonPoint, "azimuth").empty()) {
                            float azimuth_degrees = (std::stof(JSON::getChildren(jsonPoint, "azimuth")[0]->value));
                            // convert the -180 -> 180 degree range to 0 -> 1
                            azimuth_degrees = std::fmod(azimuth_degrees, 360.0); // protect a 360 cycle
                            if (azimuth_degrees < 0) {                           // check if -180 to 180, convert to 0-360
                                azimuth_degrees += 360.0;
                            }
                            azimuth = azimuth_degrees / 360.0;
                        }
                        if (!JSON::getChildren(jsonPoint, "elevation").empty()) {
                            // convert the -90 -> 90 degree range to 0 -> 1
                            float elevation_degrees = (std::stof(JSON::getChildren(jsonPoint, "elevation")[0]->value));
                            elevation_degrees = clamp_local(elevation_degrees, -90, 90);
                            elevation = elevation_degrees / 90;
                        }

                        float normalisedOutputDiverge = diverge_radius * (1 / std::cos(PI * 0.25f)); // normalize for entire cube space and not just spherical space

                        points.push_back({
                            // process normalized polar inputs as cartesian normalized for the vector space
                            std::sin((azimuth)*PI * 2) * normalisedOutputDiverge * std::sin((-elevation + 1) * PI / 2),
                            std::cos((azimuth)*PI * 2) * normalisedOutputDiverge * std::sin((-elevation + 1) * PI / 2),
                            std::cos((-elevation + 1) * PI / 2) * normalisedOutputDiverge,
                        });

                    } else {
                        /// TEST FOR CARTESIAN DEFINITIONS
                        if (!JSON::getChildren(jsonPoint, "x").empty() && !JSON::getChildren(jsonPoint, "y").empty() && !JSON::getChildren(jsonPoint, "z").empty()) {
                            points.push_back({std::stof(JSON::getChildren(jsonPoint, "x")[0]->value),
                                              std::stof(JSON::getChildren(jsonPoint, "y")[0]->value),
                                              std::stof(JSON::getChildren(jsonPoint, "z")[0]->value)});
                        }
                    }
                } else {
                    break;
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

void Mach1TranscodeCore::setInputFormatCustomPoints(std::vector<Mach1Point3D> points) {
    inFmt = getFormatFromString("CustomPoints");
    inCustomPoints = points;
}

void Mach1TranscodeCore::setOutputFormat(int outFmt) {
    // Assert that the format is not out of range (e.g., not -1)
    assert(outFmt >= 0 && outFmt < Mach1TranscodeConstants::formats.size());
    this->outFmt = outFmt;
}

void Mach1TranscodeCore::setOutputFormatCustomPointsJson(std::string strJson) {
    outFmt = getFormatFromString("CustomPoints");
    outCustomPoints = parseCustomPointsJson(strJson);
}

void Mach1TranscodeCore::setOutputFormatCustomPoints(std::vector<Mach1Point3D> points) {
    outFmt = getFormatFromString("CustomPoints");
    outCustomPoints = points;
}

void Mach1TranscodeCore::setCustomPointsSamplerCallback(Mach1Point3D *(*callback)(long long, int &)) {
    customPointsSamplerCallback = callback;
}

bool Mach1TranscodeCore::processConversionPath() {
    // return for identical formats
    if (inFmt == outFmt) {
        formatConversionPath = {inFmt};
        return true;
    }

    // direct connection check
    bool directConnection = findMatrix(inFmt, outFmt) >= 0 ||
        (inFmt == getFormatFromString("CustomPoints") && outFmt == getFormatFromString("CustomPoints")) ||
        (inFmt == getFormatFromString("CustomPoints") && getPointsSet(outFmt).size() > 0) ||
        (getPointsSet(inFmt).size() > 0 && outFmt == getFormatFromString("CustomPoints"));
    
    if (directConnection) {
        formatConversionPath = {inFmt, outFmt};
        return true;
    }

    // get the total number of formats
    const int maxFormats = Mach1TranscodeConstants::formats.size();
    
    // build adjacency lists for forward and backward connections
    std::vector<std::vector<int>> forwardAdjList(maxFormats);
    std::vector<std::vector<int>> backwardAdjList(maxFormats);
    
    // populate adjacency lists (precompute once)
    for (int i = 0; i < maxFormats; i++) {
        for (int j = 0; j < maxFormats; j++) {
            bool isCompatible = 
                findMatrix(i, j) >= 0 ||
                (i == getFormatFromString("CustomPoints") && j == getFormatFromString("CustomPoints")) ||
                (i == getFormatFromString("CustomPoints") && getPointsSet(j).size() > 0) ||
                (getPointsSet(i).size() > 0 && j == getFormatFromString("CustomPoints"));
            
            if (isCompatible) {
                forwardAdjList[i].push_back(j);
                backwardAdjList[j].push_back(i);
            }
        }
    }
    
    // bidirectional BFS
    std::vector<bool> forwardVisited(maxFormats, false);
    std::vector<bool> backwardVisited(maxFormats, false);
    std::vector<int> forwardParent(maxFormats, -1);
    std::vector<int> backwardParent(maxFormats, -1);
    
    std::deque<int> forwardQueue, backwardQueue;
    
    forwardQueue.push_back(inFmt);
    backwardQueue.push_back(outFmt);
    forwardVisited[inFmt] = true;
    backwardVisited[outFmt] = true;
    
    int meetingPoint = -1;
    
    while (!forwardQueue.empty() && !backwardQueue.empty() && meetingPoint == -1) {
        // process one level of forward search
        int forwardSize = forwardQueue.size();
        for (int i = 0; i < forwardSize; i++) {
            int currentFmt = forwardQueue.front();
            forwardQueue.pop_front();
            
            // iterate only through adjacent formats (not all formats)
            for (int nextFmt : forwardAdjList[currentFmt]) {
                if (!forwardVisited[nextFmt]) {
                    forwardVisited[nextFmt] = true;
                    forwardParent[nextFmt] = currentFmt;
                    
                    // check if we've met the backward search
                    if (backwardVisited[nextFmt]) {
                        meetingPoint = nextFmt;
                        break;
                    }
                    
                    forwardQueue.push_back(nextFmt);
                }
            }
            
            if (meetingPoint != -1) break;
        }
        
        if (meetingPoint != -1) break;
        
        // process one level of backward search
        int backwardSize = backwardQueue.size();
        for (int i = 0; i < backwardSize; i++) {
            int currentFmt = backwardQueue.front();
            backwardQueue.pop_front();
            
            // iterate only through adjacent formats
            for (int prevFmt : backwardAdjList[currentFmt]) {
                if (!backwardVisited[prevFmt]) {
                    backwardVisited[prevFmt] = true;
                    backwardParent[prevFmt] = currentFmt;
                    
                    // check if we've met the forward search
                    if (forwardVisited[prevFmt]) {
                        meetingPoint = prevFmt;
                        break;
                    }
                    
                    backwardQueue.push_back(prevFmt);
                }
            }
            
            if (meetingPoint != -1) break;
        }
    }
    
    // reconstruct path if a meeting point was found
    if (meetingPoint != -1) {
        formatConversionPath.clear();
        
        // build forward path
        for (int fmt = meetingPoint; fmt != -1; fmt = forwardParent[fmt]) {
            formatConversionPath.insert(formatConversionPath.begin(), fmt);
        }
        
        // build backward path (excluding meeting point which is already included)
        for (int fmt = backwardParent[meetingPoint]; fmt != -1; fmt = backwardParent[fmt]) {
            formatConversionPath.push_back(fmt);
        }
        
        return true;
    }
    
    // no path found
    formatConversionPath.clear();
    return false;
}


std::vector<Mach1Point3D> Mach1TranscodeCore::getPointsSet(int fmt) {
    /*
     * X = Left(-) to Right(+) (from a top down perspective)
     * Y = Front(+) to Back(-) (from a top down perspective)
     * Z = Top(+) to Bottom(-) (from a top down perspective)
     */

    static std::vector<Mach1Point3D> m1Spatial_4_Def = {
        {-1, 1, 0},
        {1, 1, 0},
        {-1, -1, 0},
        {1, -1, 0}};

    static std::vector<Mach1Point3D> m1Spatial_8_Def = {
        {-1, 1, 1},
        {1, 1, 1},
        {-1, -1, 1},
        {1, -1, 1},

        {-1, 1, -1},
        {1, 1, -1},
        {-1, -1, -1},
        {1, -1, -1}};

    static std::vector<Mach1Point3D> m1Spatial_12_Def = {
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

    static std::vector<Mach1Point3D> m1Spatial_14_Def = {
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

    static std::map<int, std::vector<Mach1Point3D> > standards = {
        {getFormatFromString("M1Horizon"), m1Spatial_4_Def},
        {getFormatFromString("M1Spatial-4"), m1Spatial_4_Def},
        {getFormatFromString("M1Spatial"), m1Spatial_8_Def},
        {getFormatFromString("M1Spatial-8"), m1Spatial_8_Def},
        {getFormatFromString("M1Spatial-12"), m1Spatial_12_Def},
        {getFormatFromString("M1Spatial-14"), m1Spatial_14_Def},
    };

    std::vector<Mach1Point3D> vec;
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

std::vector<std::vector<float> > Mach1TranscodeCore::getCoeffs(int idxMatrix) {
    std::vector<std::vector<float> > coeffs;

    if (idxMatrix >= 0) {
        for (int i = 0; i < matrices[idxMatrix].channels.size(); i++) {
            Mach1TranscodeChannelBase *channel = matrices[idxMatrix].channels[i];
            if (Mach1TranscodeCoeffs *c = dynamic_cast<Mach1TranscodeCoeffs *>(channel)) {
                coeffs.push_back(c->data);
            }
#ifdef M1TRANSCODE_INLINE_ENCODE
            else if (Mach1TranscodePanner *p = dynamic_cast<Mach1TranscodePanner *>(channel)) {
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
                std::vector<std::vector<float> > gains = m1encode.resultingPoints.getGains();
                coeffs.push_back(gains[i]);
            }
#endif
        }
    }

    std::vector<std::vector<float> > coeffsFlipped(coeffs[0].size(), std::vector<float>(coeffs.size()));
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
    std::vector<std::vector<float> > currentFormatConversionMatrix;

    // TODO: if `inFmt` or `outFmt` is not a m1spatial format return an informative error
    if (inFmt == getFormatFromString("CustomPoints") && outFmt != getFormatFromString("CustomPoints")) {
        // custom points input
        currentFormatConversionMatrix = generateCoeffSetForPoints(inCustomPoints, getPointsSet(outFmt));
    } else if (inFmt != getFormatFromString("CustomPoints") && outFmt == getFormatFromString("CustomPoints")) {
        // custom points output
        currentFormatConversionMatrix = generateCoeffSetForPoints(getPointsSet(inFmt), outCustomPoints);
    } else if (inFmt == getFormatFromString("CustomPoints") && outFmt == getFormatFromString("CustomPoints")) {
        // custom points input and output
        currentFormatConversionMatrix = generateCoeffSetForPoints(inCustomPoints, outCustomPoints);
    } else if (inFmt != getFormatFromString("CustomPoints") && outFmt != getFormatFromString("CustomPoints")) {
        // predefined recipes only
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
                inCustomPoints.push_back(*(Mach1Point3D *)&points[i]);
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

    // find a path if it is empty
    if (formatConversionPath.size() == 0) {
        processConversionPath();
    }

    for (int k = 0; k < formatConversionPath.size() - 1; k++) {
        int inFmt = formatConversionPath[k];
        int outFmt = formatConversionPath[k + 1];

        int inChans = getNumChannels(inFmt, true);
        int outChans = getNumChannels(outFmt, false);
        std::vector<std::vector<float> > currentFormatConversionMatrix;

        // TODO: if `inFmt` or `outFmt` is not a m1spatial format return an informative error
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
    // find a path if it is empty
    if (formatConversionPath.size() == 0) {
        processConversionPath();
    }

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
