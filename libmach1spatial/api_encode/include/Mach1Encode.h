//  Mach1 Spatial SDK
//  Copyright © 2017 Mach1. All rights reserved.

#ifndef MACH1SPATIALSDK_MACH1ENCODE_H
#define MACH1SPATIALSDK_MACH1ENCODE_H

#include <Mach1Point3D.h>
#include <cstring>
#include <string>
#include <vector>

#include "Mach1EncodeCAPI.h"

#ifdef M1ENCODE_INLINE_DECODE
#    include "Mach1DecodeCAPI.h"
#endif

template <typename PCM>
class Mach1Encode {
  public:
    Mach1Encode();
    Mach1Encode(const Mach1Encode &other);
    Mach1Encode &operator=(const Mach1Encode &other);
    Mach1Encode(Mach1Encode&& other) noexcept;
    Mach1Encode& operator=(Mach1Encode&& other) noexcept;
    ~Mach1Encode();

    /**
     * @brief Returns the control center reference point's normalized coordinate location (XYZ)
     * within the vector panning space.
     * @returns Vector of Mach1Point3Ds, where the components of each point are as follows:
     * <ul>
     *      <li> X: front-back (0.0 (back) -> 1.0 (front)) </li>
     *      <li> Y: up-down (0.0 (down) -> 1.0 (up)) </li>
     *      <li> Z: left-right (0.0 (left) -> 1.0 (right)) </li>
     * </ul>
     */
    std::vector<Mach1Point3D> getPoints();

    /**
     * @brief Returns an array per input channel, resulting in an array of array results
     * each internal array (per input channel) is a list of the needed coefficients to
     * encode to a Mach1 VVBP/SPS format (depending on the selected output mode).
     * @return A 2D vector, whose layout is as follows:
     * <ul>
     *      <li> 1st dimension of array is the number of input channels/points </li>
     *      <li> 2nd dimension of array is the resulting coefficient gains to be applied for encode </li>
     * </ul>
     */
    std::vector<std::vector<PCM> > getGains();

    /**
     * @brief Return the string name for each input channel/point.
     */
    std::vector<std::string> getPointsNames();

    /**
     * @brief Given the name of an input channel/point, return array of gain coefficients.
     * @param pointName The name of the requested point/channel.
     * @return Vector of N gain coefficients, where N is the amount channels that this
     * Mach1Encode object is configured to output to.
     */
    std::vector<float> getGainsForInputChannelNamed(const std::string &pointName);

    /**
     * @brief Recalculate and update the encoder with the currently set spatial configuration.
     * @note Call when an update to Mach1 vector calculations is desired.
     */
    void generatePointResults();

    /**
     * @brief Get the number of point results that have been generated after a call to generatePointResults.
     */
    int getPointsCount();

    /**
     * @brief Get the format in which this Mach1Encode expects to receive audio for encoding.
     */
    Mach1EncodeInputMode getInputMode();

    /**
     * Set the format in which this Mach1Encode expects to receive audio for encoding.
     */
    void setInputMode(Mach1EncodeInputMode inputMode);

    /**
     * @brief Get the format in which this Mach1Encode will output encoded audio.
     */
    Mach1EncodeOutputMode getOutputMode();

    /**
     * @brief Set the format in which this Mach1Encode will output encoded audio.
     */
    void setOutputMode(Mach1EncodeOutputMode outputMode);

    /**
     * @brief Get the calculation strategy by which the points in this Mach1Encode are calculated.
     *
     * Panner modes govern how Mach1Encode distributes your input signal(s) across the Mach1
     * output points.
     *
     * @return Which Mach1EncodePannerMode the Mach1Encode instance is currently using.
     */
    Mach1EncodePannerMode getPannerMode();

