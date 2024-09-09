//  Mach1 Spatial SDK
//  Copyright © 2017-2020 Mach1. All rights reserved.

#pragma once

#include "Mach1DecodeCAPI.h"
#include <string>
#include <vector>

class Mach1Decode {
  public:
    Mach1Decode();
    ~Mach1Decode();

    void setPlatformType(Mach1PlatformType type);
    void setDecodeAlgoType(Mach1DecodeAlgoType newAlgorithmType);

#ifndef __EMSCRIPTEN__
    void decode(float Yaw, float Pitch, float Roll, float *result, int bufferSize = 0, int sampleIndex = 0);
    void decodeCoeffs(float *result, int bufferSize = 0, int sampleIndex = 0);
    void decodePannedCoeffs(float *result, int bufferSize = 0, int sampleIndex = 0, bool applyPanLaw = true);
#endif
    std::vector<float> decode(float Yaw, float Pitch, float Roll, int bufferSize = 0, int sampleIndex = 0);
    std::vector<float> decodeCoeffs(int bufferSize = 0, int sampleIndex = 0);
    std::vector<float> decodePannedCoeffs(int bufferSize = 0, int sampleIndex = 0, bool applyPanLaw = true);
    std::vector<float> decodeCoeffsUsingTranscodeMatrix(std::vector< std::vector<float> > matrix, int channels, int bufferSize = 0, int sampleIndex = 0);

    int getFormatChannelCount();
    int getFormatCoeffCount();
    void setRotation(Mach1Point3D newRotationFromMinusOnetoOne);
    void setRotationDegrees(Mach1Point3D newRotationDegrees);
    void setRotationRadians(Mach1Point3D newRotationRadians);
    void setRotationQuat(Mach1Point4D newRotationQuat);

    void setFilterSpeed(float filterSpeed);

    template <typename T>
    inline void decodeBuffer(std::vector< std::vector<T> > &in, std::vector< std::vector<T> > &out, int size);

    template <typename T>
    inline void decodeBufferInPlace(std::vector< std::vector<T> > &buffer, int size);

    template <typename T>
    inline void decodeBufferRebuffer(std::vector< std::vector<T> > &in, std::vector< std::vector<T> > &out, int size);

    template <typename T>
    inline void decodeBufferInPlaceRebuffer(std::vector< std::vector<T> > &buffer, int size);

    long getCurrentTime();
#ifndef __EMSCRIPTEN__
    char *getLog();
#else
    std::string getLog();
#endif

    Mach1Point3D getCurrentAngle();

private:
    inline void restructureIntermediaryBuffer(int channel_count, size_t buffer_size);
    inline void clearIntermediaryBuffer();

private:
    void *M1obj;

    std::vector<std::vector<float > > intermediary_buffer;
    int ib_channel_count;
    size_t ib_buffer_size;
};

void Mach1Decode::restructureIntermediaryBuffer(int channel_count, size_t buffer_size) {
    if (ib_channel_count == channel_count && ib_buffer_size == buffer_size) return;

    intermediary_buffer.resize(channel_count);

    for (int i = 0; i < channel_count; i++) {
        intermediary_buffer[i].resize(buffer_size);
    }

    ib_channel_count = channel_count;
    ib_buffer_size = buffer_size;
}

void Mach1Decode::clearIntermediaryBuffer() {
    for (int i = 0; i < ib_channel_count; i++) {
        memset(intermediary_buffer[i].data(), 0, sizeof(float) * ib_buffer_size);
    }
}

template <typename T>
void Mach1Decode::decodeBuffer(std::vector< std::vector<T> > &in, std::vector< std::vector<T> > &out, int size) {
    // get output gain multipliers
    auto decode_gains = decodeCoeffs(); // TODO: Implement interpolation between coeffs.

    // process the samples manually
    for (int decode_idx = 0, output_idx = 0; decode_idx < decode_gains.size(); decode_idx += 2, output_idx += 1) {
        for (int sample_idx = 0; sample_idx < size; sample_idx++) {

            // get the sample in each loop
            float sample = in[output_idx][sample_idx];

            // clear the output for the new values to come in
            out[output_idx][sample_idx] = 0;
            out[0][sample_idx] += sample * decode_gains[decode_idx + 0];
            out[1][sample_idx] += sample * decode_gains[decode_idx + 1];
        }
    }
}

template<typename T>
void Mach1Decode::decodeBufferInPlace(std::vector<std::vector<T>> &buffer, int size) {
    decodeBuffer(buffer, buffer, size);
}

template<typename T>
void Mach1Decode::decodeBufferRebuffer(std::vector<std::vector<T>> &in, std::vector<std::vector<T>> &out, int size)
{
    restructureIntermediaryBuffer(2, size);
    clearIntermediaryBuffer();

    // get output gain multipliers
    auto decode_gains = decodeCoeffs(); // TODO: Implement interpolation between coeffs.

    // process the samples manually
    for (int decode_idx = 0, output_idx = 0; decode_idx < decode_gains.size(); decode_idx += 2, output_idx += 1) {
        for (int sample_idx = 0; sample_idx < size; sample_idx++) {

            // get the sample in each loop
            float sample = in[output_idx][sample_idx];

            intermediary_buffer[0][sample_idx] += sample * decode_gains[decode_idx + 0];
            intermediary_buffer[1][sample_idx] += sample * decode_gains[decode_idx + 1];
        }
    }

    for (int output_idx = 0; output_idx < 2; output_idx++) {
        memcpy(out[output_idx].data(), intermediary_buffer[output_idx].data(), sizeof(float) * size);
    }

}

template<typename T>
void Mach1Decode::decodeBufferInPlaceRebuffer(std::vector< std::vector<T> > &buffer, int size)
{
    decodeBufferRebuffer(buffer, buffer, size);
}

