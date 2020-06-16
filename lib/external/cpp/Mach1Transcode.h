//  Mach1 Spatial SDK
//  Copyright © 2017-2020 Mach1. All rights reserved.

#pragma once

#include <vector>
#include <string>
#include "Mach1TranscodeCAPI.h"

class Mach1Transcode {
private:
	void* M1obj;
	
public:
	Mach1Transcode();
	~Mach1Transcode();

	int getInputNumChannels();
	int getOutputNumChannels();
	Mach1TranscodeFormatType getFormatFromString(char* str);
	char* getFormatName(Mach1TranscodeFormatType fmt);

	float calcNormalization(float** bufs, int numSamples);
	void applyMasterGain(float** bufs, int numSamples, float masterGain = 1.0f);

	float db2level(float db);
	float level2db(float level);

	void setLFESub(std::vector<int> subChannelIndices, int sampleRate);
	void setSpatialDownmixer(float corrThreshold = 0.1);
	bool getSpatialDownmixerPossibility();

	void setInputFormat(Mach1TranscodeFormatType inFmt);
	void setInputFormatADM(char* inXml);
	void setInputFormatTTJson(char* inJson);
	void setInputFormatTTPoints(std::vector<Mach1Point3D> points);

	void setOutputFormat(Mach1TranscodeFormatType outFmt);
	void setOutputFormatTTJson(char* outJson);
	void setOutputFormatTTPoints(std::vector<Mach1Point3D> points);

	bool computeConvertionPath();
	void getMatrixConversion(float* matrix);
	void convert(float** inBufs, float** outBufs, int numSamples);

	std::vector<Mach1TranscodeFormatType> getFormatsConvertionPath();
};
