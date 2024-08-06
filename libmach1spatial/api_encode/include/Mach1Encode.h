//  Mach1 Spatial SDK
//  Copyright © 2017-2020 Mach1. All rights reserved.

#pragma once

#include "Mach1DecodeCAPI.h" // TODO: only include if we build with decode API
#include "Mach1EncodeCAPI.h"
#include <string>
#include <vector>

class Mach1Encode {
  private:
    void *M1obj;
    std::vector<std::vector<float>> gains;

  public:
    Mach1Encode();
    ~Mach1Encode();

    Mach1Encode(const Mach1Encode &other);
    Mach1Encode &operator=(const Mach1Encode &other);

    std::vector<Mach1Point3D> getPoints();
    std::vector<std::vector<float>> getGains();
    std::vector<std::string> getPointsNames();
    std::vector<float> getGainsForInputChannelNamed(std::string pointName);

    void generatePointResults();
    int getPointsCount();

    // TODO: only include if we build with decode API
    std::vector<float> getResultingCoeffsDecoded(Mach1DecodeAlgoType decodeType, std::vector<float> &decodeResult);

    Mach1EncodeInputModeType getInputMode();
    Mach1EncodeOutputModeType getOutputMode();
	Mach1EncodePannerModeType getPannerMode();
	bool getAutoOrbit();
	int getInputChannelsCount();
    int getOutputChannelsCount();

    template <typename T>
    void encodeBuffer(std::vector<std::vector<T>> *inBuffer, std::vector<std::vector<T>> *outBuffer, int bufferSize);

    template <typename T>
    void encodeBuffer(std::vector<T *> *inBuffer, std::vector<T *> *outBuffer, int bufferSize);

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
};

template <typename T>
inline void Mach1Encode::encodeBuffer(std::vector<std::vector<T>> *inBuffer, std::vector<std::vector<T>> *outBuffer, int bufferSize) {
    std::vector<std::vector<float>> _gains = getGains();
    if (this->gains.size() != _gains.size())
        this->gains = _gains;

    T value;
    float prc = 0;
    float gain = 0;
    for (size_t c = 0; c < _gains.size(); c++) {
        for (size_t k = 0; k < _gains[c].size(); k++) {
            T *out = outBuffer->operator[](k * getInputChannelsCount() + c).data();
            T *in = inBuffer->operator[](c).data();

            float startGain = this->gains[c][k];
            float endGain = _gains[c][k];

            for (size_t i = 0; i < bufferSize; i++) {
                prc = 1.0 * i / bufferSize;
                gain = startGain * (1 - prc) + endGain * prc;
                out[i] = in[i] * gain;
            }
        }
    }
    this->gains = _gains;
}

template <typename T>
void Mach1Encode::encodeBuffer(std::vector<T *> *inBuffer, std::vector<T *> *outBuffer, int bufferSize) {
    std::vector<std::vector<float>> _gains = getGains();
    if (this->gains.size() != _gains.size())
        this->gains = _gains;

    T value;
    float prc = 0;
    float gain = 0;
    for (size_t c = 0; c < _gains.size(); c++) {
        for (size_t k = 0; k < _gains[c].size(); k++) {
            T *out = outBuffer->operator[](k * getInputChannelsCount() + c);
            T *in = inBuffer->operator[](c);

            float startGain = this->gains[c][k];
            float endGain = _gains[c][k];

            for (size_t i = 0; i < bufferSize; i++) {
                prc = 1.0 * i / bufferSize;
                gain = startGain * (1 - prc) + endGain * prc;
                out[i] = in[i] * gain;
            }
        }
    }
    this->gains = _gains;
}
