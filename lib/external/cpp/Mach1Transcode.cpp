//  Mach1 Spatial SDK
//  Copyright © 2017-2020 Mach1. All rights reserved.

#include "Mach1Transcode.h"

Mach1Transcode::Mach1Transcode()
{
	M1obj = Mach1TranscodeCAPI_create();
}

Mach1Transcode::~Mach1Transcode()
{
	Mach1TranscodeCAPI_delete(M1obj);
}

int Mach1Transcode::getInputNumChannels()
{
	return Mach1TranscodeCAPI_getInputNumChannels(M1obj);
    /// Returns the number of channels for indicated input format
    ///
    /// - Returns:
    ///     - integer of number of input channels
}

int Mach1Transcode::getOutputNumChannels()
{
	return Mach1TranscodeCAPI_getOutputNumChannels(M1obj);
    /// Returns the number of channels for indicated output format
    ///
    /// - Returns:
    ///     - integer of number of output channels
}

Mach1TranscodeFormatType Mach1Transcode::getFormatFromString(char * str)
{
	return Mach1TranscodeCAPI_getFormatFromString(M1obj, str);
	/// Returns the enum for indicated format's string name
	///
	/// - Parameters: 
	///		- string of format name
	/// - Returns: 
	///		- format from enum
}

char* Mach1Transcode::getFormatName(Mach1TranscodeFormatType fmt) {
	return Mach1TranscodeCAPI_getFormatName(M1obj, fmt);
}

float Mach1Transcode::processNormalization(float** bufs, int numSamples)
{
	return Mach1TranscodeCAPI_processNormalization(M1obj, bufs, numSamples);
}

void Mach1Transcode::processMasterGain(float** bufs, int numSamples, float masterGain)
{
	Mach1TranscodeCAPI_processMasterGain(M1obj, bufs, numSamples, masterGain);
}

float Mach1Transcode::db2level(float db)
{
	return Mach1TranscodeCAPI_db2level(M1obj, db);
}

float Mach1Transcode::level2db(float level)
{
	return Mach1TranscodeCAPI_level2db(M1obj, level);
}

void Mach1Transcode::setLFESub(std::vector<int> subChannelIndices, int sampleRate)
{
	Mach1TranscodeCAPI_setLFESub(M1obj, subChannelIndices.data(), subChannelIndices.size(), sampleRate);
}

void Mach1Transcode::setSpatialDownmixer(float corrThreshold)
{
	Mach1TranscodeCAPI_setSpatialDownmixer(M1obj, corrThreshold);
}

bool Mach1Transcode::getSpatialDownmixerPossibility()
{
	return Mach1TranscodeCAPI_getSpatialDownmixerPossibility(M1obj);
}

void Mach1Transcode::setInputFormat(Mach1TranscodeFormatType inFmt)
{
	Mach1TranscodeCAPI_setInputFormat(M1obj, inFmt);
}

void Mach1Transcode::setInputFormatADM(char* inXml)
{
	Mach1TranscodeCAPI_setInputFormatADM(M1obj, inXml);
}

void Mach1Transcode::setInputFormatTTJson(char* strJson)
{
	Mach1TranscodeCAPI_setInputFormatTTJson(M1obj, strJson);
}

void Mach1Transcode::setInputFormatTTPoints(std::vector<Mach1Point3D> points)
{
	Mach1TranscodeCAPI_setInputFormatTTPoints(M1obj, points.data(), points.size());
}

void Mach1Transcode::setOutputFormat(Mach1TranscodeFormatType outFmt)
{
	Mach1TranscodeCAPI_setOutputFormat(M1obj, outFmt);
}

void Mach1Transcode::setOutputFormatTTJson(char* strJson)
{
	Mach1TranscodeCAPI_setOutputFormatTTJson(M1obj, strJson);
}

void Mach1Transcode::setOutputFormatTTPoints(std::vector<Mach1Point3D> points)
{
	Mach1TranscodeCAPI_setInputFormatTTPoints(M1obj, points.data(), points.size());
}

bool Mach1Transcode::processConversionPath()
{
	return Mach1TranscodeCAPI_processConversionPath(M1obj);
}

std::vector<std::vector<float>> Mach1Transcode::getMatrixConversion()
{
	float* matrix = new float[getInputNumChannels() * getOutputNumChannels()];
	Mach1TranscodeCAPI_getMatrixConversion(M1obj, matrix);

	std::vector<std::vector<float>> vec;
	vec.resize(getOutputNumChannels());
	for (size_t i = 0; i < vec.size(); i++)
	{
		vec[i].resize(getInputNumChannels());
		for (size_t j = 0; j < vec[i].size(); j++)
		{
			vec[i][j] = matrix[i * getInputNumChannels() + j];
		}
	}

	delete[] matrix;
	return vec;
}

void Mach1Transcode::processConversion(float** inBufs, float** outBufs, int numSamples)
{
	Mach1TranscodeCAPI_processConversion(M1obj, inBufs, outBufs, numSamples);
}

std::vector<Mach1TranscodeFormatType> Mach1Transcode::getFormatsConvertionPath()
{
	int count = 0;
	Mach1TranscodeFormatType* arr = Mach1TranscodeCAPI_getFormatsConvertionPath(M1obj, &count);

	std::vector<Mach1TranscodeFormatType> vec(arr, arr + count);
	return vec;
}

 
