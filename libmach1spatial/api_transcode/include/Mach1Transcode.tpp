//  Mach1 Spatial SDK
//  Copyright © 2017-2021 Mach1. All rights reserved.

#include <cstring>

template <typename PCM>
Mach1Transcode<PCM>::Mach1Transcode() {
    M1obj = Mach1TranscodeCAPI_create();
}

template <typename PCM>
Mach1Transcode<PCM>::~Mach1Transcode() {
    Mach1TranscodeCAPI_delete(M1obj);
}

template <typename PCM>
int Mach1Transcode<PCM>::getInputNumChannels() {
    return Mach1TranscodeCAPI_getInputNumChannels(M1obj);
}

template <typename PCM>
void Mach1Transcode<PCM>::setInputFormat(int inFmt) {
    Mach1TranscodeCAPI_setInputFormat(M1obj, inFmt);
}

template <typename PCM>
int Mach1Transcode<PCM>::getOutputNumChannels() {
    return Mach1TranscodeCAPI_getOutputNumChannels(M1obj);
}

template <typename PCM>
void Mach1Transcode<PCM>::setOutputFormat(int outFmt) {
    Mach1TranscodeCAPI_setOutputFormat(M1obj, outFmt);
}

template <typename PCM>
int Mach1Transcode<PCM>::getFormatFromString(std::string str) {
    return Mach1TranscodeCAPI_getFormatFromString(M1obj, (char *)str.c_str());
}

template <typename PCM>
std::string Mach1Transcode<PCM>::getFormatName(int fmt) {
    return std::string(Mach1TranscodeCAPI_getFormatName(M1obj, fmt));
}

template <typename PCM>
std::vector<std::string> Mach1Transcode<PCM>::getAllFormatNames() {
	const char** formats = Mach1TranscodeCAPI_getAllFormatNames(M1obj);
	std::vector<std::string> formatNames;
	for (size_t i = 0; formats[i] != nullptr; i++) {
		formatNames.push_back(std::string(formats[i]));
	}
	return formatNames;
}

template <typename PCM>
int Mach1Transcode<PCM>::getFormatsCount() {
    return Mach1TranscodeCAPI_getFormatsCount(M1obj);
}

#ifndef __EMSCRIPTEN__
template <typename PCM>
PCM Mach1Transcode<PCM>::processNormalization(PCM **bufs, int numSamples) {
    return Mach1TranscodeCAPI_processNormalization(M1obj, bufs, numSamples);
}

template <typename PCM>
void Mach1Transcode<PCM>::processMasterGain(PCM **bufs, int numSamples, PCM masterGain) {
    Mach1TranscodeCAPI_processMasterGain(M1obj, bufs, numSamples, masterGain);
}
#endif

template <typename PCM>
PCM Mach1Transcode<PCM>::processNormalization(std::vector<std::vector<PCM>> &bufs) {
    if (bufs.size() == 0)
        return 0;

    float **b = new float *[bufs.size()];
    for (int i = 0; i < bufs.size(); i++) {
        b[i] = bufs[i].data();
    }
    float peak = Mach1TranscodeCAPI_processNormalization(M1obj, b, (int)bufs[0].size());
    delete[] b;

    return peak;
}

template <typename PCM>
void Mach1Transcode<PCM>::processMasterGain(std::vector<std::vector<PCM>> &bufs, PCM masterGain) {
    if (bufs.size() == 0)
        return;

    float **b = new float *[bufs.size()];
    for (int i = 0; i < bufs.size(); i++) {
        b[i] = bufs[i].data();
    }
    Mach1TranscodeCAPI_processMasterGain(M1obj, b, (int)bufs[0].size(), masterGain);
    delete[] b;
}

template <typename PCM>
PCM Mach1Transcode<PCM>::db2level(PCM db) {
    return Mach1TranscodeCAPI_db2level(M1obj, db);
}

template <typename PCM>
PCM Mach1Transcode<PCM>::level2db(PCM level) {
    return Mach1TranscodeCAPI_level2db(M1obj, level);
}

template <typename PCM>
void Mach1Transcode<PCM>::setLFESub(std::vector<int> subChannelIndices, int sampleRate) {
    Mach1TranscodeCAPI_setLFESub(M1obj, subChannelIndices.data(), (int)subChannelIndices.size(), sampleRate);
}

template <typename PCM>
void Mach1Transcode<PCM>::setSpatialDownmixer(float corrThreshold) {
    Mach1TranscodeCAPI_setSpatialDownmixer(M1obj, corrThreshold);
}

template <typename PCM>
bool Mach1Transcode<PCM>::getSpatialDownmixerPossibility() {
    return Mach1TranscodeCAPI_getSpatialDownmixerPossibility(M1obj);
}

template <typename PCM>
std::vector<PCM> Mach1Transcode<PCM>::getAvgSamplesDiff() {
    float *avg = Mach1TranscodeCAPI_getAvgSamplesDiff(M1obj);
    return std::vector<float>(avg, avg + 4);
}

template <typename PCM>
void Mach1Transcode<PCM>::setInputFormatCustomPointsJson(std::string inJson) {
    Mach1TranscodeCAPI_setInputFormatCustomPointsJson(M1obj, (char *)inJson.c_str());
}

template <typename PCM>
void Mach1Transcode<PCM>::setInputFormatCustomPoints(std::vector<Mach1Point3D> points) {
    Mach1TranscodeCAPI_setInputFormatCustomPoints(M1obj, points.data(), (int)points.size());
}

