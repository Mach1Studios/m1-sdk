//  Mach1 Spatial SDK
//  Copyright © 2017-2021 Mach1. All rights reserved.

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

Mach1TranscodeFormatType Mach1Transcode::getFormatFromString(std::string str)
{
	return Mach1TranscodeCAPI_getFormatFromString(M1obj, (char*)str.c_str());
	/// Returns the enum for indicated format's string name
	///
	/// - Parameters: 
	///		- string of format name
	/// - Returns: 
	///		- format from enum
}

std::string Mach1Transcode::getFormatName(Mach1TranscodeFormatType fmt) {
	return std::string(Mach1TranscodeCAPI_getFormatName(M1obj, fmt));
}
#ifndef  __EMSCRIPTEN__ 
float Mach1Transcode::processNormalization(float** bufs, int numSamples)
{
	return Mach1TranscodeCAPI_processNormalization(M1obj, bufs, numSamples);
}

void Mach1Transcode::processMasterGain(float** bufs, int numSamples, float masterGain)
{
	Mach1TranscodeCAPI_processMasterGain(M1obj, bufs, numSamples, masterGain);
    /// Applys an input gain to the output soundfield
    ///
    /// - Parameters: 
    ///     - input buffer
    ///     - integer of input number of samples
    ///     - float for gain multiplier
}
#endif

float Mach1Transcode::processNormalization(std::vector<std::vector<float>>& bufs)
{
	if (bufs.size() == 0) return 0;

	float** b = new float*[bufs.size()];
	for (int i = 0; i < bufs.size(); i++) {
		b[i] = bufs[i].data();
	}
	float peak = Mach1TranscodeCAPI_processNormalization(M1obj, b, bufs[0].size());
	delete[] b;

	return peak;
}

void Mach1Transcode::processMasterGain(std::vector<std::vector<float>>& bufs, float masterGain)
{
	if (bufs.size() == 0) return;

	float** b = new float*[bufs.size()];
	for (int i = 0; i < bufs.size(); i++) {
		b[i] = bufs[i].data();
	}
	Mach1TranscodeCAPI_processMasterGain(M1obj, b, bufs[0].size(), masterGain);
	delete[] b;
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
    /// Applys a low pass filter (LPF) to each indicated channel index of the input format and soundfield
    ///
    /// - Parameters: 
    ///     - integer inputs for channel indices to be processed
    ///     - integer for samplerate 
}

void Mach1Transcode::setSpatialDownmixer(float corrThreshold)
{
	Mach1TranscodeCAPI_setSpatialDownmixer(M1obj, corrThreshold);
    /// Sets the threshold float for `getSpatialDownmixerPossibility` calculation
    ///
    /// - Parameters: 
    ///     - float from 0.0 to 1.0 where 0.0 no difference and incrementing to 1.0 is more difference
}

bool Mach1Transcode::getSpatialDownmixerPossibility()
{
	return Mach1TranscodeCAPI_getSpatialDownmixerPossibility(M1obj);
    /// Returns true if compared signals are less than the `setSpatialDownmixer(corrThreshold)`
    ///
    /// - Returns: 
    ///     - boolean returned true if the compared signal difference is less then the `setSpatialDownmixer(corrThreshold)`
    ///
    /// - Remarks:
    ///     - when true; transcodings that are set to ouput to `Mach1Spatial` will process an additional conversion to `Mach1Horizon` 
}

void Mach1Transcode::setInputFormat(Mach1TranscodeFormatType inFmt)
{
	Mach1TranscodeCAPI_setInputFormat(M1obj, inFmt);
    /// Sets the input format for transcoding from the preset Mach1Transcode options
    ///
    /// Remarks:
    ///     View the current list of Mach1Transcode preset formats here: https://dev.mach1.tech/#formats-supported
}

void Mach1Transcode::setInputFormatADM(std::string inXml)
{
	Mach1TranscodeCAPI_setInputFormatADM(M1obj, (char*)inXml.c_str());
    /// Sets the input format for transcoding from the parsed ADM metadata within the audiofile
}