    /**
     * @brief Sets the style and mode of panner input calculation.
     *
     * Panner modes govern how Mach1Encode distributes your input signal(s) across the Mach1
     * output points. The following modes exist:
     *
     * - MODE_ISOTROPICLINEAR:
     *   Distributes signal without prioritizing any particular direction. More natural expectations of
     *   signal distribution.
     *
     * - MODE_ISOTROPICEQUALPOWER:
     *   Uses an equal-power distribution approach for smoother panning and also applies an additional
     *   gain multiplier based on the diverge value to the center point.
     *
     * - MODE_PERIPHONICLINEAR:
     *   Naturally distributes signal horizontally but crossfades between channels vertically.
     *
     * @param pannerMode The Mach1EncodePannerMode to use for calculation.
     */
    void setPannerMode(enum Mach1EncodePannerMode pannerMode);

    /**
     * @brief Get whether auto-orbit mode is enabled. If true, then orbit rotation is extrapolated from
     * the currently set azimuth.
     */
    bool getAutoOrbit();

    /**
     * @brief Get the number of input channels that this Mach1Encode expects to receive for encoding,
     * depending on the selected input mode.
     */
    int getInputChannelsCount();

    /**
     * @brief Get the number of output channels that this Mach1Encode expects to receive for encoding,
     * depending on the selected output mode.
     */
    int getOutputChannelsCount();

    /**
     * @brief Sets the point(s) azimuth rotation of the vector space.
     * @param azimuthFromMinus1To1 floating point value in the range [-1.0; 1.0]
     */
    void setAzimuth(float azimuthFromMinus1To1);

    /**
     * @brief Sets the point(s) azimuth rotation of the vector space.
     * @param azimuthDegrees floating point value in the range [0.0; 360.0]
     */
    void setAzimuthDegrees(float azimuthDegrees);

    /**
     * @brief Sets the point(s) azimuth rotation of the vector space.
     * @param azimuthRadians floating point value in the range [-PI; PI]
     */
    void setAzimuthRadians(float azimuthRadians);

    /**
     * @brief Sets the point(s) up/down the vector space
     * @param elevationFromMinus1to1 floating point value in the range [-1.0; 1.0]
     */
    void setElevation(float elevationFromMinus1to1);

    /**
     * @brief Sets the point(s) up/down the vector space
     * @param elevationFromMinus90to90 floating point value in the range [-90.0; 90.0]
     */
    void setElevationDegrees(float elevationFromMinus90to90);

    /**
     * @brief Sets the point(s) up/down the vector space
     * @param elevationFromMinus90to90 floating point value in the range [-PI / 2.0; PI / 2.0]
     */
    void setElevationRadians(float elevationFromMinusHalfPItoHalfPI);

    /**
     * Sets the encoding behavior of the Center input channels of relevant Surround format, when true the
     * encoding behavior assumes first person perspective encoding Center channels toward the front of the
     * sound field, when false we use Center channel literally making it encoded MONO and omni-directional.
     * @note Is true by default upon the construction of a new Mach1Encode instance.
     */
    void setFrontSurroundPerspective(bool frontSurroundPerspective);

    /**
     * Gets the current global gain multiplier which is applied to all output channels equally.
     * Can be calculated with a linear amplitude multiplier or a decibel multiplier.
     * @note +6dB gain is applied by default upon the construction of a new Mach1Encode instance.
     */
    float getOutputGain(bool isDecibel);

    /**
     * Applies an additional gain multiplier to all output channels equally.
     * Can be calculated with a linear amplitude multiplier or a decibel multiplier.
     * @note +6dB gain is applied by default upon the construction of a new Mach1Encode instance.
     */
    void setOutputGain(float outputGainMultipler, bool isDecibel);

    /**
     * @brief Gets the current gain compensation value.
     * @param isDecibel true if the gain compensation is in decibels, false to return as a linear multiplier
     */
    float getGainCompensation(bool isDecibel);

    /**
     * @brief Gets whether gain compensation is active.
     */
    bool getGainCompensationActive();

