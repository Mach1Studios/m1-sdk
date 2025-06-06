//  Mach1 Spatial SDK
//  Copyright © 2017-2021 Mach1. All rights reserved.

#ifndef MACH1SPATIALSDK_MACH1TRANSCODE_H
#define MACH1SPATIALSDK_MACH1TRANSCODE_H

#include "Mach1TranscodeCAPI.h"
#include <string>
#include <vector>

template <typename PCM>
class Mach1Transcode {
  public:
    Mach1Transcode();

    ~Mach1Transcode();

    /**
     * @brief Get the input format that this Mach1Transcode has been configured to process.
     */
    int getInputFormat();

    /**
     * @brief Get the output format that this Mach1Transcode has been configured to process.
     */
    int getOutputFormat();

    /**
     * @brief Get the number of channels that this Mach1Transcode has been configured to process, based
     * on the input format.
     */
    int getInputNumChannels();

    /**
     * @brief Sets the input format for transcoding from this Mach1Transcode options.
     * @note All available preset formats are in Mach1TranscodeConstants.h
     */
    void setInputFormat(int inFmt);

    /**
     * @brief Get the number of channels that this Mach1Transcode has been configured to convert to, based
     * on the output format.
     */
    int getOutputNumChannels();

    /**
     * @brief Sets the output format for transcoding from this Mach1Transcode options.
     * @note All available preset formats are in Mach1TranscodeConstants.h
     */
    void setOutputFormat(int outFmt);

    /**
     * @brief the enum for indicated format's string name.
     * @param std String representing the human-readable name of the format.
     * @return integer representing aforementioned format.
     */
    int getFormatFromString(std::string str);

    /**
     * @brief Given an input/output format index, return its human-readable name.
     * @param fmt integer representing aforementioned format.
     * @return String representing the human-readable name of the format.
     */
    std::string getFormatName(int fmt);

    /**
     * @brief Get a list of all the names of available formats in Mach1Transcode API.
     * @return vector of strings of the names of all formats.
     */
    std::vector<std::string> getAllFormatNames();

    /**
     * @brief Get the amount of available formats in Mach1Transcode API.
     */
    int getFormatsCount();

    /**
     * @brief Get the largest PCM value in the given buffer, which would be used for normalization.
     */
    PCM processNormalization(std::vector<std::vector<PCM> > &bufs);

#ifndef __EMSCRIPTEN__
    /**
     * @brief Get the largest PCM value in the given buffer, which would be used for normalization.
     * @note bufs is expected to hold `getOutputNumChannels` amount of arrays of size `numSamples`.
     */
    PCM processNormalization(PCM **bufs, int numSamples);
#endif

    /**
     * @brief Apply an input gain to the output sound field.
     * @param bufs input buffer
     * @param masterGain float for gain multiplier
     */
    void processMasterGain(std::vector<std::vector<PCM> > &bufs, PCM masterGain);

#ifndef __EMSCRIPTEN__
    /**
     * @brief Apply an input gain to the output sound field.
     * @param bufs input buffer
     * @param numSamples input number of samples
     * @param masterGain float for gain multiplier
     */
    void processMasterGain(PCM **bufs, int numSamples, PCM masterGain);
#endif

    /**
     * @brief Apply a low pass filter (LPF) to each indicated channel index of the input format and sound field
     * @param subChannelIndices integer inputs for channel indices to be processed
     * @param sampleRate integer for samplerate
     */
    void setLFESub(std::vector<int> subChannelIndices, int sampleRate);

    /**
     * @brief Sets the threshold float for `getSpatialDownmixerPossibility` calculation
     * @param corrThreshold float from 0.0 to 1.0 where 0.0 no difference and incrementing to 1.0 is more difference
     */
    void setSpatialDownmixer(float corrThreshold = 0.1);

    /**
     * @brief Get whether the compared signals are less than the setSpatialDownmixer(corrThreshold).
     * @return true if the compared signal difference is less then the setSpatialDownmixer(corrThreshold)
     * @note when true; transcodings that are set to ouput to `Mach1Spatial` will process an additional
     * conversion to `Mach1Horizon`
     */
    bool getSpatialDownmixerPossibility();

    /**
     * @brief Sets the input format for transcoding from CustomPoints directly.
     * @note View the JSON spec for describing a format here: https://dev.mach1.tech/#json-descriptions
     */
    void setInputFormatCustomPointsJson(std::string inJson);

    /**
     * @brief Sets the input format for transcoding from CustomPoints directly
     * @note View the JSON spec for describing a format here: https://dev.mach1.tech/#json-descriptions
     */
    void setInputFormatCustomPoints(std::vector<Mach1Point3D> points);

    /**
     * @brief Sets the output format for transcoding from an external JSON source
     * @note View the JSON spec for describing a format here: https://dev.mach1.tech/#json-descriptions
     */
    void setOutputFormatCustomPointsJson(std::string outJson);

    /**
     * @brief Sets the output format for transcoding from CustomPoints directly
     * @note View the JSON spec for describing a format here: https://dev.mach1.tech/#json-descriptions
     */
    void setOutputFormatCustomPoints(std::vector<Mach1Point3D> points);

