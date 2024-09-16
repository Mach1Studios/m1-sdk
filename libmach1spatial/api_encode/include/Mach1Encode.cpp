//  Mach1 Spatial SDK
//  Copyright © 2017 Mach1. All rights reserved.

#include "Mach1Encode.h"

Mach1Encode::Mach1Encode() {
    M1obj = Mach1EncodeCAPI_create();
}

Mach1Encode::~Mach1Encode() {
    Mach1EncodeCAPI_delete(M1obj);
}

Mach1Encode::Mach1Encode(const Mach1Encode &other) {
    M1obj = Mach1EncodeCAPI_create();
    Mach1EncodeCAPI_copy(other.M1obj, M1obj);
    gains = other.gains;
}

Mach1Encode &Mach1Encode::operator=(const Mach1Encode &other) {
    if (this != &other) {
        Mach1EncodeCAPI_copy(other.M1obj, M1obj);
        gains = other.gains;
    }
    return *this;
}

std::vector<Mach1Point3D> Mach1Encode::getPoints() {
    std::vector<Mach1Point3D> vec(Mach1EncodeCAPI_getPointsCount(M1obj));

    Mach1Point3D *arr = (Mach1Point3D *)Mach1EncodeCAPI_getPoints(M1obj);
    for (int i = 0; i < vec.size(); i++)
        vec[i] = arr[i];

    return vec;
    /// Retruns the control center reference point's normalized coordinate location (XYZ)
    /// within the vector panning space
    ///
    /// - Parameters:
    ///     - X: front-back (0.0 (back) -> 1.0 (front))
    ///     - Y: up-down (0.0 (down) -> 1.0 (up))
    ///     - Z: left-right (0.0 (left) -> 1.0 (right))
}

std::vector<std::vector<float> > Mach1Encode::getGains() {
    std::vector<std::vector<float> > vec(Mach1EncodeCAPI_getPointsCount(M1obj));

    float **arr = (float **)Mach1EncodeCAPI_getGains(M1obj);

    for (int i = 0; i < vec.size(); i++) {
        vec[i].resize(Mach1EncodeCAPI_getOutputChannelsCount(M1obj));

        for (int j = 0; j < vec[i].size(); j++) {
            vec[i][j] = arr[i][j];
        }
    }

    return vec;
    /// Returns an array per input channel, resulting in an array of array results
    /// each internal array (per input channel) is a list of the needed coefficients to
    /// encode to a Mach1 VVBP/SPS format (depending on the selected `setOutputMode`)
    ///
    /// - Returns:
    ///     - 1st dimension of array is the number of input channels/points
    ///     - 2nd dimension of array is the resulting coefficient gains to be applied for encode
}

std::vector<std::string> Mach1Encode::getPointsNames() {
    std::vector<std::string> vec(Mach1EncodeCAPI_getPointsCount(M1obj));

    char **arr = (char **)Mach1EncodeCAPI_getPointsNames(M1obj);

    for (int i = 0; i < vec.size(); i++) {
        vec[i] = arr[i];
    }

    return vec;
    /// Return the string name for each input channel/point
}

std::vector<float> Mach1Encode::getGainsForInputChannelNamed(std::string pointName) {
    std::vector<float> vec(Mach1EncodeCAPI_getOutputChannelsCount(M1obj));

    float *arr = (float *)Mach1EncodeCAPI_getGainsForInputChannelNamed(M1obj, (char *)pointName.c_str());

    for (int i = 0; i < vec.size(); i++) {
        vec[i] = arr[i];
    }

    return vec;
    /// Returns array of gain coefficients for specificed input channel/point
}

void Mach1Encode::generatePointResults() {
    Mach1EncodeCAPI_generatePointResults(M1obj);
    /// Function for controlling the update of all calculations of a Mach1Encode instance
    ///
    /// - Remark: Call when an update to Mach1 vector calculations is desired
}

int Mach1Encode::getPointsCount() {
    return Mach1EncodeCAPI_getPointsCount(M1obj);
    /// Returns the number of input channels/points that Mach1Encode instance has
    ///
    /// - Returns:
    ///     - integer of number of input channels/points
}

