//  Mach1 Spatial SDK
//  Copyright © 2017 Mach1. All rights reserved.

#pragma once

#include <string>
#include <cstring>
#include <string>
#include <vector>
#include <Mach1Point3D.h>

#include "Mach1EncodeCAPI.h"

#ifdef M1ENCODE_INLINE_DECODE
#include "Mach1DecodeCAPI.h"
#endif

template <typename PCM>
class Mach1Encode {
public:
    Mach1Encode();
    Mach1Encode(const Mach1Encode &other);

    Mach1Encode& operator=(const Mach1Encode &other);
    Mach1Encode(Mach1Encode&& other) = default; // TODO: Implement
    Mach1Encode& operator=(Mach1Encode&& other) = default; // TODO: Implement

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
    std::vector< std::vector<PCM> > getGains();

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
    std::vector<float> getGainsForInputChannelNamed(const std::string& pointName);

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
     * @brief Get the strategy by which to position the points in this Mach1Encode.
     */
	Mach1EncodePannerMode getPannerMode();

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
     * @brief Sets the style and mode of panner input calculation
     */
    void setPannerMode(enum Mach1EncodePannerMode pannerMode);

    /**
     * Sets the encoding behavior of the Center input channels of relevant Surround format, when true the
     * encoding behavior assumes first person perspective encoding Center channels toward the front of the
     * sound field, when false we use Center channel literally making it encoded MONO and omni-directional.
     * @note Is true by default upon the construction of a new Mach1Encode instance.
     */
    void setFrontSurroundPerspective(bool frontSurroundPerspective);

    /**
     * Applies an additional gain multiplier to all output channels equally.
     * Can be calculated with a linear amplitude multiplier or a decibel multiplier.
     * @note No gain is applied by default upon the construction of a new Mach1Encode instance.
     */
    void setOutputGain(float outputGainMultipler, bool isDecibel);

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
    inline void encodeBuffer(std::vector< std::vector<PCM> >& in, std::vector< std::vector<PCM> >& out, int size);

    /**
     * Encode the input buffers and emplace the result into the specified output buffers.
     * Each channel of the buffers are expected to be of the specified size.
     * Processing is performed by storing the result of the computation in an intermediary buffer
     * and then copying over the result from the intermediary buffer into the output buffer.
     */
    inline void encodeBufferRebuffer(std::vector< std::vector<PCM> >& in, std::vector< std::vector<PCM> >& out, int size);

    /**
     * Encode the specified buffer in-place.
     * Each channel of the buffer is expected to be of the specified size.
     * Processing is performed by directly modifying the buffer for every iteration of the algorithm.
     */
    inline void encodeBufferInPlaceRebuffer(std::vector<std::vector<PCM>>& buffer, size_t buffer_size);

    /**
     * Encode the specified buffer in-place.
     * Each channel of the buffer is expected to be of the specified size.
     * Processing is performed by directly modifying the buffer for every iteration of the algorithm.
     */
    inline void encodeBufferInPlace(std::vector<std::vector<PCM>>& buffer, size_t buffer_size);

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

    std::vector< std::vector<PCM> > last_gains;
    std::vector< std::vector<PCM> > intermediary_buffer;

    int ib_channel_count{};
    size_t ib_buffer_size{};
};

#include "Mach1Encode.tpp" // templated implementation of Mach1Encode
