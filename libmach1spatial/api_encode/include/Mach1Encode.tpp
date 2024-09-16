//  Mach1 Spatial SDK
//  Copyright © 2017 Mach1. All rights reserved.

template<typename PCM>
Mach1Encode<PCM>::Mach1Encode() {
    M1obj = Mach1EncodeCAPI_create();
}

template<typename PCM>
Mach1Encode<PCM>::~Mach1Encode() {
    Mach1EncodeCAPI_delete(M1obj);
}

template<typename PCM>
Mach1Encode<PCM>::Mach1Encode(const Mach1Encode<PCM> &other) {
    M1obj = Mach1EncodeCAPI_create();
    Mach1EncodeCAPI_copy(other.M1obj, M1obj);
}

template<typename PCM>
Mach1Encode<PCM> &Mach1Encode<PCM>::operator=(const Mach1Encode<PCM> &other) {
    if (this == &other) {
        return *this;
    }

    Mach1EncodeCAPI_copy(other.M1obj, M1obj);

    return *this;
}

template<typename PCM>
std::vector<Mach1Point3D> Mach1Encode<PCM>::getPoints() {
    std::vector<Mach1Point3D> vec(Mach1EncodeCAPI_getPointsCount(M1obj));

    auto *arr = (Mach1Point3D *)Mach1EncodeCAPI_getPoints(M1obj);
    for (int i = 0; i < vec.size(); i++)
        vec[i] = arr[i];

    return vec;
}

template<typename PCM>
std::vector<std::vector<PCM>> Mach1Encode<PCM>::getGains() {
    std::vector<std::vector<float>> vec(Mach1EncodeCAPI_getPointsCount(M1obj));

    auto **arr = (float **)Mach1EncodeCAPI_getGains(M1obj);

    for (int i = 0; i < vec.size(); i++) {
        vec[i].resize(Mach1EncodeCAPI_getOutputChannelsCount(M1obj));

        for (int j = 0; j < vec[i].size(); j++) {
            vec[i][j] = arr[i][j];
        }
    }

    return vec;
}

template<typename PCM>
std::vector<std::string> Mach1Encode<PCM>::getPointsNames() {
    std::vector<std::string> vec(Mach1EncodeCAPI_getPointsCount(M1obj));

    auto **arr = (char **)Mach1EncodeCAPI_getPointsNames(M1obj);

    for (int i = 0; i < vec.size(); i++) {
        vec[i] = arr[i];
    }

    return vec;
}

template<typename PCM>
std::vector<float> Mach1Encode<PCM>::getGainsForInputChannelNamed(const std::string& pointName) {
    std::vector<float> vec(Mach1EncodeCAPI_getOutputChannelsCount(M1obj));

    auto *arr = (float *)Mach1EncodeCAPI_getGainsForInputChannelNamed(M1obj, (char *)pointName.c_str());

    for (int i = 0; i < vec.size(); i++) {
        vec[i] = arr[i];
    }

    return vec;
}

template<typename PCM>
void Mach1Encode<PCM>::generatePointResults() {
    Mach1EncodeCAPI_generatePointResults(M1obj);
}

template<typename PCM>
int Mach1Encode<PCM>::getPointsCount() {
    return Mach1EncodeCAPI_getPointsCount(M1obj);
}

template<typename PCM>
Mach1EncodeInputMode Mach1Encode<PCM>::getInputMode() {
    return Mach1EncodeCAPI_getInputMode(M1obj);
}

template<typename PCM>
Mach1EncodeOutputMode Mach1Encode<PCM>::getOutputMode() {
    return Mach1EncodeCAPI_getOutputMode(M1obj);
}

template<typename PCM>
Mach1EncodePannerMode Mach1Encode<PCM>::getPannerMode() {
	return Mach1EncodeCAPI_getPannerMode(M1obj);
}

template<typename PCM>
bool Mach1Encode<PCM>::getAutoOrbit() {
	return Mach1EncodeCAPI_getAutoOrbit(M1obj);
}

template<typename PCM>
int Mach1Encode<PCM>::getInputChannelsCount() {
    return Mach1EncodeCAPI_getInputChannelsCount(M1obj);
}