#ifdef M1ENCODE_INLINE_DECODE
std::vector<float> Mach1Encode::getResultingCoeffsDecoded(Mach1DecodeMode decodeMode, std::vector<float> &decodeResult) {
    std::vector<float> vec(Mach1EncodeCAPI_getPointsCount(M1obj) * 2);

    float *arr = (float *)Mach1EncodeCAPI_getResultingCoeffsDecoded(M1obj, decodeMode, decodeResult.data());

    for (int i = 0; i < vec.size(); i++) {
        vec[i] = arr[i];
    }

    return vec;
    /// A shorthand function for encoding->decoding audio object handling,
    /// useful preview UX so that a full input->mach1spatial_multichannel->stereo
    /// rendeering to disk isnt required and instead designs that stack decode results
    /// live can more easily be created
    ///
    /// - Remark: Each input audio channel results a direct decode instead of the encode coefficients
}
#endif

Mach1EncodeInputMode Mach1Encode::getInputMode() {
    return Mach1EncodeCAPI_getInputMode(M1obj);
}

Mach1EncodeOutputMode Mach1Encode::getOutputMode() {
    return Mach1EncodeCAPI_getOutputMode(M1obj);
}

Mach1EncodePannerMode Mach1Encode::getPannerMode() {
    return Mach1EncodeCAPI_getPannerMode(M1obj);
}

bool Mach1Encode::getAutoOrbit() {
    return Mach1EncodeCAPI_getAutoOrbit(M1obj);
}

int Mach1Encode::getInputChannelsCount() {
    return Mach1EncodeCAPI_getInputChannelsCount(M1obj);
    /// Returns the number of input channels/points that Mach1Encode instance has
    ///
    /// - Returns:
    ///     - integer of number of input channels/points
}

int Mach1Encode::getOutputChannelsCount() {
    return Mach1EncodeCAPI_getOutputChannelsCount(M1obj);
    /// Returns the number of output channels/points that Mach1Encode instance has
    ///
    /// - Returns:
    ///     - integer of number of output channels/points
}

void Mach1Encode::setInputMode(Mach1EncodeInputMode inputMode) {
    Mach1EncodeCAPI_setInputMode(M1obj, inputMode);
    /// Sets the number of input streams to be positioned as points
    ///
    /// - Parameters:
    /// 	- Mach1EncodeInputModeMono
    /// 	- Stereo
    /// 	- LCR
    /// 	- Quad
    /// 	- LCRS
    /// 	- AFormat
    /// 	- FiveDotZero
    /// 	- FiveDotOneFilm
    /// 	- FiveDotOneDTS
    /// 	- FiveDotOneSMTPE
    /// 	- BFOAACN
    /// 	- BFOAFUM
    /// 	- B2OAACN
    /// 	- B2OAFUMA
    /// 	- B3OAACN,
    /// 	- B3OAFUMA
}

void Mach1Encode::setOutputMode(Mach1EncodeOutputMode outputMode) {
    Mach1EncodeCAPI_setOutputMode(M1obj, outputMode);
    /// Sets the output Mach1Spatial mode
    ///
    /// - Parameters:
    ///     - M1Spatial_4 (4ch) [Yaw]
    ///     - M1Spatial_8 (8ch) [Yaw, Pitch, Roll]
    ///     - M1Spatial_12 (12ch) [Yaw, Pitch, Roll]
    ///     - M1Spatial_14 (14ch) [Yaw, Pitch, Roll]
}

void Mach1Encode::setAzimuth(float azimuthFromMinus1To1) {
    Mach1EncodeCAPI_setAzimuth(M1obj, azimuthFromMinus1To1);
    /// Sets the point(s) azimuth rotation of the vector space
    ///
    /// - Parameters:
    ///     - value range: -1.0 -> 1.0
}

void Mach1Encode::setAzimuthDegrees(float azimuthDegrees) {
    Mach1EncodeCAPI_setAzimuthDegrees(M1obj, azimuthDegrees);
    /// Sets the point(s) azimuth rotation of the vector space
    ///
    /// - Parameters:
    ///     - value range: 0 -> 360
}

void Mach1Encode::setAzimuthRadians(float azimuthRadians) {
    Mach1EncodeCAPI_setAzimuthRadians(M1obj, azimuthRadians);
    /// Sets the point(s) azimuth rotation of the vector space
    ///
    /// - Parameters:
    ///     - value range: -PI -> PI
}

