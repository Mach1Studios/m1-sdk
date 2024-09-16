//  Mach1 Spatial SDK
//  Copyright © 2017 Mach1. All rights reserved.

#pragma once

#include "Mach1DecodeCAPI.h"
#include <string>
#include <cstring>
#include <vector>

template <typename PCM>
class Mach1Decode {
  public:
    Mach1Decode();

    ~Mach1Decode();

    /**
     * @brief Set the device's angle order and convention if applicable.
     */
    void setPlatformType(Mach1PlatformType type);

    /**
     * @brief Set the decoding mode.
     */
    void setDecodeMode(Mach1DecodeMode mode);

    /**
     * @brief Get the get amount of channels that this Mach1Decode expects to decode, based on the
     * currently active decoding mode.
     */
    int getFormatChannelCount();

    /**
     * @brief Get the get amount of decoding coefficients this Mach1Decode will generate, based on the
     * currently active decoding algorithm.
     */
    int getFormatCoeffCount();

    /**
     * @brief Set current buffer/sample intended decoding orientation YPR.
     * @param newRotationFromMinusOnetoOne
     * <ul>
     *      <li> Yaw:   [Range: 0.0 -> 1.0 | -0.5 -> 0.5];  [Range: 0.0 -> 360 | -180 -> 180] </li>
     *      <li> Pitch: [Range: -0.25 -> 0.25];             [Range: -90 -> 90] </li>
     *      <li> Roll:  [Range: -0.25 -> 0.25];             [Range: -90 -> 90] </li>
     * </ul>
     */
    void setRotation(Mach1Point3D newRotationFromMinusOnetoOne);

    /**
     * @brief Set current buffer/sample intended decoding orientation YPR.
     * @param newRotationFromMinusOnetoOne
     * <ul>
     *      <li> Yaw:   [Range: 0.0 -> 360 | -180 -> 180] </li>
     *      <li> Pitch: [Range: -90 -> 90] </li>
     *      <li> Roll:  [Range: -90 -> 90] </li>
     * </ul>
     */
    void setRotationDegrees(Mach1Point3D newRotationDegrees);

    /**
     * @brief Set current buffer/sample intended decoding orientation YPR.
     * @param newRotationFromMinusOnetoOne
     * <ul>
     *      <li> Yaw:   [Range: 0.0 -> 2PI | -PI  -> PI];   [Range: 0.0 -> 360 | -180 -> 180] </li>
     *      <li> Pitch: [Range: -PI/2 -> PI/2];             [Range: -90   -> 90] </li>
     *      <li> Roll:  [Range: -PI/2 -> PI/2];             [Range: -90   -> 90] </li>
     * </ul>
     */
    void setRotationRadians(Mach1Point3D newRotationRadians);

    /**
     * @brief Set current buffer/sample intended decoding orientation YPR.
     * @param newRotationFromMinusOnetoOne
     * <ul>
     *      <li> W: [Range: -1.0 -> 1.0] </li>
     *      <li> X: [Range: -1.0 -> 1.0] </li>
     *      <li> Y: [Range: -1.0 -> 1.0] </li>
     *      <li> Z: [Range: -1.0 -> 1.0] </li>
     * </ul>
     */
    void setRotationQuat(Mach1Point4D newRotationQuat);

    /**
     * @brief Set the amount of angle smoothing applied to the orientation angles used for this Mach1Decode.
     * @param filterSpeed floating point value between [0.0001 -> 1.0], where 1.0 represents no filter.
     */
    void setFilterSpeed(float filterSpeed);

    /**
     * @brief Get the current elapsed time in milliseconds (ms) this Mach1Decode has been constructed.
     */
    long getCurrentTime();

    /**
     * @brief Get this Mach1Decode's current 3D angle for feedback design.
     */
    Mach1Point3D getCurrentAngle();