    /**
     * @brief Sets whether gain compensation is active.
     * @param active true if gain compensation is active, false if it is not
     * 
     * Gain compensation is an automatic adjustment to the gain of the output channels 
     * to compensate for the gain loss on Mach1Decode side based on the number of output channels of Mach1Encode. 
     * This can be useful for ensuring that Mach1Encode instances have the same gain structure when setting a new output mode.
     * 
     * @note Is true by default upon the construction of a new Mach1Encode instance.
     */
    void setGainCompensationActive(bool active);

    /**
     * @brief Sets the two stereo points around the axis of the center point between them
     * @param orbitRotationFromMinusOnetoOne floating point value in the range [-1.0; 1.0]
     */
    void setOrbitRotation(float orbitRotationFromMinusOnetoOne);

    /**
     * @brief Sets the two stereo points around the axis of the center point between them
     * @param orbitRotationDegrees floating point value in the range [-180.0; 180.0]
     */
    void setOrbitRotationDegrees(float orbitRotationDegrees);

    /**
     * @brief Sets the two stereo points around the axis of the center point between them
     * @param orbitRotationRadians floating point value in the range [-PI -> PI]
     */
    void setOrbitRotationRadians(float orbitRotationRadians);

    /**
     * @brief Sets the point(s) to/from center origin of the vector space.
     * @param divergeFromMinus1To1 floating point value between -1.0 and 1.0
     */
    void setDiverge(float divergeFromMinus1To1);

    /**
     * @brief Sets the space between the two stereo points
     * @param sSpreadFrom0to1 floating point value between 0.0 and 1.0
     */
    void setStereoSpread(float sSpreadFrom0to1);

    /**
     * @brief Sets whether encoding behavior acts evenly with distribution across all azimuth/rotation angles
     * and all altitude/pitch angles.
     * @note Is true by default upon the construction of a new Mach1Encode instance.
     */
    void setAutoOrbit(bool autoOrbit);

    /**
     * Encode the input buffers and emplace the result into the specified output buffers.
     * Each channel of the buffers are expected to be of the specified size.
     * Processing is performed by directly modifying the output buffer for every iteration of the algorithm.
     */
    inline void encodeBuffer(std::vector<std::vector<PCM> > &in, std::vector<std::vector<PCM> > &out, int size);

    /**
     * Encode the input buffers and emplace the result into the specified output buffers.
     * Each channel of the buffers are expected to be of the specified size.
     * Processing is performed by storing the result of the computation in an intermediary buffer
     * and then copying over the result from the intermediary buffer into the output buffer.
     */
    inline void encodeBufferRebuffer(std::vector<std::vector<PCM> > &in, std::vector<std::vector<PCM> > &out, int size);

    /**
     * Encode the specified buffer in-place.
     * Each channel of the buffer is expected to be of the specified size.
     * Processing is performed by directly modifying the buffer for every iteration of the algorithm.
     */
    inline void encodeBufferInPlaceRebuffer(std::vector<std::vector<PCM> > &buffer, size_t buffer_size);

    /**
     * Encode the specified buffer in-place.
     * Each channel of the buffer is expected to be of the specified size.
     * Processing is performed by directly modifying the buffer for every iteration of the algorithm.
     */
    inline void encodeBufferInPlace(std::vector<std::vector<PCM> > &buffer, size_t buffer_size);

#ifdef M1ENCODE_INLINE_DECODE
    /**
     * @brief A shorthand function for encoding->decoding audio object handling,
     * useful preview UX so that a full input->mach1spatial_multichannel->stereo
     * rendering to disk isn't required and instead designs that stack decode results
     * live can more easily be created.
     * @note: Each input audio channel results a direct decode instead of the encode coefficients
     */
    std::vector<float> getResultingCoeffsDecoded(Mach1DecodeMode decodeMode, std::vector<float> &decodeResult);
#endif

  private:
    void *M1obj;

