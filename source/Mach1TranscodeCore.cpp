//  Mach1 Spatial SDK
//  Copyright © 2017-2020 Mach1. All rights reserved.

#include "Mach1TranscodeCore.h"
#include "Mach1GenerateCoeffs.h"
#include "Mach1EncodeCore.h"
#include "Mach1TranscodeVectorFormats.h"
#include "Mach1TranscodeSurroundFormats.h"
#include "Mach1TranscodeAmbisonicFormats.h"
#include <string.h> 
#include <cstring>
#include "json/json.h"

#define _USE_MATH_DEFINES
#include <cmath>

#ifndef M_LN10
    #define M_LN10 2.30258509299404568402
#endif

Mach1TranscodeCore::Mach1TranscodeCore()
{
	inFmt = -1;
	outFmt = -1;

	bufferSize = 4096;
	for (int i = 0; i < Mach1TranscodeConstants::MAXCHANS; i++)  {
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

int Mach1TranscodeCore::getFormatFromString(const char* str) {
	for (auto it = Mach1TranscodeConstants::formats.begin(); it != Mach1TranscodeConstants::formats.end(); ++it) {
		if (std::strcmp(str, it->name.data()) == 0) {
			return it - Mach1TranscodeConstants::formats.begin();
		}
	}
	return -1;
}

const char* Mach1TranscodeCore::getFormatName(int fmt)
{
	if (fmt < Mach1TranscodeConstants::formats.size()) {
		return Mach1TranscodeConstants::formats[fmt].name.data();
	}
	return "";
}

float Mach1TranscodeCore::processNormalization(float** bufs, int numSamples) {
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

void Mach1TranscodeCore::processMasterGain(float** bufs, int numSamples, float masterGain) {
	if (masterGain == 1.0) return;
	if (outFmt == -1) return;

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

const std::vector<float> Mach1TranscodeCore::getAvgSamplesDiff() {
	return spatialDownmixChecker.getAvgSamplesDiff();
}

void Mach1TranscodeCore::setInputFormat(int inFmt)
{
	this->inFmt = inFmt;
}

std::vector<Mach1Point3DCore> parseCustomPointsJson(std::string srtJson)
{
	std::vector<Mach1Point3DCore> points;

	auto doc = JSON::parse(srtJson);
	if (doc.size() > 0) {
		auto jsonPoints = JSON::getChildren(doc, "points");
		if (jsonPoints.size() > 0) {
			for (int i = 0; ; i++) {
				auto jsonPoint = JSON::getElement(jsonPoints, 3, i);
				if (jsonPoint.size() > 0) {
					points.push_back(
						Mach1Point3DCore(
							std::stof(JSON::getChildren(jsonPoint, "x")[0]->value),
							std::stof(JSON::getChildren(jsonPoint, "y")[0]->value),
							std::stof(JSON::getChildren(jsonPoint, "z")[0]->value)
						)
					);
				}
			}
		}
	}

	for (int i = 0; i < doc.size(); i++) delete doc[i];

	return points;
}

void Mach1TranscodeCore::setInputFormatCustomPointsJson(std::string strJson)
{
	inFmt = getFormatFromString("CustomPoints");
	inCustomPoints = parseCustomPointsJson(strJson);
}

void Mach1TranscodeCore::setInputFormatCustomPoints(std::vector<Mach1Point3DCore> points)
{
    inFmt = getFormatFromString("CustomPoints");
    inCustomPoints = points;
}

void Mach1TranscodeCore::setOutputFormat(int outFmt)
{
	this->outFmt = outFmt;
}

void Mach1TranscodeCore::setOutputFormatCustomPointsJson(std::string strJson)
{
	outFmt = getFormatFromString("CustomPoints");
	outCustomPoints = parseCustomPointsJson(strJson);
}

void Mach1TranscodeCore::setOutputFormatCustomPoints(std::vector<Mach1Point3DCore> points)
{
    outFmt = getFormatFromString("CustomPoints");
    outCustomPoints = points;
}

void Mach1TranscodeCore::setCustomPointsSamplerCallback(Mach1Point3D *(*callback)(long long, int &))
{
	customPointsSamplerCallback = callback;
}

bool Mach1TranscodeCore::processConversionPath()
{
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
                        }
                        else {
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
	}
	else {
		formatConversionPath = intermediateFormatsList;
		return true;
	}
}

std::vector<Mach1Point3DCore> Mach1TranscodeCore::getPointsSet(int fmt) {
	// M1 horizon plane points
	static std::vector<Mach1Point3DCore> m1HorizonDef = { {-1, 1, 0},
												{1, 1, 0},
												{-1, -1, 0},
												{1, -1, 0} };

	// M1 spatial cube points
	static std::vector<Mach1Point3DCore> m1SpatialDef = { {-1, 1, 1},
												{1, 1, 1},
												{-1, -1, 1},
												{1, -1, 1},

												{-1, 1, -1},
												{1, 1, -1},
												{-1, -1, -1},
												{1, -1, -1} };

	// M1 spatial+ cube points
	static std::vector<Mach1Point3DCore> m1SpatialPlusDef = { {-1, 1, 1},
												{1, 1, 1},
												{-1, -1, 1},
												{1, -1, 1},

												{-1, 1, -1},
												{1, 1, -1},
												{-1, -1, -1},
												{1, -1, -1},

												{0, 1 / 0.707f, 0},
												{1 / 0.707f, 0, 0},
												{0, -1 / 0.707f, 0},
												{-1 / 0.707f, 0, 0} };

	// M1 spatial++ cube points
	static std::vector<Mach1Point3DCore> m1SpatialPlusPlusDef = { {-1, 1, 1},
												{1, 1, 1},
												{-1, -1, 1},
												{1, -1, 1},

												{-1, 1, -1},
												{1, 1, -1},
												{-1, -1, -1},
												{1, -1, -1},

												{0, 1 / 0.707f, 0},
												{1 / 0.707f, 0, 0},
												{0, -1 / 0.707f, 0},
												{-1 / 0.707f, 0, 0},

												{ 1 / 0.707f, 0, 0},
												{-1 / 0.707f, 0, 0} };

	// M1 spatial extended cube points
	static std::vector<Mach1Point3DCore> m1SpatialExtendedDef = { {-1, 1, 1},
												{1, 1, 1},
												{-1, -1, 1},
												{1, -1, 1},

												{-1, 1, -1},
												{1, 1, -1},
												{-1, -1, -1},
												{1, -1, -1},

												{0, 1 / 0.707f, 1},
												{1 / 0.707f, 0, 1},
												{0, -1 / 0.707f, 1},
												{-1 / 0.707f, 0, 1},

												{0, 1 / 0.707f, -1},
												{1 / 0.707f, 0, -1},
												{0, -1 / 0.707f, -1},
												{-1 / 0.707f, 0, -1} };

	// M1 spatial extended+ cube points
	static std::vector<Mach1Point3DCore> m1SpatialExtendedPlusDef = { {-1, 1, 1},
												{1, 1, 1},
												{-1, -1, 1},
												{1, -1, 1},

												{-1, 1, -1},
												{1, 1, -1},
												{-1, -1, -1},
												{1, -1, -1},

												{0, 1 / 0.707f, 1},
												{1 / 0.707f, 0, 1},
												{0, -1 / 0.707f, 1},
												{-1 / 0.707f, 0, 1},

												{0, 1 / 0.707f, -1},
												{1 / 0.707f, 0, -1},
												{0, -1 / 0.707f, -1},
												{-1 / 0.707f, 0, -1},

												{1 / 0.707f, 0, 0},
												{-1 / 0.707f, 0, 0} };

	static std::map<int, std::vector <Mach1Point3DCore> > standards = {
		{getFormatFromString("M1Horizon"), m1HorizonDef},
		{getFormatFromString("M1Spatial"), m1SpatialDef},
		{getFormatFromString("M1SpatialPlus"), m1SpatialPlusDef},
		{getFormatFromString("M1SpatialPlusPlus"), m1SpatialPlusPlusDef},
		{getFormatFromString("M1SpatialExtended"), m1SpatialExtendedDef},
		{getFormatFromString("M1SpatialExtendedPlus"), m1SpatialExtendedPlusDef},
	};

	std::vector<Mach1Point3DCore> vec;
	if (standards.find(fmt) != standards.end()) {
		vec = standards[fmt];
	}
	return vec;
}

int Mach1TranscodeCore::findMatrix(int inFmt, int outFmt)
{
	const char* inFmtName = getFormatName(inFmt);
	const char* outFmtName = getFormatName(outFmt);

	for (int i = 0; i < matrices.size(); i++) {
		if (matrices[i].formatFrom == inFmtName && matrices[i].formatTo == outFmtName) {
			return i;
		}
	}
	return -1;
}

std::vector<std::vector<float>> Mach1TranscodeCore::getCoeffs(int idxMatrix)
{
	std::vector<std::vector<float>> coeffs;

	if (idxMatrix >= 0) {
		for (int i = 0; i < matrices[idxMatrix].channels.size(); i++)
		{
			Mach1TranscodeChannelBase* channel = matrices[idxMatrix].channels[i];
			if (Mach1TranscodeCoeffs* c = dynamic_cast<Mach1TranscodeCoeffs*>(channel)) {
				coeffs.push_back(c->data);
			}
			else if (Mach1TranscodePanner* p = dynamic_cast<Mach1TranscodePanner*>(channel)) {
				/* panner logic */
				/*
				M1EncodeCore m1encode;
				m1encode.setInputMode(M1EncodeCore::InputMode::INPUT_STEREO);
				m1encode.setOutputMode(M1EncodeCore::OutputMode::OUTPUT_SPATIAL_8CH);
				*/
				coeffs.push_back({});
			}
			else {
				coeffs.push_back({});
			}
		}
	}

	return coeffs;
}


void Mach1TranscodeCore::processConversion(int inFmt, float** inBufs, int outFmt, float** outBufs, int numSamples)
{
    int inChans = getNumChannels(inFmt, true);
    int outChans = getNumChannels(outFmt, false);
    std::vector< std::vector<float> > currentFormatConversionMatrix;

    if (inFmt == getFormatFromString("CustomPoints") && outFmt != getFormatFromString("CustomPoints")) {
        currentFormatConversionMatrix = generateCoeffSetForPoints(inCustomPoints, getPointsSet(outFmt));
    }
	else if (inFmt != getFormatFromString("CustomPoints") && outFmt == getFormatFromString("CustomPoints")) {
		currentFormatConversionMatrix = generateCoeffSetForPoints(getPointsSet(inFmt), outCustomPoints);
	}
	else if (inFmt == getFormatFromString("CustomPoints") && outFmt == getFormatFromString("CustomPoints")) {
		currentFormatConversionMatrix = generateCoeffSetForPoints(inCustomPoints, outCustomPoints);
	}
	else if (inFmt != getFormatFromString("CustomPoints") && outFmt != getFormatFromString("CustomPoints")) {
        currentFormatConversionMatrix = getCoeffs(findMatrix(inFmt, outFmt));
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


		if(inFmt == getFormatFromString("CustomPoints") && customPointsSamplerCallback != nullptr) {
			// updates the points
			inCustomPoints.clear();

			int cnt = 0;
			Mach1Point3D* points = customPointsSamplerCallback(sample, cnt);
			for (int i = 0; i < cnt; i++) {
				inCustomPoints.push_back(*(Mach1Point3DCore*)&points[i]);
			}

			currentFormatConversionMatrix = generateCoeffSetForPoints(inCustomPoints, outCustomPoints);
		}
		
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

int Mach1TranscodeCore::getNumChannels(int fmt, bool isInput)
{
	if (fmt == getFormatFromString("CustomPoints")) {
		return isInput ? (int)inCustomPoints.size() : (int)outCustomPoints.size();
	}
    return Mach1TranscodeConstants::formats[fmt].numChannels;
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

	std::memset(mPrev, 0, mSize);
	std::memset(mCurrent, 0, mSize);
	std::memset(mRes, 0, mSize);

	for (int k = 0; k < formatConversionPath.size() - 1; k++) {
		int inFmt = formatConversionPath[k];
		int outFmt = formatConversionPath[k + 1];

		int inChans = getNumChannels(inFmt, true);
		int outChans = getNumChannels(outFmt, false);
		std::vector< std::vector<float> > currentFormatConversionMatrix;

		if (inFmt == getFormatFromString("CustomPoints") && outFmt != getFormatFromString("CustomPoints")) {
			currentFormatConversionMatrix = generateCoeffSetForPoints(inCustomPoints, getPointsSet(outFmt));
		}
		else if (inFmt != getFormatFromString("CustomPoints") && outFmt == getFormatFromString("CustomPoints")) {
			currentFormatConversionMatrix = generateCoeffSetForPoints(getPointsSet(inFmt), outCustomPoints);
		}
		else if (inFmt == getFormatFromString("CustomPoints") && outFmt == getFormatFromString("CustomPoints")) {
			currentFormatConversionMatrix = generateCoeffSetForPoints(inCustomPoints, outCustomPoints);
		}
		else if (inFmt != getFormatFromString("CustomPoints") && outFmt != getFormatFromString("CustomPoints")) {
			currentFormatConversionMatrix = getCoeffs(findMatrix(inFmt, outFmt));
		}

		std::memset(mCurrent, 0, mSize);
		for (int outChannel = 0; outChannel < outChans; outChannel++)
		{
			for (int inChannel = 0; inChannel < inChans; inChannel++) {
				mCurrent[outChannel][inChannel] = currentFormatConversionMatrix[outChannel][inChannel];
			}
		}

		if (k == 0) {
			std::memcpy(mRes, mCurrent, mSize);
		}
		else {
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
			matrix[i *  inChans + j] = mRes[i][j];
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

void Mach1TranscodeCore::processConversion(float ** inBufs, float ** outBufs, int numSamples)
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
	if (outFmt == getFormatFromString("M1Spatial")) {
		spatialDownmixChecker.ProcessBuffer(outBufs, numSamples);
	}
}

std::vector<int>& Mach1TranscodeCore::getFormatConversionPath() {
	return formatConversionPath;
}
