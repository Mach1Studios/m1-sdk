//  Mach1 Spatial SDK
//  Copyright © 2017-2020 Mach1. All rights reserved.

#include "Mach1TranscodeCore.h"
#include "Mach1TranscodeCAPI.h"
#include "Mach1Point3DCore.h"
#include "external\cpp\Mach1TranscodeCAPI.h"

void* Mach1TranscodeCAPI_create()
{
	return new Mach1TranscodeCore();
}

void Mach1TranscodeCAPI_delete(void* M1obj)
{
	if (M1obj != nullptr) {
		delete (Mach1TranscodeCore*)M1obj;
		M1obj = nullptr;
	}
}

M1_API int Mach1TranscodeCAPI_getInputNumChannels(void* M1obj)
{
	return ((Mach1TranscodeCore*)M1obj)->getInputNumChannels();
}

M1_API int Mach1TranscodeCAPI_getOutputNumChannels(void* M1obj)
{
	return ((Mach1TranscodeCore*)M1obj)->getOutputNumChannels();
}

M1_API Mach1TranscodeFormatType Mach1TranscodeCAPI_getFormatFromString(void* M1obj, char* str)
{
	return (Mach1TranscodeFormatType)((Mach1TranscodeCore*)M1obj)->getFormatFromString(str);
}

M1_API const char* Mach1TranscodeCAPI_getFormatName(void* M1obj, Mach1TranscodeFormatType fmt)
{
	return ((Mach1TranscodeCore*)M1obj)->getFormatName(M1obj, (Mach1TranscodeFormats::FormatType)fmt);
}

M1_API float Mach1TranscodeCAPI_processNormalization(void* M1obj, float** bufs, int numSamples)
{
	return ((Mach1TranscodeCore*)M1obj)->processNormalization(bufs, numSamples);
}

M1_API void Mach1TranscodeCAPI_processMasterGain(void* M1obj, float** bufs, int numSamples, float masterGain)
{
	((Mach1TranscodeCore*)M1obj)->processMasterGain(bufs, numSamples, masterGain);
}

M1_API float Mach1TranscodeCAPI_db2level(void* M1obj, float db)
{
	return ((Mach1TranscodeCore*)M1obj)->db2level(db);
}

M1_API float Mach1TranscodeCAPI_level2db(void* M1obj, float level)
{
	return ((Mach1TranscodeCore*)M1obj)->level2db(level);
}

M1_API void Mach1TranscodeCAPI_setLFESub(void* M1obj, int* subChannelIndices, int numChannels, int sampleRate)
{
	std::vector<int> values(subChannelIndices, subChannelIndices + numChannels);
	((Mach1TranscodeCore*)M1obj)->setLFESub(values, sampleRate);
}

M1_API void Mach1TranscodeCAPI_setSpatialDownmixer(void* M1obj, float corrThreshold)
{
	((Mach1TranscodeCore*)M1obj)->setSpatialDownmixer(corrThreshold);
}

M1_API bool Mach1TranscodeCAPI_getSpatialDownmixerPossibility(void* M1obj)
{
	return ((Mach1TranscodeCore*)M1obj)->getSpatialDownmixerPossibility();
}

M1_API float* Mach1TranscodeCAPI_getAvgSamplesDiff(void* M1obj)
{
	const std::vector<float>& avgSamplesDiff = ((Mach1TranscodeCore*)M1obj)->getAvgSamplesDiff();
	return (float*)avgSamplesDiff.data();
}

M1_API void Mach1TranscodeCAPI_setInputFormat(void* M1obj, Mach1TranscodeFormatType inFmt)
{
	((Mach1TranscodeCore*)M1obj)->setInputFormat((Mach1TranscodeFormats::FormatType)inFmt);
}

M1_API void Mach1TranscodeCAPI_setInputFormatCustomPointsJson(void* M1obj, char* strJson)
{
	((Mach1TranscodeCore*)M1obj)->setInputFormatCustomPointsJson(strJson);
}

M1_API void Mach1TranscodeCAPI_setInputFormatCustomPoints(void* M1obj, Mach1Point3D* points, int count)
{
	((Mach1TranscodeCore*)M1obj)->setInputFormatCustomPoints(std::vector< Mach1Point3DCore>((Mach1Point3DCore*)points, (Mach1Point3DCore*)points + count));
}

M1_API void Mach1TranscodeCAPI_setOutputFormat(void* M1obj, Mach1TranscodeFormatType outFmt)
{
	((Mach1TranscodeCore*)M1obj)->setOutputFormat((Mach1TranscodeFormats::FormatType)outFmt);
}

M1_API void Mach1TranscodeCAPI_setOutputFormatCustomPointsJson(void* M1obj, char* strJson)
{
	((Mach1TranscodeCore*)M1obj)->setOutputFormatCustomPointsJson(strJson);
}

M1_API void Mach1TranscodeCAPI_setOutputFormatCustomPoints(void* M1obj, Mach1Point3D* points, int count)
{
	((Mach1TranscodeCore*)M1obj)->setOutputFormatCustomPoints(std::vector< Mach1Point3DCore>((Mach1Point3DCore*)points, (Mach1Point3DCore*)points + count));
}

M1_API void Mach1TranscodeCAPI_setCustomPointsSamplerCallback(void * M1obj, Mach1Point3D *(*callback)(long long, int &))
{
	((Mach1TranscodeCore*)M1obj)->setCustomPointsSamplerCallback(callback);
}

M1_API bool Mach1TranscodeCAPI_processConversionPath(void* M1obj)
{
	return ((Mach1TranscodeCore*)M1obj)->processConversionPath();
}

M1_API void Mach1TranscodeCAPI_processConversion(void* M1obj, float** inBufs, float** outBufs, int numSamples)
{
	((Mach1TranscodeCore*)M1obj)->processConversion(inBufs, outBufs, numSamples);
}

M1_API void Mach1TranscodeCAPI_getMatrixConversion(void * M1obj, float *matrix)
{
	((Mach1TranscodeCore*)M1obj)->getMatrixConversion(matrix);
}

M1_API Mach1TranscodeFormatType * Mach1TranscodeCAPI_getFormatConversionPath(void* M1obj, int* count)
{
	std::vector<Mach1TranscodeFormats::FormatType>& formatsPath = ((Mach1TranscodeCore*)M1obj)->getFormatConversionPath();
	*count = (int)formatsPath.size();
	return (Mach1TranscodeFormatType*)formatsPath.data();
}





































