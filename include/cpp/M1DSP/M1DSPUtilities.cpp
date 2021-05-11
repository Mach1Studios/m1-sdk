//  Mach1 Spatial SDK
//  Copyright © 2017-2021 Mach1. All rights reserved.

#include "M1DSPUtilities.h"

void M1DSP::Utilities::CSpatialDownmixChecker::Setup(float threshold) {
	this->threshold = threshold;

	totalSamples = 0;
	sumSample = std::vector<float>(4);
}

void M1DSP::Utilities::CSpatialDownmixChecker::ProcessBuffer(float ** buffer, int bufferSize) {
	for (int j = 0; j < bufferSize; j++) {
		for (int n = 0; n < sumSample.size(); n++) {
			sumSample[n] += fabs(buffer[n][j] - buffer[4 + n][j]);
		}
	}
	totalSamples += bufferSize;
}

std::vector<float>& M1DSP::Utilities::CSpatialDownmixChecker::getAvgSamplesDiff() {
	avgSamplesDiff.resize(sumSample.size());
	for (int n = 0; n < avgSamplesDiff.size(); n++) {
		avgSamplesDiff[n] = sumSample[n] / totalSamples;
	}
	return avgSamplesDiff;
}

bool M1DSP::Utilities::CSpatialDownmixChecker::bShouldDownmix() {
	bool ConvertToHorizon = false;
	std::vector<float>& avgSamplesDiff = getAvgSamplesDiff();
	for (int n = 0; n < avgSamplesDiff.size(); n++) {
		if (avgSamplesDiff[n] < threshold) ConvertToHorizon = true;
	}
	return ConvertToHorizon;
}