    std::vector<std::vector<PCM> > last_gains;
    std::vector<std::vector<PCM> > intermediary_buffer;

    int ib_channel_count{};
    size_t ib_buffer_size{};
};

#endif // MACH1SPATIALSDK_MACH1ENCODE_H

// Implementation of Mach1Encode<PCM>
#ifndef MACH1SPATIALSDK_MACH1ENCODE_TPP
#define MACH1SPATIALSDK_MACH1ENCODE_TPP

template <typename PCM>
Mach1Encode<PCM>::Mach1Encode() {
    M1obj = Mach1EncodeCAPI_create();
}

template <typename PCM>
Mach1Encode<PCM>::~Mach1Encode() {
    Mach1EncodeCAPI_delete(M1obj);
}

template <typename PCM>
Mach1Encode<PCM>::Mach1Encode(const Mach1Encode<PCM> &other) {
    M1obj = Mach1EncodeCAPI_create();
    Mach1EncodeCAPI_copy(other.M1obj, M1obj);
}

template <typename PCM>
Mach1Encode<PCM>& Mach1Encode<PCM>::operator=(const Mach1Encode<PCM>& other) {
    if (this != &other) {
        // Delete current M1obj if it's not nullptr
        if (M1obj != nullptr) {
            Mach1EncodeCAPI_delete(M1obj);
            M1obj = nullptr;
        }

        // Create a new M1obj
        M1obj = Mach1EncodeCAPI_create();

        // Copy from other.M1obj into M1obj
        Mach1EncodeCAPI_copy(other.M1obj, M1obj);
    }
    return *this;
}

template <typename PCM>
Mach1Encode<PCM>::Mach1Encode(Mach1Encode&& other) noexcept {
    M1obj = other.M1obj;
    other.M1obj = nullptr;
}

template <typename PCM>
Mach1Encode<PCM>& Mach1Encode<PCM>::operator=(Mach1Encode&& other) noexcept {
    if (this != &other) {
        // Delete current M1obj
        if (M1obj != nullptr) {
            Mach1EncodeCAPI_delete(M1obj);
        }

        // Transfer ownership
        M1obj = other.M1obj;
        other.M1obj = nullptr;
    }
    return *this;
}

template <typename PCM>
std::vector<Mach1Point3D> Mach1Encode<PCM>::getPoints() {
    std::vector<Mach1Point3D> vec(Mach1EncodeCAPI_getPointsCount(M1obj));

    auto *arr = (Mach1Point3D *)Mach1EncodeCAPI_getPoints(M1obj);
    for (int i = 0; i < vec.size(); i++)
        vec[i] = arr[i];

    return vec;
}

template <typename PCM>
std::vector<std::vector<PCM> > Mach1Encode<PCM>::getGains() {
    std::vector<std::vector<float> > vec(Mach1EncodeCAPI_getPointsCount(M1obj));

    auto **arr = (float **)Mach1EncodeCAPI_getGains(M1obj);

    for (int i = 0; i < vec.size(); i++) {
        vec[i].resize(Mach1EncodeCAPI_getOutputChannelsCount(M1obj));

        for (int j = 0; j < vec[i].size(); j++) {
            vec[i][j] = arr[i][j];
        }
    }

    return vec;
}

template <typename PCM>
std::vector<std::string> Mach1Encode<PCM>::getPointsNames() {
    std::vector<std::string> vec(Mach1EncodeCAPI_getPointsCount(M1obj));

    auto **arr = (char **)Mach1EncodeCAPI_getPointsNames(M1obj);

    for (int i = 0; i < vec.size(); i++) {
        vec[i] = arr[i];
    }

    return vec;
}

template <typename PCM>
std::vector<float> Mach1Encode<PCM>::getGainsForInputChannelNamed(const std::string &pointName) {
    std::vector<float> vec(Mach1EncodeCAPI_getOutputChannelsCount(M1obj));

    auto *arr = (float *)Mach1EncodeCAPI_getGainsForInputChannelNamed(M1obj, (char *)pointName.c_str());

    for (int i = 0; i < vec.size(); i++) {
        vec[i] = arr[i];
    }

    return vec;
}