    void setCustomPointsSamplerCallback(Mach1Point3D *(*callback)(long long, int &));

    /**
     * @brief Use this function to control when to call for calculating the format transcoding calculations
     * @return true if successful, false when no conversion path is found (used for error handling)
     * @note Needs to be called before `processConversion()` is called.
     */
    bool processConversionPath();

    /**
     * @brief Get the transcoding matrix of coefficients based on the set input and output formats
     * @return vector of vector of coefficients
     */
    std::vector<std::vector<PCM> > getMatrixConversion();

    /**
     * @brief Get the shortest found conversion path to get from input format X to output format Y, both
     * set by `Mach1Transcode::setInputFormat(int inFmt)` and `Mach1Transcode::setOutputFormat(int outFmt)`
     * @note Majority of format instances will use Mach1Spatial as the middle format for
     * non-Mach1-format -> non-Mach1-format transcoding. This is due to Mach1 Spatial being a platonic
     * solid format, ideal for safe calculations without loss
     */
    std::vector<int> getFormatConversionPath();

    /**
     * @brief Process the conversion as set by previous functions.
     */
    void processConversion(std::vector<std::vector<PCM> > &in, std::vector<std::vector<PCM> > &out, int numSamples);

#ifndef __EMSCRIPTEN__
    /**
     * @brief Process the conversion as set by previous functions.
     */
    void processConversion(PCM **inBufs, PCM **outBufs, int numSamples);
#endif

    /**
     * @brief Process the conversion as set by previous functions.
     * @note Process the input buffer in an intermediary buffer, and then move it to the output buffer
     */
    void processConversionRebuffer(std::vector<std::vector<PCM> > &in, std::vector<std::vector<PCM> > &out, int numSamples);

    std::vector<PCM> getAvgSamplesDiff();

    PCM db2level(PCM db);

    PCM level2db(PCM level);

  private:
    void *M1obj;

    std::vector<PCM *> input_ptrs;
    std::vector<PCM *> output_ptrs;

    std::vector<std::vector<PCM> > input_conversion_buffer;
    std::vector<std::vector<PCM> > output_conversion_buffer;

    int ic_channel_count{};
    size_t ic_buffer_size{};

    int oc_channel_count{};
    size_t oc_buffer_size{};
};

#endif // MACH1SPATIALSDK_MACH1TRANSCODE_H

// Implementation of Mach1Transcode<PCM>
#ifndef MACH1SPATIALSDK_MACH1TRANSCODE_TPP
#define MACH1SPATIALSDK_MACH1TRANSCODE_TPP

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
int Mach1Transcode<PCM>::getInputFormat() {
    return Mach1TranscodeCAPI_getInputFormat(M1obj);
}

template <typename PCM>
int Mach1Transcode<PCM>::getOutputFormat() {
    return Mach1TranscodeCAPI_getOutputFormat(M1obj);
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
    const char *name = Mach1TranscodeCAPI_getFormatName(M1obj, fmt);
    return name ? std::string(name) : std::string();
}

template <typename PCM>
std::vector<std::string> Mach1Transcode<PCM>::getAllFormatNames() {
    const char **formats = Mach1TranscodeCAPI_getAllFormatNames(M1obj);
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
PCM Mach1Transcode<PCM>::processNormalization(std::vector<std::vector<PCM> > &bufs) {
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
void Mach1Transcode<PCM>::processMasterGain(std::vector<std::vector<PCM> > &bufs, PCM masterGain) {
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
std::vector<std::vector<PCM> > Mach1Transcode<PCM>::getMatrixConversion() {
    float *matrix = new float[getInputNumChannels() * getOutputNumChannels()];
    Mach1TranscodeCAPI_getMatrixConversion(M1obj, matrix);

    std::vector<std::vector<float> > vec;
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
void Mach1Transcode<PCM>::processConversion(std::vector<std::vector<PCM> > &in, std::vector<std::vector<PCM> > &out, int numSamples) {
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
    if (oc_channel_count != out.size() || oc_buffer_size != numSamples) {
        output_conversion_buffer.resize(out.size());
        output_ptrs.resize(out.size());

        for (int i = 0; i < out.size(); i++) {
            output_conversion_buffer[i].resize(numSamples);
        }

        oc_channel_count = out.size();
        oc_buffer_size = numSamples;
    }

    for (int i = 0; i < out.size(); i++) {
        output_ptrs[i] = out[i].data();
    }

    Mach1TranscodeCAPI_processConversion(M1obj, input_ptrs.data(), output_ptrs.data(), numSamples);
}

template <typename PCM>
void Mach1Transcode<PCM>::processConversionRebuffer(std::vector<std::vector<PCM> > &in, std::vector<std::vector<PCM> > &out, int numSamples) {
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
    if (oc_channel_count != out.size() || oc_buffer_size != numSamples) {
        output_conversion_buffer.resize(out.size());
        output_ptrs.resize(out.size());

        for (int i = 0; i < out.size(); i++) {
            output_conversion_buffer[i].resize(numSamples);
        }

        oc_channel_count = out.size();
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

#endif // MACH1SPATIALSDK_MACH1TRANSCODE_TPP