template<typename PCM>
int Mach1Encode<PCM>::getOutputChannelsCount() {
    return Mach1EncodeCAPI_getOutputChannelsCount(M1obj);
}

template<typename PCM>
void Mach1Encode<PCM>::setInputMode(Mach1EncodeInputMode inputMode) {
    Mach1EncodeCAPI_setInputMode(M1obj, inputMode);
}

template<typename PCM>
void Mach1Encode<PCM>::setOutputMode(Mach1EncodeOutputMode outputMode) {
    Mach1EncodeCAPI_setOutputMode(M1obj, outputMode);
}

template<typename PCM>
void Mach1Encode<PCM>::setAzimuth(float azimuthFromMinus1To1) {
    Mach1EncodeCAPI_setAzimuth(M1obj, azimuthFromMinus1To1);
}

template<typename PCM>
void Mach1Encode<PCM>::setAzimuthDegrees(float azimuthDegrees) {
    Mach1EncodeCAPI_setAzimuthDegrees(M1obj, azimuthDegrees);
}

template<typename PCM>
void Mach1Encode<PCM>::setAzimuthRadians(float azimuthRadians) {
    Mach1EncodeCAPI_setAzimuthRadians(M1obj, azimuthRadians);
}

template<typename PCM>
void Mach1Encode<PCM>::setDiverge(float divergeFromMinus1To1) {
    Mach1EncodeCAPI_setDiverge(M1obj, divergeFromMinus1To1);
}

template<typename PCM>
void Mach1Encode<PCM>::setElevation(float elevationFromMinus1to1) {
    Mach1EncodeCAPI_setElevation(M1obj, elevationFromMinus1to1);
}

template<typename PCM>
void Mach1Encode<PCM>::setElevationDegrees(float elevationFromMinus90to90) {
    Mach1EncodeCAPI_setElevationDegrees(M1obj, elevationFromMinus90to90);
}

template<typename PCM>
void Mach1Encode<PCM>::setElevationRadians(float elevationFromMinusHalfPItoHalfPI) {
    Mach1EncodeCAPI_setElevationRadians(M1obj, elevationFromMinusHalfPItoHalfPI);
}

template<typename PCM>
void Mach1Encode<PCM>::setPannerMode(Mach1EncodePannerMode pannerMode) {
    Mach1EncodeCAPI_setPannerMode(M1obj, pannerMode);
}

template<typename PCM>
void Mach1Encode<PCM>::setFrontSurroundPerspective(bool frontSurroundPerspective) {
    Mach1EncodeCAPI_setFrontSurroundPerspective(M1obj, frontSurroundPerspective);
}

template<typename PCM>
void Mach1Encode<PCM>::setOutputGain(float outputGainMultipler, bool isDecibel) {
    Mach1EncodeCAPI_setOutputGain(M1obj, outputGainMultipler, isDecibel);
}

template<typename PCM>
void Mach1Encode<PCM>::setOrbitRotation(float orbitRotationFromMinusOnetoOne) {
    Mach1EncodeCAPI_setOrbitRotation(M1obj, orbitRotationFromMinusOnetoOne);
}

template<typename PCM>
void Mach1Encode<PCM>::setOrbitRotationDegrees(float orbitRotationDegrees) {
    Mach1EncodeCAPI_setOrbitRotationDegrees(M1obj, orbitRotationDegrees);
}

template<typename PCM>
void Mach1Encode<PCM>::setOrbitRotationRadians(float orbitRotationRadians) {
    Mach1EncodeCAPI_setOrbitRotationRadians(M1obj, orbitRotationRadians);
}

template<typename PCM>
void Mach1Encode<PCM>::setStereoSpread(float sSpreadFrom0to1) {
    Mach1EncodeCAPI_setStereoSpread(M1obj, sSpreadFrom0to1);
}

template<typename PCM>
void Mach1Encode<PCM>::setAutoOrbit(bool autoOrbit) {
    Mach1EncodeCAPI_setAutoOrbit(M1obj, autoOrbit);
}