template <typename PCM>
void Mach1Encode<PCM>::generatePointResults() {
    Mach1EncodeCAPI_generatePointResults(M1obj);
}

template <typename PCM>
int Mach1Encode<PCM>::getPointsCount() {
    return Mach1EncodeCAPI_getPointsCount(M1obj);
}

template <typename PCM>
Mach1EncodeInputMode Mach1Encode<PCM>::getInputMode() {
    return Mach1EncodeCAPI_getInputMode(M1obj);
}

template <typename PCM>
Mach1EncodeOutputMode Mach1Encode<PCM>::getOutputMode() {
    return Mach1EncodeCAPI_getOutputMode(M1obj);
}

template <typename PCM>
Mach1EncodePannerMode Mach1Encode<PCM>::getPannerMode() {
    return Mach1EncodeCAPI_getPannerMode(M1obj);
}

template <typename PCM>
bool Mach1Encode<PCM>::getAutoOrbit() {
    return Mach1EncodeCAPI_getAutoOrbit(M1obj);
}

template <typename PCM>
int Mach1Encode<PCM>::getInputChannelsCount() {
    return Mach1EncodeCAPI_getInputChannelsCount(M1obj);
}

template <typename PCM>
int Mach1Encode<PCM>::getOutputChannelsCount() {
    return Mach1EncodeCAPI_getOutputChannelsCount(M1obj);
}

template <typename PCM>
void Mach1Encode<PCM>::setInputMode(Mach1EncodeInputMode inputMode) {
    Mach1EncodeCAPI_setInputMode(M1obj, inputMode);
}

template <typename PCM>
void Mach1Encode<PCM>::setOutputMode(Mach1EncodeOutputMode outputMode) {
    Mach1EncodeCAPI_setOutputMode(M1obj, outputMode);
}

template <typename PCM>
void Mach1Encode<PCM>::setAzimuth(float azimuthFromMinus1To1) {
    Mach1EncodeCAPI_setAzimuth(M1obj, azimuthFromMinus1To1);
}

template <typename PCM>
void Mach1Encode<PCM>::setAzimuthDegrees(float azimuthDegrees) {
    Mach1EncodeCAPI_setAzimuthDegrees(M1obj, azimuthDegrees);
}

template <typename PCM>
void Mach1Encode<PCM>::setAzimuthRadians(float azimuthRadians) {
    Mach1EncodeCAPI_setAzimuthRadians(M1obj, azimuthRadians);
}

template <typename PCM>
void Mach1Encode<PCM>::setDiverge(float divergeFromMinus1To1) {
    Mach1EncodeCAPI_setDiverge(M1obj, divergeFromMinus1To1);
}

template <typename PCM>
void Mach1Encode<PCM>::setElevation(float elevationFromMinus1to1) {
    Mach1EncodeCAPI_setElevation(M1obj, elevationFromMinus1to1);
}

template <typename PCM>
void Mach1Encode<PCM>::setElevationDegrees(float elevationFromMinus90to90) {
    Mach1EncodeCAPI_setElevationDegrees(M1obj, elevationFromMinus90to90);
}

template <typename PCM>
void Mach1Encode<PCM>::setElevationRadians(float elevationFromMinusHalfPItoHalfPI) {
    Mach1EncodeCAPI_setElevationRadians(M1obj, elevationFromMinusHalfPItoHalfPI);
}

template <typename PCM>
void Mach1Encode<PCM>::setPannerMode(Mach1EncodePannerMode pannerMode) {
    Mach1EncodeCAPI_setPannerMode(M1obj, pannerMode);
}

