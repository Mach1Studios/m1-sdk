//  Mach1 Spatial SDK
//  Copyright © 2017 Mach1. All rights reserved.

template <typename PCM>
Mach1Decode<PCM>::Mach1Decode() {
    M1obj = Mach1DecodeCAPI_create();
}

template <typename PCM>
Mach1Decode<PCM>::~Mach1Decode() {
    Mach1DecodeCAPI_delete(M1obj);
}

template <typename PCM>
void Mach1Decode<PCM>::setPlatformType(Mach1PlatformType type) {
    Mach1DecodeCAPI_setPlatformType(M1obj, type);
}

template <typename PCM>
void Mach1Decode<PCM>::setDecodeMode(Mach1DecodeMode mode) {
    Mach1DecodeCAPI_setDecodeMode(M1obj, mode);
}

#ifndef __EMSCRIPTEN__
template <typename PCM>
void Mach1Decode<PCM>::decode(float Yaw, float Pitch, float Roll, float *result, int bufferSize, int sampleIndex) {
    Mach1DecodeCAPI_decode(M1obj, Yaw, Pitch, Roll, result, bufferSize, sampleIndex);
}

template <typename PCM>
void Mach1Decode<PCM>::decodeCoeffs(float *result, int bufferSize, int sampleIndex) {
    Mach1DecodeCAPI_decodeCoeffs(M1obj, result, bufferSize, sampleIndex);
}

template <typename PCM>
void Mach1Decode<PCM>::decodePannedCoeffs(float *result, int bufferSize, int sampleIndex, bool applyPanLaw) {
    Mach1DecodeCAPI_decodePannedCoeffs(M1obj, result, bufferSize, sampleIndex, applyPanLaw);
}
#endif

template <typename PCM>
std::vector<PCM> Mach1Decode<PCM>::decode(float Yaw, float Pitch, float Roll, int bufferSize, int sampleIndex) {
    std::vector<PCM> vec(getFormatCoeffCount());

    Mach1DecodeCAPI_decode(M1obj, Yaw, Pitch, Roll, vec.data(), bufferSize, sampleIndex);

    return vec;
}

template <typename PCM>
std::vector<PCM> Mach1Decode<PCM>::decodeCoeffs(int bufferSize, int sampleIndex) {
    std::vector<PCM> vec(getFormatCoeffCount());

    Mach1DecodeCAPI_decodeCoeffs(M1obj, vec.data(), bufferSize, sampleIndex);

    return vec;
}

template <typename PCM>
std::vector<PCM> Mach1Decode<PCM>::decodePannedCoeffs(int bufferSize, int sampleIndex, bool applyPanLaw) {
    std::vector<PCM> vec(getFormatCoeffCount());

    Mach1DecodeCAPI_decodePannedCoeffs(M1obj, vec.data(), bufferSize, sampleIndex, applyPanLaw);

    return vec;
}

template <typename PCM>
std::vector<PCM> Mach1Decode<PCM>::decodeCoeffsUsingTranscodeMatrix(std::vector< std::vector<float> > matrix, int channels, int bufferSize, int sampleIndex) {
    std::vector<PCM> vec(2 * channels);

    int inChans = channels;
    int outChans = getFormatChannelCount();

    float *m = new float[inChans * outChans];
    for (int i = 0; i < outChans; i++) {
        for (int j = 0; j < inChans; j++) {
            m[i * inChans + j] = matrix[i][j];
        }
    }

    Mach1DecodeCAPI_decodeCoeffsUsingTranscodeMatrix(M1obj, m, channels, vec.data(), bufferSize, sampleIndex);

    delete[] m;
    return vec;
}

template <typename PCM>
int Mach1Decode<PCM>::getFormatChannelCount() {
    return Mach1DecodeCAPI_getFormatChannelCount(M1obj);
}

template <typename PCM>
int Mach1Decode<PCM>::getFormatCoeffCount() {
    return Mach1DecodeCAPI_getFormatCoeffCount(M1obj);
}

template <typename PCM>
void Mach1Decode<PCM>::setRotation(Mach1Point3D newRotationFromMinusOnetoOne) {
    Mach1DecodeCAPI_setRotation(M1obj, newRotationFromMinusOnetoOne);
}

template <typename PCM>
void Mach1Decode<PCM>::setRotationDegrees(Mach1Point3D newRotationDegrees) {
    Mach1DecodeCAPI_setRotationDegrees(M1obj, newRotationDegrees);
}

template <typename PCM>
void Mach1Decode<PCM>::setRotationRadians(Mach1Point3D newRotationRadians) {
    Mach1DecodeCAPI_setRotationRadians(M1obj, newRotationRadians);
}

template <typename PCM>
void Mach1Decode<PCM>::setRotationQuat(Mach1Point4D newRotationQuat) {
    Mach1DecodeCAPI_setRotationQuat(M1obj, newRotationQuat);
}

template <typename PCM>
void Mach1Decode<PCM>::setFilterSpeed(float filterSpeed) {
    Mach1DecodeCAPI_setFilterSpeed(M1obj, filterSpeed);
}

