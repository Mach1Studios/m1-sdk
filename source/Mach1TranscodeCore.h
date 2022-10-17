//  Mach1 Spatial SDK
//  Copyright © 2017-2022 Mach1. All rights reserved.
//
// References: 
// - York ambisonics: https://www.york.ac.uk/sadie-project/ambidec.html

#pragma once

#ifndef Mach1TranscodeCore_h
#define Mach1TranscodeCore_h

#include <math.h>
#include <stddef.h>
#include <vector>
#include <map>
#include "M1DSP/M1DSPFilters.h"
#include "M1DSP/M1DSPUtilities.h"
#include "Mach1Point3DCore.h"
#include "Mach1Point3D.h"

#include "Mach1TranscodeConstants.h"

struct ProcessSettings {
	bool processObjectBed = true;
	bool processChannelBed = true;
	bool enableBinauralRendering = true;
};

class Mach1TranscodeCore
{
private:
	int inFmt;
	int outFmt;

	std::vector<Mach1TranscodeMatrix> matrices;

	std::vector<Mach1Point3DCore> inCustomPoints;
	std::vector<Mach1Point3DCore> outCustomPoints;

	float *buffers[Mach1TranscodeConstants::MAXCHANS];
	int bufferSize;

	std::vector<int> formatConversionPath;

	M1DSP::Utilities::CSpatialDownmixChecker spatialDownmixChecker;

	std::vector<M1DSP::Filters::CFilterSimpleLP> lpFilters;
	std::vector<int> subChannelIndices;

	std::vector<Mach1Point3DCore> getPointsSet(int fmt);
	int findMatrix(int inFmt, int outFmt);
	std::vector<std::vector<float>> getCoeffs(int idxMatrix);
	void processConversion(int inFmt, float** inBufs, int outFmt, float** outBufs, int numSamples);
	int getNumChannels(int fmt, bool isInput);

	Mach1Point3D* (*customPointsSamplerCallback) (long long, int&) = nullptr;

public:

	Mach1TranscodeCore();
	~Mach1TranscodeCore();

	int getInputNumChannels();
	int getOutputNumChannels();
	int getFormatFromString(const char* str);
	const char* getFormatName(int fmt);

	float processNormalization(float** bufs, int numSamples);
	void processMasterGain(float** bufs, int numSamples, float masterGain = 1.0f);

	float db2level(float db);
	float level2db(float level);

	void setLFESub(std::vector<int> subChannelIndices, int sampleRate);
	void setSpatialDownmixer(float corrThreshold = 0.1);
	bool getSpatialDownmixerPossibility();
	const std::vector<float> getAvgSamplesDiff();

	void setInputFormat(int inFmt);
	void setInputFormatCustomPointsJson(std::string inJson);
	void setInputFormatCustomPoints(std::vector<Mach1Point3DCore> points);

	void setOutputFormat(int outFmt);
	void setOutputFormatCustomPointsJson(std::string outJson);
	void setOutputFormatCustomPoints(std::vector<Mach1Point3DCore> points);

	void setCustomPointsSamplerCallback(Mach1Point3D* (*callback) (long long, int&));

	bool processConversionPath();
	void getMatrixConversion(float* matrix);
	void processConversion(float** inBufs, float** outBufs, int numSamples);

	std::vector<int>& getFormatConversionPath();
};

#endif /* Mach1TranscodeCore_h */
