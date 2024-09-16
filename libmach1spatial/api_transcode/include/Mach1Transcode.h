//  Mach1 Spatial SDK
//  Copyright © 2017-2021 Mach1. All rights reserved.

#pragma once

#include "Mach1TranscodeCAPI.h"
#include <string>
#include <vector>

template <typename PCM>
class Mach1Transcode {
public:
    Mach1Transcode();

    ~Mach1Transcode();

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
    PCM processNormalization(std::vector<std::vector<PCM>> &bufs);

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
    void processMasterGain(std::vector<std::vector<PCM>> &bufs, PCM masterGain);

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
    std::vector<std::vector<PCM>> getMatrixConversion();

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
    void processConversion(std::vector<std::vector<PCM>>& in, std::vector<std::vector<PCM>>& out, int numSamples);

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
    void processConversionRebuffer(std::vector<std::vector<PCM>>& in, std::vector<std::vector<PCM>>& out, int numSamples);

    std::vector<PCM> getAvgSamplesDiff();

    PCM db2level(PCM db);

    PCM level2db(PCM level);

private:
    void *M1obj;

    std::vector<PCM*> input_ptrs;
    std::vector<PCM*> output_ptrs;

    std::vector<std::vector<PCM>> input_conversion_buffer;
    std::vector<std::vector<PCM>> output_conversion_buffer;

    int ic_channel_count{};
    size_t ic_buffer_size{};

    int oc_channel_count{};
    size_t oc_buffer_size{};
};

#include "Mach1Transcode.tpp" // templated implementation of Mach1Transcode