void Mach1Transcode::setInputFormatTTJson(std::string strJson)
{
	Mach1TranscodeCAPI_setInputFormatTTJson(M1obj, (char*)strJson.c_str());
    /// Sets the input format for transcoding from an external JSON source
    ///
    /// Remarks:
    ///     View the JSON spec for describing a format here: https://dev.mach1.tech/#json-descriptions
}

void Mach1Transcode::setInputFormatTTPoints(std::vector<Mach1Point3D> points)
{
	Mach1TranscodeCAPI_setInputFormatTTPoints(M1obj, points.data(), points.size());
    /// Sets the input format for transcoding from TT directly
    ///
    /// Remarks:
    ///     View the JSON spec for describing a format here: https://dev.mach1.tech/#json-descriptions
}

void Mach1Transcode::setOutputFormat(Mach1TranscodeFormatType outFmt)
{
	Mach1TranscodeCAPI_setOutputFormat(M1obj, outFmt);
    /// Sets the output format for transcoding from the preset Mach1Transcode options
    ///
    /// Remarks:
    ///     View the current list of Mach1Transcode preset formats here: https://dev.mach1.tech/#formats-supported
}

void Mach1Transcode::setOutputFormatTTJson(std::string strJson)
{
	Mach1TranscodeCAPI_setOutputFormatTTJson(M1obj, (char*)strJson.c_str());
    /// Sets the output format for transcoding from an external JSON source
    ///
    /// Remarks:
    ///     View the JSON spec for describing a format here: https://dev.mach1.tech/#json-descriptions
}

void Mach1Transcode::setOutputFormatTTPoints(std::vector<Mach1Point3D> points)
{
	Mach1TranscodeCAPI_setInputFormatTTPoints(M1obj, points.data(), points.size());
    /// Sets the output format for transcoding from TT directly
    ///
    /// Remarks:
    ///     View the JSON spec for describing a format here: https://dev.mach1.tech/#json-descriptions
}

bool Mach1Transcode::processConversionPath()
{
	return Mach1TranscodeCAPI_processConversionPath(M1obj);
    /// Use this function to control when to call for calculating the format transcoding calculations
    ///
    /// Remarks:
    ///     Needs to be called before `processConversion()` is called.
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
	/// Returns the transcoding matrix of coefficients based on the set input and output formats
}
#ifndef  __EMSCRIPTEN__ 
void Mach1Transcode::processConversion(float** inBufs, float** outBufs, int numSamples)
{
	Mach1TranscodeCAPI_processConversion(M1obj, inBufs, outBufs, numSamples);
    /// Call to process the conversion as set by previous functions
}
#endif

void Mach1Transcode::processConversion(std::vector<std::vector<float>>& inBufs, std::vector<std::vector<float>>& outBufs)
{
	if (inBufs.size() == 0 || outBufs.size() == 0) return;
	
	float** bIn = new float*[inBufs.size()];
	for (int i = 0; i < inBufs.size(); i++) {
		bIn[i] = inBufs[i].data();
	}
	float** bOut = new float*[outBufs.size()];
	for (int i = 0; i < outBufs.size(); i++) {
		bOut[i] = outBufs[i].data();
	}

	Mach1TranscodeCAPI_processConversion(M1obj, bIn, bOut, inBufs[0].size());

	delete[] bIn;
	delete[] bOut;
}

std::vector<Mach1TranscodeFormatType> Mach1Transcode::getFormatConversionPath()
{
	int count = 0;
	Mach1TranscodeFormatType* arr = Mach1TranscodeCAPI_getFormatConversionPath(M1obj, &count);

	std::vector<Mach1TranscodeFormatType> vec(arr, arr + count);
	return vec;
    /// Returns the shortest found conversion path to get from input format X to output format Y, both set by `Mach1Transcode::setInputFormat(Mach1TranscodeFormatType inFmt)` and `Mach1Transcode::setOutputFormat(Mach1TranscodeFormatType outFmt)`
    ///
    /// Remarks:
    ///     Majority of format instances will use Mach1Spatial as the middle format for non-Mach1-format -> non-Mach1-format transcodings. This is due to Mach1 Spatial being a platonic solid format, ideal for safe calculations without loss
}

 