template <typename PCM>
void Mach1Transcode<PCM>::setOutputFormatCustomPointsJson(std::string strJson) {
    Mach1TranscodeCAPI_setOutputFormatCustomPointsJson(M1obj, (char *)strJson.c_str());
}

template <typename PCM>
void Mach1Transcode<PCM>::setOutputFormatCustomPoints(std::vector<Mach1Point3D> points) {
    Mach1TranscodeCAPI_setInputFormatCustomPoints(M1obj, points.data(), (int)points.size());
}

template <typename PCM>
void Mach1Transcode<PCM>::setCustomPointsSamplerCallback(Mach1Point3D *(*callback)(long long, int &)) {
    Mach1TranscodeCAPI_setCustomPointsSamplerCallback(M1obj, callback);
}

template <typename PCM>
bool Mach1Transcode<PCM>::processConversionPath() {
    return Mach1TranscodeCAPI_processConversionPath(M1obj);
}

template <typename PCM>
std::vector<std::vector<PCM>> Mach1Transcode<PCM>::getMatrixConversion() {
    float *matrix = new float[getInputNumChannels() * getOutputNumChannels()];
    Mach1TranscodeCAPI_getMatrixConversion(M1obj, matrix);

    std::vector<std::vector<float>> vec;
    vec.resize(getOutputNumChannels());
    for (size_t i = 0; i < vec.size(); i++) {
        vec[i].resize(getInputNumChannels());
        for (size_t j = 0; j < vec[i].size(); j++) {
            vec[i][j] = matrix[i * getInputNumChannels() + j];
        }
    }

    delete[] matrix;
    return vec;
}
#ifndef __EMSCRIPTEN__
template <typename PCM>
void Mach1Transcode<PCM>::processConversion(PCM **inBufs, PCM **outBufs, int numSamples) {
    Mach1TranscodeCAPI_processConversion(M1obj, inBufs, outBufs, numSamples);
}
#endif

template <typename PCM>
void Mach1Transcode<PCM>::processConversion(std::vector<std::vector<PCM>>& in, std::vector<std::vector<PCM>>& out, int numSamples) {
    if (in.size() == 0 || out.size() == 0)
        return;

    // restructure input buffer
    if (ic_channel_count != in.size() || ic_buffer_size != numSamples) {
        input_conversion_buffer.resize(in.size());
        input_ptrs.resize(in.size());

        for (int i = 0; i < in.size(); i++) {
            input_conversion_buffer[i].resize(numSamples);
        }

        ic_channel_count = in.size();
        ic_buffer_size = numSamples;
    }

    for (int i = 0; i < in.size(); i++) {
        input_ptrs[i] = in[i].data();
    }

    // restructure output buffer
    if (oc_channel_count != in.size() || oc_buffer_size != numSamples) {
        output_conversion_buffer.resize(in.size());
        output_ptrs.resize(in.size());

        for (int i = 0; i < in.size(); i++) {
            output_conversion_buffer[i].resize(numSamples);
        }

        oc_channel_count = in.size();
        oc_buffer_size = numSamples;
    }

    for (int i = 0; i < out.size(); i++) {
        output_ptrs[i] = out[i].data();
    }

    Mach1TranscodeCAPI_processConversion(M1obj, input_ptrs.data(), output_ptrs.data(), numSamples);
}

template <typename PCM>
void Mach1Transcode<PCM>::processConversionRebuffer(std::vector<std::vector<PCM>>& in, std::vector<std::vector<PCM>>& out, int numSamples) {
    if (in.size() == 0 || out.size() == 0) {
        return;
    }

    // restructure input buffer
    if (ic_channel_count != in.size() || ic_buffer_size != numSamples) {
        input_conversion_buffer.resize(in.size());
        input_ptrs.resize(in.size());

        for (int i = 0; i < in.size(); i++) {
            input_conversion_buffer[i].resize(numSamples);
        }

        ic_channel_count = in.size();
        ic_buffer_size = numSamples;
    }

    for (int i = 0; i < ic_channel_count; i++) {
        memcpy(input_conversion_buffer[i].data(), in.data(), sizeof(PCM) * ic_buffer_size);
        input_ptrs[i] = input_conversion_buffer[i].data();
    }

    // restructure output buffer
    if (oc_channel_count != in.size() || oc_buffer_size != numSamples) {
        output_conversion_buffer.resize(in.size());
        output_ptrs.resize(in.size());

        for (int i = 0; i < in.size(); i++) {
            output_conversion_buffer[i].resize(numSamples);
        }

        oc_channel_count = in.size();
        oc_buffer_size = numSamples;
    }

    for (int i = 0; i < oc_channel_count; i++) {
        memcpy(output_conversion_buffer[i].data(), out.data(), sizeof(PCM) * oc_buffer_size);
        output_ptrs[i] = output_conversion_buffer[i].data();
    }

    Mach1TranscodeCAPI_processConversion(M1obj, input_ptrs.data(), output_ptrs.data(), numSamples);

    for (int i = 0; i < oc_channel_count; i++) {
        memcpy(out.data(), output_conversion_buffer[i].data(), sizeof(PCM) * oc_buffer_size);
    }


}

template <typename PCM>
std::vector<int> Mach1Transcode<PCM>::getFormatConversionPath() {
    int count = 0;
    int *arr = Mach1TranscodeCAPI_getFormatConversionPath(M1obj, &count);

    std::vector<int> vec(arr, arr + count);
    return vec;
}