template <typename PCM>
void Mach1Encode<PCM>::setFrontSurroundPerspective(bool frontSurroundPerspective) {
    Mach1EncodeCAPI_setFrontSurroundPerspective(M1obj, frontSurroundPerspective);
}

template <typename PCM>
float Mach1Encode<PCM>::getOutputGain(bool isDecibel) {
    Mach1EncodeCAPI_getOutputGain(M1obj, isDecibel);
}

template <typename PCM>
void Mach1Encode<PCM>::setOutputGain(float outputGainMultipler, bool isDecibel) {
    Mach1EncodeCAPI_setOutputGain(M1obj, outputGainMultipler, isDecibel);
}

template <typename PCM>
float Mach1Encode<PCM>::getGainCompensation(bool isDecibel) {
    return Mach1EncodeCAPI_getGainCompensation(M1obj, isDecibel);
}

template <typename PCM>
bool Mach1Encode<PCM>::getGainCompensationActive() {
    return Mach1EncodeCAPI_getGainCompensationActive(M1obj);
}

template <typename PCM>
void Mach1Encode<PCM>::setGainCompensationActive(bool active) {
    Mach1EncodeCAPI_setGainCompensationActive(M1obj, active);
}

template <typename PCM>
void Mach1Encode<PCM>::setOrbitRotation(float orbitRotationFromMinusOnetoOne) {
    Mach1EncodeCAPI_setOrbitRotation(M1obj, orbitRotationFromMinusOnetoOne);
}

template <typename PCM>
void Mach1Encode<PCM>::setOrbitRotationDegrees(float orbitRotationDegrees) {
    Mach1EncodeCAPI_setOrbitRotationDegrees(M1obj, orbitRotationDegrees);
}

template <typename PCM>
void Mach1Encode<PCM>::setOrbitRotationRadians(float orbitRotationRadians) {
    Mach1EncodeCAPI_setOrbitRotationRadians(M1obj, orbitRotationRadians);
}

template <typename PCM>
void Mach1Encode<PCM>::setStereoSpread(float sSpreadFrom0to1) {
    Mach1EncodeCAPI_setStereoSpread(M1obj, sSpreadFrom0to1);
}

template <typename PCM>
void Mach1Encode<PCM>::setAutoOrbit(bool autoOrbit) {
    Mach1EncodeCAPI_setAutoOrbit(M1obj, autoOrbit);
}

#ifdef M1ENCODE_INLINE_DECODE
template <typename PCM>
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
inline void
Mach1Encode<PCM>::encodeBuffer(std::vector<std::vector<PCM> > &in, std::vector<std::vector<PCM> > &out, int size) {
    auto encode_gains = getGains();

    if (last_gains.size() != encode_gains.size()) {
        last_gains = encode_gains;
    }

    float size_reciprocal = 1.0f / (float)size;

    // process the samples manually
    for (int point_idx = 0; point_idx < getPointsCount(); point_idx++) {
        for (int output_idx = 0; output_idx < getOutputChannelsCount(); output_idx++) {
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

template <typename PCM>
inline void Mach1Encode<PCM>::encodeBufferInPlace(std::vector<std::vector<PCM> > &buffer, size_t buffer_size) {
    encodeBuffer(buffer, buffer, buffer_size);
}

template <typename PCM>
inline void
Mach1Encode<PCM>::encodeBufferRebuffer(std::vector<std::vector<PCM> > &in, std::vector<std::vector<PCM> > &out,
                                       int size) {
    auto encode_gains = getGains();
    auto output_channel_count = getOutputChannelsCount();

    if (last_gains.size() != encode_gains.size()) {
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

template <typename PCM>
inline void Mach1Encode<PCM>::encodeBufferInPlaceRebuffer(std::vector<std::vector<PCM> > &buffer, size_t buffer_size) {
    encodeBufferRebuffer(buffer, buffer, buffer_size);
}

#endif // MACH1SPATIALSDK_MACH1ENCODE_TPP
