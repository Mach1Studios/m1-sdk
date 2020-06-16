//  Mach1 Spatial SDK
//  Copyright © 2017-2020 Mach1. All rights reserved.

#include "Mach1TranscodeCore.h"
#include "Mach1GenerateCoeffs.h"
#include "nlohmann/json.hpp"

#define _USE_MATH_DEFINES
#include <math.h>

Mach1TranscodeCore::Mach1TranscodeCore()
{
	inFmt = Mach1TranscodeFormats::FormatType::Empty;
	outFmt = Mach1TranscodeFormats::FormatType::Empty;

	bufferSize = 4096;
	for (int i = 0; i < Mach1TranscodeConstants::MAXCHANS; i++)  {
		buffers[i] = new float[bufferSize];
	}
}

Mach1TranscodeCore::~Mach1TranscodeCore()
{
	for (int i = 0; i < Mach1TranscodeConstants::MAXCHANS; i++) {
		delete[] buffers[i];
	}
}

int Mach1TranscodeCore::getInputNumChannels()
{
	return getNumChannels(inFmt, true);
}

int Mach1TranscodeCore::getOutputNumChannels()
{
    return getNumChannels(outFmt, false);
}

Mach1TranscodeFormats::FormatType Mach1TranscodeCore::getFormatFromString(char * str) {
	for (auto it = Mach1TranscodeConstants::FormatNames.begin(); it != Mach1TranscodeConstants::FormatNames.end(); ++it) {
		if (strcmp(str, it->second) == 0) {
			return it->first;
		}
	}
	return Mach1TranscodeFormats::Empty;
}

char * Mach1TranscodeCore::getFormatName(void * M1obj, Mach1TranscodeFormats::FormatType fmt)
{
	return Mach1TranscodeConstants::FormatNames.at(fmt);
}