#ifdef M1ENCODE_INLINE_DECODE
template<typename PCM>
std::vector<float> Mach1Encode<PCM>::getResultingCoeffsDecoded(Mach1DecodeMode decodeMode, std::vector<float> &decodeResult) {
    std::vector<float> vec(Mach1EncodeCAPI_getPointsCount(M1obj) * 2);

    float *arr = (float *)Mach1EncodeCAPI_getResultingCoeffsDecoded(M1obj, decodeMode, decodeResult.data());

    for (int i = 0; i < vec.size(); i++) {
        vec[i] = arr[i];
    }

    return vec;
}
#endif


template <typename PCM>
inline void Mach1Encode<PCM>::encodeBuffer(std::vector< std::vector<PCM> >& in, std::vector< std::vector<PCM> >& out, int size) {
    auto encode_gains = getGains();

    if(last_gains.size() != encode_gains.size()) {
        last_gains = encode_gains;
    }

    float size_reciprocal = 1.0f / (float)size;

    // process the samples manually
    for (int point_idx = 0; point_idx < getPointsCount(); point_idx++) {
        for (int output_idx = 0; output_idx < getOutputChannelsCount(); output_idx++){
            auto encode_gain = encode_gains[point_idx][output_idx];
            auto last_encode_gain = last_gains[point_idx][output_idx];

            for (int sample_idx = 0; sample_idx < size; sample_idx++) {
                auto prc = (float)sample_idx * size_reciprocal;
                auto gain = encode_gain * (1.0f - prc) + last_encode_gain * prc;

                auto val = in[output_idx][sample_idx];
                out[output_idx][sample_idx] = gain * val;
            }
        }
    }
}

template<typename PCM>
inline void Mach1Encode<PCM>::encodeBufferInPlace(std::vector<std::vector<PCM>> &buffer, size_t buffer_size) {
    encodeBuffer(buffer, buffer, buffer_size);
}

template<typename PCM>
inline void Mach1Encode<PCM>::encodeBufferRebuffer(std::vector<std::vector<PCM>>& in, std::vector<std::vector<PCM>>& out, int size) {
    auto encode_gains = getGains();
    auto output_channel_count = getOutputChannelsCount();

    if(last_gains.size() != encode_gains.size()) {
        last_gains = encode_gains;
    }

    // restructure intermediary buffer
    if (ib_channel_count != output_channel_count || ib_buffer_size != size) {
        intermediary_buffer.resize(output_channel_count);

        for (int i = 0; i < output_channel_count; i++) {
            intermediary_buffer[i].resize(size);
        }

        ib_channel_count = output_channel_count;
        ib_buffer_size = size;

        for (int i = 0; i < ib_channel_count; i++) {
            memset(intermediary_buffer[i].data(), 0, sizeof(float) * ib_buffer_size);
        }
    }

    // clear intermediary buffer
    for (int i = 0; i < ib_channel_count; i++) {
        memset(intermediary_buffer[i].data(), 0, sizeof(float) * ib_buffer_size);
    }

    float size_reciprocal = 1.0f / (float)size;

    // process the samples manually
    for (int point_idx = 0; point_idx < getPointsCount(); point_idx++) {
        for (int output_idx = 0; output_idx < output_channel_count; output_idx++) {
            auto encode_gain = encode_gains[point_idx][output_idx];
            auto last_encode_gain = last_gains[point_idx][output_idx];

            for (int sample_idx = 0; sample_idx < size; sample_idx++) {
                auto prc = (float)sample_idx * size_reciprocal;
                auto gain = encode_gain * (1.0f - prc) + last_encode_gain * prc;
                auto val = in[output_idx][sample_idx];
                intermediary_buffer[output_idx][sample_idx] += gain * val;
            }
        }
    }

    // copy over values in intermediary buffer to output buffer
    for (int output_idx = 0; output_idx < output_channel_count; output_idx++) {
        memcpy(out[output_idx].data(), intermediary_buffer[output_idx].data(), sizeof(float) * size);
    }

    last_gains = encode_gains;
}

template<typename PCM>
inline void Mach1Encode<PCM>::encodeBufferInPlaceRebuffer(std::vector<std::vector<PCM>> &buffer, size_t buffer_size) {
    encodeBufferRebuffer(buffer, buffer, buffer_size);
}