void Mach1Encode::setDiverge(float divergeFromMinus1To1) {
    Mach1EncodeCAPI_setDiverge(M1obj, divergeFromMinus1To1);
    /// Sets the point(s) to/from center origin of the vector space
    ///
    /// - Parameters:
    ///     - value range: -1.0 -> 1.0
}

void Mach1Encode::setElevation(float elevationFromMinus1to1) {
    Mach1EncodeCAPI_setElevation(M1obj, elevationFromMinus1to1);
    /// Sets the point(s) up/down the vector space
    ///
    /// - Parameters:
    ///     - value range: -1.0 -> 1.0
}

void Mach1Encode::setElevationDegrees(float elevationFromMinus90to90) {
    Mach1EncodeCAPI_setElevationDegrees(M1obj, elevationFromMinus90to90);
    /// Sets the point(s) up/down the vector space
    ///
    /// - Parameters:
    ///     - value range: -90->90
}

void Mach1Encode::setElevationRadians(float elevationFromMinusHalfPItoHalfPI) {
    Mach1EncodeCAPI_setElevationRadians(M1obj, elevationFromMinusHalfPItoHalfPI);
    /// Sets the point(s) up/down the vector space
    ///
    /// - Parameters:
    ///     - value range: -PI/2 -> PI/2
}

void Mach1Encode::setPannerMode(Mach1EncodePannerMode pannerMode) {
    Mach1EncodeCAPI_setPannerMode(M1obj, pannerMode);
    /// Sets the style and mode of panner input calculation
    ///
    /// - Parameters:
    ///		- IsotropicLinear
    ///		- IsotropicEqualPower
    ///		- PeriphonicLinear
}

void Mach1Encode::setFrontSurroundPerspective(bool frontSurroundPerspective) {
    Mach1EncodeCAPI_setFrontSurroundPerspective(M1obj, frontSurroundPerspective);
    /// Sets the encoding behavior of the Center input channels of relevant Surround format,
    /// when true the encoding behavior assumes first person perspective encoding Center channels
    /// toward the front of the soundfield, when false we use Center channel literally making it
    /// encoded MONO and omni-directional.
    ///
    /// Remark: Default is true
}

void Mach1Encode::setOutputGain(float outputGainMultipler, bool isDecibel) {
    Mach1EncodeCAPI_setOutputGain(M1obj, outputGainMultipler, isDecibel);
    /// Applies an additional gain multiplier to all output channels equally
    /// Can be calculated with a linear amplitude multiplier or a decibel multiplier
    ///
    /// Remark: Default is passthrough (1.0, false)
}

void Mach1Encode::setOrbitRotation(float orbitRotationFromMinusOnetoOne) {
    Mach1EncodeCAPI_setOrbitRotation(M1obj, orbitRotationFromMinusOnetoOne);
    /// Sets the two stereo points around the axis of the center point between them
    ///
    /// - Parameters:
    ///     - value range: -1.0 -> 1.0
}

void Mach1Encode::setOrbitRotationDegrees(float orbitRotationDegrees) {
    Mach1EncodeCAPI_setOrbitRotationDegrees(M1obj, orbitRotationDegrees);
    /// Sets the two stereo points around the axis of the center point between them
    ///
    /// - Parameters:
    ///     - value range: -180.0->180.0
}

void Mach1Encode::setOrbitRotationRadians(float orbitRotationRadians) {
    Mach1EncodeCAPI_setOrbitRotationRadians(M1obj, orbitRotationRadians);
    /// Sets the two stereo points around the axis of the center point between them
    ///
    /// - Parameters:
    ///     - value range: -PI -> PI
}

void Mach1Encode::setStereoSpread(float sSpreadFrom0to1) {
    Mach1EncodeCAPI_setStereoSpread(M1obj, sSpreadFrom0to1);
    /// Sets the space between the two stereo points
    ///
    /// - Parameters:
    ///     - value range: 0.0 -> 1.0
}

void Mach1Encode::setAutoOrbit(bool autoOrbit) {
    Mach1EncodeCAPI_setAutoOrbit(M1obj, autoOrbit);
    /// Sets encoding behavior acts evenly with distribution
    /// across all azimuth/rotation angles and all altitude/pitch angles
    ///
    /// Remark: Default is true
}