float Mach1TranscodeCore::calcNormalization(float** bufs, int numSamples) {
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

void Mach1TranscodeCore::applyMasterGain(float** bufs, int numSamples, float masterGain) {
	if (masterGain == 1.0) return;
	if (outFmt == Mach1TranscodeFormats::FormatType::Empty) return;

	int nChannels = getOutputNumChannels();
	for (int i = 0; i < nChannels; i++) {
		for (int sample = 0; sample < numSamples; sample++) {
			bufs[i][sample] *= masterGain;
		}
	}
}

void Mach1TranscodeCore::setLFESub(std::vector<int> subChannelIndices, int sampleRate)
{
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

void Mach1TranscodeCore::setInputFormat(Mach1TranscodeFormats::FormatType inFmt)
{
	this->inFmt = inFmt;
}

void Mach1TranscodeCore::setInputFormatADM(char * inXml)
{
    // TODO
}

std::vector<Mach1Point3DCore> parseTTJson(char* srtJson)
{
	std::vector<Mach1Point3DCore> points;
	nlohmann::json j = nlohmann::json::parse(srtJson);
	nlohmann::json o = j["points"];
	for (nlohmann::json::iterator it = o.begin(); it != o.end(); ++it) {
		nlohmann::json p = it.value();
		Mach1Point3DCore point(p["x"], p["y"], p["z"]);
		points.push_back(point);
	}
	return points;
}

void Mach1TranscodeCore::setInputFormatTTJson(char * strJson)
{
	inFmt = Mach1TranscodeFormats::FormatType::TTPoints;
	inTTPoints = parseTTJson(strJson);
}

void Mach1TranscodeCore::setInputFormatTTPoints(std::vector<Mach1Point3DCore> points)
{
    inFmt = Mach1TranscodeFormats::FormatType::TTPoints;
    inTTPoints = points;
}

void Mach1TranscodeCore::setOutputFormat(Mach1TranscodeFormats::FormatType outFmt)
{
	this->outFmt = outFmt;
}

void Mach1TranscodeCore::setOutputFormatTTJson(char* strJson)
{
	outFmt = Mach1TranscodeFormats::FormatType::TTPoints;
	outTTPoints = parseTTJson(strJson);
}

void Mach1TranscodeCore::setOutputFormatTTPoints(std::vector<Mach1Point3DCore> points)
{
    outFmt = Mach1TranscodeFormats::FormatType::TTPoints;
    outTTPoints = points;
}

bool Mach1TranscodeCore::computeConvertionPath()
{
    // compute tree of pathes from inFmt to all others
    struct Node {
		int prev;
		Mach1TranscodeFormats::FormatType fmt;
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
			Mach1TranscodeFormats::FormatType fmt = tree[i].fmt;
            for (int j = 0; j < Mach1TranscodeConstants::NUMFMTS; j++) {

                if (Mach1TranscodeConstants::FormatMatrix.find(std::make_pair(fmt, (Mach1TranscodeFormats::FormatType)j)) != Mach1TranscodeConstants::FormatMatrix.end()) {
                    
                    // check if this format already exist on that path
                    int k = i;
                    bool exist = false;
                    while (!exist && k != -1) {
                        if (tree[k].fmt == j) {
                            exist = true;
                            break;
                        }
                        else {
                            k = tree[k].prev;
                        }
                    }
                    
                    if (exist == false) {
                        Node node;
                        node.fmt = (Mach1TranscodeFormats::FormatType)j;
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
    std::vector<Mach1TranscodeFormats::FormatType> intermediateFormatsList;
    intermediateFormatsList.reserve(100);
    for (int i = 0; i < tree.size(); i++) {
        if (tree[i].fmt == outFmt) {
            for (int k = i; k != -1; k = tree[k].prev) {
                intermediateFormatsList.insert(intermediateFormatsList.begin(), tree[k].fmt); // push_front
            }
            break;
        }
    }
    
	if (intermediateFormatsList.size() < 2) {
		formatsConvertionPath.clear();
		return false;
	}
	else {
		formatsConvertionPath = intermediateFormatsList;
		return true;
	}
}

std::vector<Mach1Point3DCore> getPointsSet(Mach1TranscodeFormats::FormatType fmt) {
	// M1 horizon plane points
	static std::vector<Mach1Point3DCore> m1Horizon = { {-1, 0, 1},
												{1, 0, 1},
												{-1, 0, -1},
												{1, 0, -1} };

	// M1 spatial cube points
	static std::vector<Mach1Point3DCore> m1Spatial = { {-1, 1, 1},
												{1, 1, 1},
												{-1, 1, -1},
												{1, 1, -1},

												{-1, -1, 1},
												{1, -1, 1},
												{-1, -1, -1},
												{1, -1, -1} };

	// M1 spatial+ cube points
	static std::vector<Mach1Point3DCore> m1SpatialPlus = { {-1, 1, 1},
												{1, 1, 1},
												{-1, 1, -1},
												{1, 1, -1},

												{-1, -1, 1},
												{1, -1, 1},
												{-1, -1, -1},
												{1, -1, -1},

												{0, 0, 1 / 0.707},
												{1 / 0.707, 0, 0},
												{0, 0, -1 / 0.707},
												{-1 / 0.707, 0, 0} };

	// M1 spatial++ cube points
	static std::vector<Mach1Point3DCore> m1SpatialPlusPlus = { {-1, 1, 1},
												{1, 1, 1},
												{-1, 1, -1},
												{1, 1, -1},

												{-1, -1, 1},
												{1, -1, 1},
												{-1, -1, -1},
												{1, -1, -1},

												{0, 0, 1 / 0.707},
												{1 / 0.707, 0, 0},
												{0, 0, -1 / 0.707},
												{-1 / 0.707, 0, 0},

												{ 1 / 0.707, 0, 0},
												{-1 / 0.707, 0, 0} };

	// M1 spatial extended cube points
	static std::vector<Mach1Point3DCore> m1SpatialExtended = { {-1, 1, 1},
												{1, 1, 1},
												{-1, 1, -1},
												{1, 1, -1},

												{-1, -1, 1},
												{1, -1, 1},
												{-1, -1, -1},
												{1, -1, -1},

												{0, 1, 1 / 0.707},
												{1 / 0.707, 1, 0},
												{0, 1, -1 / 0.707},
												{-1 / 0.707, 1, 0},

												{0, -1, 1 / 0.707},
												{1 / 0.707, -1, 0},
												{0, -1, -1 / 0.707},
												{-1 / 0.707, -1, 0} };

	// M1 spatial extended+ cube points
	static std::vector<Mach1Point3DCore> m1SpatialExtendedPlus = { {-1, 1, 1},
												{1, 1, 1},
												{-1, 1, -1},
												{1, 1, -1},

												{-1, -1, 1},
												{1, -1, 1},
												{-1, -1, -1},
												{1, -1, -1},

												{0, 1, 1 / 0.707},
												{1 / 0.707, 1, 0},
												{0, 1, -1 / 0.707},
												{-1 / 0.707, 1, 0},

												{0, -1, 1 / 0.707},
												{1 / 0.707, -1, 0},
												{0, -1, -1 / 0.707},
												{-1 / 0.707, -1, 0},

												{1 / 0.707, 0, 0},
												{-1 / 0.707, 0, 0} };


	static std::map<Mach1TranscodeFormats::FormatType, std::vector<Mach1Point3DCore>> standards = {
		{Mach1TranscodeFormats::FormatType::M1Horizon, m1Horizon},
		{Mach1TranscodeFormats::FormatType::M1Spatial, m1Spatial},
		{Mach1TranscodeFormats::FormatType::M1SpatialPlus, m1SpatialPlus},
		{Mach1TranscodeFormats::FormatType::M1SpatialPlusPlus, m1SpatialPlusPlus},
		{Mach1TranscodeFormats::FormatType::M1SpatialExtended, m1SpatialExtended},
		{Mach1TranscodeFormats::FormatType::M1SpatialExtendedPlus, m1SpatialExtendedPlus},
	};

	std::vector<Mach1Point3DCore> vec;
	if (standards.find(fmt) != standards.end()) {
		vec = standards[fmt];
	}
	return vec;
}

void Mach1TranscodeCore::convert(Mach1TranscodeFormats::FormatType inFmt, float** inBufs, Mach1TranscodeFormats::FormatType outFmt, float** outBufs, int numSamples)
{
    int inChans = getNumChannels(inFmt, true);
    int outChans = getNumChannels(outFmt, false);
    std::vector<std::vector<float>> currentFormatConversionMatrix;

    if (inFmt == Mach1TranscodeFormats::FormatType::TTPoints && outFmt != Mach1TranscodeFormats::FormatType::TTPoints) {
        currentFormatConversionMatrix = generateCoeffSetForPoints(inTTPoints, getPointsSet(outFmt));
    }
    else if (inFmt != Mach1TranscodeFormats::FormatType::TTPoints && outFmt == Mach1TranscodeFormats::FormatType::TTPoints) {
		currentFormatConversionMatrix = generateCoeffSetForPoints(getPointsSet(outFmt), inTTPoints);
    }
    else if (inFmt != Mach1TranscodeFormats::FormatType::TTPoints && outFmt != Mach1TranscodeFormats::FormatType::TTPoints) {
        currentFormatConversionMatrix = ((SpatialSoundMatrix*)Mach1TranscodeConstants::FormatMatrix.at(std::make_pair(inFmt, outFmt)))->getData();
    }
    
    // copy pointers to local
    float* inPtrs[Mach1TranscodeConstants::MAXCHANS];
    if (inBufs != NULL) {
        for (int i = 0; i<inChans; i++)
            inPtrs[i] = inBufs[i];
    }
    float* outPtrs[Mach1TranscodeConstants::MAXCHANS];
    if (outBufs != NULL) {
        for (int i = 0; i<outChans; i++)
            outPtrs[i] = outBufs[i];
    }
    
    // matrix mult each sample
    float ins[Mach1TranscodeConstants::MAXCHANS];
    for (int sample = 0; sample < numSamples; sample++) {
        for (int inChannel = 0; inChannel < inChans; inChannel++)
            ins[inChannel] = inPtrs[inChannel][sample];
        for (int channel = inChans; channel < Mach1TranscodeConstants::MAXCHANS; channel++)
            ins[channel] = 0;
        
        for (int outChannel = 0; outChannel < outChans; outChannel++)
        {
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

int Mach1TranscodeCore::getNumChannels(Mach1TranscodeFormats::FormatType fmt, bool isInput)
{
	if (fmt == Mach1TranscodeFormats::FormatType::TTPoints) {
		return isInput ? inTTPoints.size() : outTTPoints.size();
	}
    return Mach1TranscodeConstants::FormatChannels.at(fmt);
}

float Mach1TranscodeCore::db2level(float db)
{
	return (float)exp(M_LN10 * ((double)db / 20.0));
}

float Mach1TranscodeCore::level2db(float level)
{
	return (float)(20.0f * log((double)level) / M_LN10);
}

void Mach1TranscodeCore::getMatrixConversion(float* matrix)
{
	float mPrev[Mach1TranscodeConstants::MAXCHANS][Mach1TranscodeConstants::MAXCHANS];
	float mCurrent[Mach1TranscodeConstants::MAXCHANS][Mach1TranscodeConstants::MAXCHANS];
	float mRes[Mach1TranscodeConstants::MAXCHANS][Mach1TranscodeConstants::MAXCHANS];
	int mSize = Mach1TranscodeConstants::MAXCHANS * Mach1TranscodeConstants::MAXCHANS * sizeof(float);

	int prevInChans = 0;
	int prevOutChans = 0;

	memset(mPrev, 0, mSize);
	memset(mCurrent, 0, mSize);
	memset(mRes, 0, mSize);

	for (int k = 0; k < formatsConvertionPath.size() - 1; k++) {
		Mach1TranscodeFormats::FormatType inFmt = formatsConvertionPath[k];
		Mach1TranscodeFormats::FormatType outFmt = formatsConvertionPath[k + 1];

		int inChans = getNumChannels(inFmt, true);
		int outChans = getNumChannels(outFmt, false);
		std::vector<std::vector<float>> currentFormatConversionMatrix;

		if (inFmt == Mach1TranscodeFormats::FormatType::TTPoints && outFmt != Mach1TranscodeFormats::FormatType::TTPoints) {
			currentFormatConversionMatrix = generateCoeffSetForPoints(inTTPoints, getPointsSet(outFmt));
		}
		else if (inFmt != Mach1TranscodeFormats::FormatType::TTPoints && outFmt == Mach1TranscodeFormats::FormatType::TTPoints) {
			currentFormatConversionMatrix = generateCoeffSetForPoints(getPointsSet(outFmt), inTTPoints);
		}
		else if (inFmt != Mach1TranscodeFormats::FormatType::TTPoints && outFmt != Mach1TranscodeFormats::FormatType::TTPoints) {
			currentFormatConversionMatrix = ((SpatialSoundMatrix*)Mach1TranscodeConstants::FormatMatrix.at(std::make_pair(inFmt, outFmt)))->getData();
		}

		memset(mCurrent, 0, mSize);
		for (int outChannel = 0; outChannel < outChans; outChannel++)
		{
			for (int inChannel = 0; inChannel < inChans; inChannel++) {
				mCurrent[outChannel][inChannel] = currentFormatConversionMatrix[outChannel][inChannel];
			}
		}

		if (k == 0) {
			memcpy(mRes, mCurrent, mSize);
		}
		else {
			memset(mRes, 0, mSize);
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
		memcpy(mPrev, mRes, Mach1TranscodeConstants::MAXCHANS * Mach1TranscodeConstants::MAXCHANS * sizeof(float));
	}

	int inChans = getInputNumChannels();
	int outChans = getOutputNumChannels();
	for (size_t i = 0; i < outChans; i++) {
		for (size_t j = 0; j < inChans; j++) {
			matrix[i *  inChans + j] = mRes[i][j];
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

void Mach1TranscodeCore::convert(float ** inBufs, float ** outBufs, int numSamples)
{
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
		memcpy(buffers[i], inBufs[i], (int)numSamples * sizeof(float));
	}

	// process input sub channels
	for (int k = 0; k < lpFilters.size(); k++) {
		for (int j = 0; j < numSamples; j++) {
			buffers[subChannelIndices[k]][j] = lpFilters[k].Run(buffers[subChannelIndices[k]][j]);
		}
	}

	for (int k = 0; k < formatsConvertionPath.size() - 1; k++) {
		convert(formatsConvertionPath[k], k == 0 ? inBufs : buffers, formatsConvertionPath[k + 1], buffers, (int)numSamples);
	}

	int nOutChannels = getOutputNumChannels();
	for (int i = 0; i < nOutChannels; i++) {
		memcpy(outBufs[i], buffers[i], (int)numSamples * sizeof(float));
	}

	// spatial downmix check
	if (outFmt == Mach1TranscodeFormats::FormatType::M1Spatial) {
		spatialDownmixChecker.ProcessBuffer(outBufs, numSamples);
	}
}

std::vector<Mach1TranscodeFormats::FormatType>& Mach1TranscodeCore::getFormatsConvertionPath() {
	return formatsConvertionPath;
}
