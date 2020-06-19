//  Mach1 Spatial SDK
//  Copyright © 2017-2020 Mach1. All rights reserved.

#include "Mach1Encode.h"

Mach1Encode::Mach1Encode()
{
	M1obj = Mach1EncodeCAPI_create();
}

Mach1Encode::~Mach1Encode()
{
	Mach1EncodeCAPI_delete(M1obj);
}

std::vector<Mach1Point3D> Mach1Encode::getPoints()
{
	std::vector<Mach1Point3D> vec(Mach1EncodeCAPI_getPointsCount(M1obj));

	Mach1Point3D* arr = (Mach1Point3D*)Mach1EncodeCAPI_getPoints(M1obj);
	for (int i = 0; i < vec.size(); i++) vec[i] = arr[i];

	return vec;
    /// Retruns the control center reference point's normalized coordinate location (XYZ) 
    /// within the vector panning space
    ///
    /// - Parameters:
    ///     - X: front-back (0.0 (back) -> 1.0 (front))
    ///     - Y: up-down (0.0 (down) -> 1.0 (up))
    ///     - Z: left-right (0.0 (left) -> 1.0 (right))
}

std::vector<std::vector<float>> Mach1Encode::getGains()
{
	std::vector<std::vector<float>> vec(Mach1EncodeCAPI_getPointsCount(M1obj));

	float** arr = (float**)Mach1EncodeCAPI_getGains(M1obj);

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

std::vector<std::string> Mach1Encode::getPointsNames()
{
	std::vector<std::string> vec(Mach1EncodeCAPI_getPointsCount(M1obj));

	char** arr = (char**)Mach1EncodeCAPI_getPointsNames(M1obj);

	for (int i = 0; i < vec.size(); i++) {
		vec[i] = arr[i];
	}

	return vec;
    /// Return the string name for each input channel/point
}

std::vector<float> Mach1Encode::getGainsForInputChannelNamed(std::string pointName)
{
	std::vector<float> vec(Mach1EncodeCAPI_getOutputChannelsCount(M1obj));

	float* arr = (float*)Mach1EncodeCAPI_getGainsForInputChannelNamed(M1obj, (char*)pointName.c_str());

	for (int i = 0; i < vec.size(); i++) {
		vec[i] = arr[i];
	}

	return vec;
    /// Returns array of gain coefficients for specificed input channel/point
}

void Mach1Encode::generatePointResults()
{
	Mach1EncodeCAPI_generatePointResults(M1obj);
    /// Function for controlling the update of all calculations of a Mach1Encode instance
    ///
    /// - Remark: Call when an update to Mach1 vector calculations is desired
}

int Mach1Encode::getPointsCount()
{
	return Mach1EncodeCAPI_getPointsCount(M1obj);
    /// Returns the number of input channels/points that Mach1Encode instance has
    ///
    /// - Returns:
    ///     - integer of number of input channels/points
}

std::vector<float> Mach1Encode::getResultingCoeffsDecoded(Mach1DecodeAlgoType decodeType, std::vector<float>& decodeResult)
{
	std::vector<float> vec(Mach1EncodeCAPI_getPointsCount(M1obj) * 2);

	float* arr = (float*)Mach1EncodeCAPI_getResultingCoeffsDecoded(M1obj, decodeType, decodeResult.data());

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

Mach1EncodeInputModeType Mach1Encode::getInputMode()
{
	return Mach1EncodeCAPI_getInputMode(M1obj);
}

Mach1EncodeOutputModeType Mach1Encode::getOutputMode()
{
	return Mach1EncodeCAPI_getOutputMode(M1obj);
}

int Mach1Encode::getInputChannelsCount()
{
    return Mach1EncodeCAPI_getInputChannelsCount(M1obj);
    /// Returns the number of input channels/points that Mach1Encode instance has
    ///
    /// - Returns:
    ///     - integer of number of input channels/points
}

int Mach1Encode::getOutputChannelsCount()
{
    return Mach1EncodeCAPI_getOutputChannelsCount(M1obj);
    /// Returns the number of output channels/points that Mach1Encode instance has
    ///
    /// - Returns:
    ///     - integer of number of output channels/points
}

void Mach1Encode::setInputMode(Mach1EncodeInputModeType inputMode)
{
	Mach1EncodeCAPI_setInputMode(M1obj, inputMode);
	/// Sets the number of input streams to be positioned as points
	///
	/// - Parameters:
	///     - INPUT_MONO
	///     - INPUT_STEREO
	///     - INPUT_QUAD
    ///     - INPUT_LCRS
    ///     - INPUT_AFORMAT
    ///     - INPUT_BFORMAT
    ///     - INPUT_FOAACN
	///		- INPUT_FOAFUM
	///		- INPUT_2OAACN
	/// 	- INPUT_2OAFUM
	/// 	- INPUT_3OAACN
	///		- INPUT_3OAFUMA
	///		- INPUT_LCR
}

void Mach1Encode::setOutputMode(Mach1EncodeOutputModeType outputMode)
{
	Mach1EncodeCAPI_setOutputMode(M1obj, outputMode);
	/// Sets the output spatial format, Mach1Spatial or Mach1Horizon
	///
	/// - Parameters:
	///     - Mach1Horizon (4ch) [Yaw]
	///     - Mach1Spatial (8ch) [Yaw, Pitch, Roll]
	///     - Mach1SpatialPlus (12ch) [Yaw, Pitch, Roll]
	///     - Mach1SpatialPlusPlus (14ch) [Yaw, Pitch, Roll]
	///     - Mach1SpatialExt (16ch) [Yaw, Pitch, Roll]
	///     - Mach1SpatialExtPlus (18ch) [Yaw, Pitch, Roll]
}

void Mach1Encode::setAzimuth(float azimuthFromMinus1To1)
{
	Mach1EncodeCAPI_setAzimuth(M1obj, azimuthFromMinus1To1);
	/// Sets the point(s) azimuth rotation of the vector space
	///
	/// - Parameters:
	///     - value range: -1.0 -> 1.0
}

void Mach1Encode::setAzimuthDegrees(float azimuthDegrees)
{
	Mach1EncodeCAPI_setAzimuthDegrees(M1obj, azimuthDegrees);
	/// Sets the point(s) azimuth rotation of the vector space
	///
	/// - Parameters:
	///     - value range: 0 -> 360
}

void Mach1Encode::setAzimuthRadians(float azimuthRadians)
{
	Mach1EncodeCAPI_setAzimuthRadians(M1obj, azimuthRadians);
	/// Sets the point(s) azimuth rotation of the vector space
	///
	/// - Parameters:
	///     - value range: -PI/2 -> PI/2
}

void Mach1Encode::setDiverge(float divergeFromMinus1To1)
{
	Mach1EncodeCAPI_setDiverge(M1obj, divergeFromMinus1To1);
	/// Sets the point(s) to/from center origin of the vector space
	///
	/// - Parameters:
	///     - value range: -1.0 -> 1.0
}

void Mach1Encode::setElevation(float elevationFromMinus1to1)
{
	Mach1EncodeCAPI_setElevation(M1obj, elevationFromMinus1to1);
	/// Sets the point(s) up/down the vector space
	///
	/// - Parameters:
	///     - value range: -1.0 -> 1.0
}

void Mach1Encode::setElevationDegrees(float elevationFromMinus90to90)
{
	Mach1EncodeCAPI_setElevationDegrees(M1obj, elevationFromMinus90to90);
	/// Sets the point(s) up/down the vector space
	///
	/// - Parameters:
	///     - value range: -90->90
}

void Mach1Encode::setElevationRadians(float elevationFromMinusHalfPItoHalfPI)
{
	Mach1EncodeCAPI_setElevationRadians(M1obj, elevationFromMinusHalfPItoHalfPI);
	/// Sets the point(s) up/down the vector space
	///
	/// - Parameters:
	///     - value range: -PI/2 -> PI/2
}

void Mach1Encode::setIsotropicEncode(bool isotropicEncode)
{
	Mach1EncodeCAPI_setIsotropicEncode(M1obj, isotropicEncode);
	/// Sets both stereo points rotate in relation to the
	/// center point between them so that they always triangulate
	/// toward center of the cuboid
	///
	/// Remark: Default is true
}

void Mach1Encode::setOrbitRotation(float orbitRotationFromMinusOnetoOne)
{
	Mach1EncodeCAPI_setOrbitRotation(M1obj, orbitRotationFromMinusOnetoOne);
	/// Sets the two stereo points around the axis of the center point between them
	///
	/// - Parameters:
	///     - value range: -1.0 -> 1.0
}

void Mach1Encode::setOrbitRotationDegrees(float orbitRotationDegrees)
{
	Mach1EncodeCAPI_setOrbitRotationDegrees(M1obj, orbitRotationDegrees);
	/// Sets the two stereo points around the axis of the center point between them
	///
	/// - Parameters:
	///     - value range: -180.0->180.0
}

void Mach1Encode::setOrbitRotationRadians(float orbitRotationRadians)
{
	Mach1EncodeCAPI_setOrbitRotationRadians(M1obj, orbitRotationRadians);
	/// Sets the two stereo points around the axis of the center point between them
	///
	/// - Parameters:
	///     - value range: -PI -> PI
}

void Mach1Encode::setStereoSpread(float sSpreadFrom0to1)
{
	Mach1EncodeCAPI_setStereoSpread(M1obj, sSpreadFrom0to1);
	/// Sets the space between the two stereo points
	///
	/// - Parameters:
	///     - value range: 0.0 -> 1.0
}

void Mach1Encode::setAutoOrbit(bool autoOrbit)
{
	Mach1EncodeCAPI_setAutoOrbit(M1obj, autoOrbit);
	/// Sets encoding behavior acts evenly with distribution
	/// across all azimuth/rotation angles and all altitude/pitch angles
	///
	/// Remark: Default is true
}

/* DEPRECATED START*/
std::vector<float> Mach1Encode::getResultingVolumesDecoded(Mach1DecodeAlgoType decodeType, std::vector<float>& decodeResult)
{
	std::vector<float> vec(Mach1EncodeCAPI_getPointsCount(M1obj) * 2);

	float* arr = (float*)Mach1EncodeCAPI_getResultingVolumesDecoded(M1obj, decodeType, decodeResult.data());

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

void Mach1Encode::setRotation(float rotationDegrees)
{
	/// Sets the point(s) azimuth rotation of the vector space
	///
	/// - Parameters:
	///     - value range: 0 -> 360
	setAzimuthDegrees(rotationDegrees);
}

void Mach1Encode::setPitch(float pitchFromMinus90to90)
{
	Mach1EncodeCAPI_setPitch(M1obj, pitchFromMinus90to90);
	/// Sets the point(s) up/down the vector space
	///
	/// - Parameters:
	///     - value range: -90->90
}

void Mach1Encode::setStereoRotate(float sRotateDegrees)
{
	Mach1EncodeCAPI_setStereoRotate(M1obj, sRotateDegrees);
	/// Sets the two stereo points around the axis of the center point between them
	///
	/// - Parameters:
	///     - value range: -180.0->180.0
}
/* DEPRECATED END */