    /**
     * Call with current update's angles to return the coefficients with which an incoming audio buffer
     * may be decoded.
     *
     * Includes two modes of use:
     * <ol>
     *      <li>Use your audio player's buffersize and current sample index for sync callbacks, with
     *      buffer size and sample index as parameters.</li>
     *      <li>Update decode results via main loop (or any loop)</li>
     * </ol>
     *
     * Setting bufferSize and sampleIndex to 0 will enable the second mode.
     *
     * @param Yaw float for device/listener yaw angle: [Range: 0->360 | -180->180]
     * @param Pitch float for device/listener pitch angle: [Range: -90->90]
     * @param Roll float for device/listener roll angle: [Range: -90->90]
     * @param bufferSize int for number of samples in a buffer, ideally supplied from your audioplayer/engine
     * @param sampleIndex int for current sample index array, ideally supplied from your audioplayer/engine
     */
    std::vector<PCM> decode(float Yaw, float Pitch, float Roll, int bufferSize = 0, int sampleIndex = 0);

    /**
     * Call with current update's angles to return the coefficients with which an incoming audio buffer
     * may be decoded.
     *
     * Includes two modes of use:
     * <ol>
     *      <li>Use your audio player's buffersize and current sample index for sync callbacks, with
     *      buffer size and sample index as parameters.</li>
     *      <li>Update decode results via main loop (or any loop)</li>
     * </ol>
     *
     * Setting bufferSize and sampleIndex to 0 will enable the second mode.
     *
     * @param bufferSize int for number of samples in a buffer, ideally supplied from your audioplayer/engine
     * @param sampleIndex int for current sample index array, ideally supplied from your audioplayer/engine
     */
    std::vector<PCM> decodeCoeffs(int bufferSize = 0, int sampleIndex = 0);

    /**
     * Call with current update's angles to return the coefficients with which an incoming audio buffer
     * may be decoded.
     *
     * Includes two modes of use:
     * <ol>
     *      <li>Use your audio player's buffersize and current sample index for sync callbacks, with
     *      buffer size and sample index as parameters.</li>
     *      <li>Update decode results via main loop (or any loop)</li>
     * </ol>
     *
     * Setting bufferSize and sampleIndex to 0 will enable the second mode.
     *
     * @param bufferSize int for number of samples in a buffer, ideally supplied from your audioplayer/engine
     * @param sampleIndex int for current sample index array, ideally supplied from your audioplayer/engine
     * @param applyPanLaw bool for control over panLaw application.
     */
    std::vector<PCM> decodePannedCoeffs(int bufferSize = 0, int sampleIndex = 0, bool applyPanLaw = true);

    std::vector<PCM> decodeCoeffsUsingTranscodeMatrix(std::vector< std::vector<float> > matrix, int channels, int bufferSize = 0, int sampleIndex = 0);

    inline void decodeBuffer(std::vector< std::vector<PCM> >& in, std::vector< std::vector<PCM> >& out, int size);

    inline void decodeBufferInPlace(std::vector< std::vector<PCM> >& buffer, int size);

    inline void decodeBufferRebuffer(std::vector< std::vector<PCM> >& in, std::vector< std::vector<PCM> >& out, int size);

    inline void decodeBufferInPlaceRebuffer(std::vector< std::vector<PCM> >& buffer, int size);

#ifndef __EMSCRIPTEN__
    void decode(float Yaw, float Pitch, float Roll, float *result, int bufferSize = 0, int sampleIndex = 0);
    void decodeCoeffs(float *result, int bufferSize = 0, int sampleIndex = 0);
    void decodePannedCoeffs(float *result, int bufferSize = 0, int sampleIndex = 0, bool applyPanLaw = true);

    /**
     * @brief Get the internal log that has been accumulated into this Mach1Decode.
     */
    char *getLog();
#else
    /**
     * @brief Get the internal log that has been accumulated into this Mach1Decode.
     */
    std::string getLog();
#endif

private:
    void *M1obj;

    std::vector<float> old_decode_gains;
    std::vector<std::vector<float > > intermediary_buffer;
    int ib_channel_count;
    size_t ib_buffer_size;
};

#include "Mach1Decode.tpp" // templated implementation of Mach1Decode
