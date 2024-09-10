//  Mach1 Spatial SDK
//  Copyright © 2017-2020 Mach1. All rights reserved.

#pragma once

#ifdef M1ENCODE_INLINE_DECODE
#include "Mach1DecodeCAPI.h"
#endif
#include "Mach1EncodeCAPI.h"
#include <string>
#include <cstring>
#include <vector>

class Mach1Encode {

  public:
    Mach1Encode();
    ~Mach1Encode();

    Mach1Encode(const Mach1Encode &other);
    Mach1Encode &operator=(const Mach1Encode &other);
    Mach1Encode(Mach1Encode&& other) = default; // TODO: Implement
    Mach1Encode& operator=(Mach1Encode&& other) = default; // TODO: Implement

    std::vector<Mach1Point3D> getPoints();
    std::vector< std::vector<float> > getGains();
    std::vector<std::string> getPointsNames();
    std::vector<float> getGainsForInputChannelNamed(std::string pointName);

    void generatePointResults();
    int getPointsCount();

#ifdef M1ENCODE_INLINE_DECODE
    std::vector<float> getResultingCoeffsDecoded(Mach1DecodeAlgoType decodeType, std::vector<float> &decodeResult);
#endif

    Mach1EncodeInputModeType getInputMode();
    Mach1EncodeOutputModeType getOutputMode();
	Mach1EncodePannerModeType getPannerMode();
	bool getAutoOrbit();
	int getInputChannelsCount();
    int getOutputChannelsCount();

    template <typename T>
    inline void encodeBuffer(std::vector< std::vector<T> >& in, std::vector< std::vector<T> >& out, int size);

    template <typename T>
    inline void encodeBufferRebuffer(std::vector< std::vector<T> >& in, std::vector< std::vector<T> >& out, int size);

    template <typename T>
    inline void encodeBufferInPlaceRebuffer(std::vector<std::vector<T>>& buffer, size_t buffer_size);

    template <typename T>
    inline void encodeBufferInPlace(std::vector<std::vector<T>>& buffer, size_t buffer_size);

    void setInputMode(Mach1EncodeInputModeType inputMode);
    void setOutputMode(Mach1EncodeOutputModeType outputMode);

    void setAzimuth(float azimuthFromMinus1To1);
    void setAzimuthDegrees(float azimuthDegrees);
    void setAzimuthRadians(float azimuthRadians);

    void setElevation(float elevationFromMinus1to1);
    void setElevationDegrees(float elevationFromMinus90to90);
    void setElevationRadians(float elevationFromMinusHalfPItoHalfPI);

    void setPannerMode(enum Mach1EncodePannerModeType pannerMode);
    void setFrontSurroundPerspective(bool frontSurroundPerspective);
    void setOutputGain(float outputGainMultipler, bool isDecibel);

    void setOrbitRotation(float orbitRotationFromMinusOnetoOne);
    void setOrbitRotationDegrees(float orbitRotationDegrees);
    void setOrbitRotationRadians(float orbitRotationRadians);

    void setDiverge(float divergeFromMinus1To1);
    void setStereoSpread(float sSpreadFrom0to1);
    void setAutoOrbit(bool autoOrbit);

private:
    inline void restructureIntermediaryBuffer(int channel_count, size_t buffer_size);
    inline void clearIntermediaryBuffer();

private:
    void *M1obj;

    std::vector< std::vector<float> > gains;
    std::vector< std::vector<float> > intermediary_buffer;

    int ib_channel_count;
    size_t ib_buffer_size;
};


void Mach1Encode::restructureIntermediaryBuffer(int channel_count, size_t buffer_size) {
    if (ib_channel_count == channel_count && ib_buffer_size == buffer_size) return;

    intermediary_buffer.resize(channel_count);

    for (int i = 0; i < channel_count; i++) {
        intermediary_buffer[i].resize(buffer_size);
    }

    ib_channel_count = channel_count;
    ib_buffer_size = buffer_size;
}

void Mach1Encode::clearIntermediaryBuffer() {
    for (int i = 0; i < ib_channel_count; i++) {
        memset(intermediary_buffer[i].data(), 0, sizeof(float) * ib_buffer_size);
    }
}


template <typename T>
inline void Mach1Encode::encodeBuffer(std::vector< std::vector<T> >& in, std::vector< std::vector<T> >& out, int size) {
    auto encode_gains = getGains(); // TODO: Implement gains interpolation strategies.
    auto output_channel_count = getOutputChannelsCount();

    // process the samples manually
    for (int point_idx = 0; point_idx < getPointsCount(); point_idx++) {
        for (int output_idx = 0; output_idx < output_channel_count; output_idx++){
            for (int sample_idx = 0; sample_idx < size; sample_idx++) {
                auto encode_gain = encode_gains[point_idx][output_idx];
                auto val = in[output_idx][sample_idx];
                out[output_idx][sample_idx] = encode_gain * val;
            }
        }
    }
}

template<typename T>
inline void Mach1Encode::encodeBufferInPlace(std::vector<std::vector<T>> &buffer, size_t buffer_size) {
    encodeBuffer(buffer, buffer, buffer_size);
}

template<typename T>
inline void Mach1Encode::encodeBufferRebuffer(std::vector<std::vector<T>>& in, std::vector<std::vector<T>>& out, int size) {
    auto encode_gains = getGains(); // TODO: Implement gains interpolation strategies.
    auto output_channel_count = getOutputChannelsCount();

    restructureIntermediaryBuffer(output_channel_count, size);
    clearIntermediaryBuffer();

    // process the samples manually
    for (int point_idx = 0; point_idx < getPointsCount(); point_idx++) {
        for (int output_idx = 0; output_idx < output_channel_count; output_idx++){
            for (int sample_idx = 0; sample_idx < size; sample_idx++) {

                auto encode_gain = encode_gains[point_idx][output_idx];
                auto val = in[output_idx][sample_idx];

                intermediary_buffer[output_idx][sample_idx] += encode_gain * val;
            }
        }
    }

    for (int output_idx = 0; output_idx < output_channel_count; output_idx++) {
        memcpy(out[output_idx].data(), intermediary_buffer[output_idx].data(), sizeof(float) * size);
    }

}

template<typename T>
inline void Mach1Encode::encodeBufferInPlaceRebuffer(std::vector<std::vector<T>> &buffer, size_t buffer_size) {
    encodeBufferRebuffer(buffer, buffer, buffer_size);
}