template <typename PCM>
long Mach1Decode<PCM>::getCurrentTime() {
    return Mach1DecodeCAPI_getCurrentTime(M1obj);
}

#ifndef __EMSCRIPTEN__
template <typename PCM>
char *Mach1Decode<PCM>::getLog() {
    return Mach1DecodeCAPI_getLog(M1obj);
}
#else
template <typename PCM>
std::string Mach1Decode<PCM>::getLog() {
    return std::string(Mach1DecodeCAPI_getLog(M1obj));
    /// Returns the current elapsed time in milliseconds (ms) since Mach1Decode object creation
}
#endif

template <typename PCM>
Mach1Point3D Mach1Decode<PCM>::getCurrentAngle() {
    return Mach1DecodeCAPI_getCurrentAngle(M1obj);
}


template <typename PCM>
void Mach1Decode<PCM>::decodeBuffer(std::vector< std::vector<PCM> > &in, std::vector< std::vector<PCM> > &out, int size) {
    // get output gain multipliers
    auto decode_gains = decodeCoeffs(); // TODO: Implement interpolation between coeffs.

    if (old_decode_gains.size() != decode_gains.size()) {
        old_decode_gains = decode_gains;
    }

    float size_reciprocal = 1.0f / (float) size;

    // process the samples manually
    for (int decode_idx = 0, output_idx = 0; decode_idx < decode_gains.size(); decode_idx += 2, output_idx += 1) {
        auto left_gain = decode_gains[decode_idx + 0];
        auto right_gain = decode_gains[decode_idx + 1];
        auto old_left_gain  = old_decode_gains[decode_idx + 0];
        auto old_right_gain = old_decode_gains[decode_idx + 1];

        for (int sample_idx = 0; sample_idx < size; sample_idx++) {
            auto prc = (float)sample_idx * size_reciprocal;
            auto left = left_gain * (1.0f - prc) + old_left_gain * prc;
            auto right = right_gain * (1.0f - prc) + old_right_gain * prc;

            // get the sample in each loop
            float sample = in[output_idx][sample_idx];

            // clear the output for the new values to come in
            out[output_idx][sample_idx] = 0;
            out[0][sample_idx] += sample * left;
            out[1][sample_idx] += sample * right;
        }
    }

    old_decode_gains = decode_gains;

}

template <typename PCM>
void Mach1Decode<PCM>::decodeBufferInPlace(std::vector<std::vector<PCM>> &buffer, int size) {
    decodeBuffer(buffer, buffer, size);
}

template <typename PCM>
void Mach1Decode<PCM>::decodeBufferRebuffer(std::vector<std::vector<PCM>> &in, std::vector<std::vector<PCM>> &out, int size)
{
    auto channel_count = 2;

    // restructure intermediary buffer
    if (ib_channel_count != channel_count || ib_buffer_size != size) {
        intermediary_buffer.resize(channel_count);

        for (int i = 0; i < channel_count; i++) {
            intermediary_buffer[i].resize(size);
        }

        ib_channel_count = channel_count;
        ib_buffer_size = size;
    }

    // clear intermediary buffer
    for (int i = 0; i < ib_channel_count; i++) {
        memset(intermediary_buffer[i].data(), 0, sizeof(float) * ib_buffer_size);
    }

    // get output gain multipliers
    auto decode_gains = decodeCoeffs(); // TODO: Implement interpolation between coeffs.

    if (old_decode_gains.size() != decode_gains.size()) {
        old_decode_gains = decode_gains;
    }

    float size_reciprocal = 1.0f / (float) size;

    // process the samples manually
    for (int decode_idx = 0, output_idx = 0; decode_idx < decode_gains.size(); decode_idx += 2, output_idx += 1) {
        auto left_gain = decode_gains[decode_idx + 0];
        auto right_gain = decode_gains[decode_idx + 1];
        auto old_left_gain  = old_decode_gains[decode_idx + 0];
        auto old_right_gain = old_decode_gains[decode_idx + 1];

        for (int sample_idx = 0; sample_idx < size; sample_idx++) {
            auto prc = (float)sample_idx * size_reciprocal;
            auto left = left_gain * (1.0f - prc) + old_left_gain * prc;
            auto right = right_gain * (1.0f - prc) + old_right_gain * prc;

            // get the sample in each loop
            float sample = in[output_idx][sample_idx];

            intermediary_buffer[0][sample_idx] += sample * left;
            intermediary_buffer[1][sample_idx] += sample * right;
        }
    }

    // copy over values in intermediary buffer to output buffer
    for (int output_idx = 0; output_idx < channel_count; output_idx++) {
        memcpy(out[output_idx].data(), intermediary_buffer[output_idx].data(), sizeof(float) * size);
    }

    old_decode_gains = decode_gains;

}

template <typename PCM>
void Mach1Decode<PCM>::decodeBufferInPlaceRebuffer(std::vector< std::vector<PCM> > &buffer, int size)
{
    decodeBufferRebuffer(buffer, buffer, size);
}
