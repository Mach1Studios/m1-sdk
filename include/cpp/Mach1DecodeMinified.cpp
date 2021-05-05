//  Mach1 Spatial SDK
//  Copyright © 2017-2020 Mach1. All rights reserved.

#include "Mach1DecodeMinified.h"

Mach1Decode::Mach1Decode()
{
	M1obj = Mach1DecodeCAPI_create();
}

Mach1Decode::~Mach1Decode()
{
	Mach1DecodeCAPI_delete(M1obj);
}

void Mach1Decode::setDecodeAlgoType(Mach1DecodeAlgoType newAlgorithmType)
{
	Mach1DecodeCAPI_setDecodeAlgoType(M1obj, newAlgorithmType);
	/// Set the decoding algorithm
	///
	/// - Parameters:
	///     - Mach1DecodeAlgoSpatial = 0 (default spatial | 8 channels)
	///     - Mach1DecodeAlgoHorizon = 2 (compass / yaw | 4 channels)
}

#ifndef  __EMSCRIPTEN__
void Mach1Decode::decode(float Yaw, float Pitch, float Roll, float * result, int bufferSize, int sampleIndex)
{
	Mach1DecodeCAPI_decode(M1obj, Yaw, Pitch, Roll, result, bufferSize, sampleIndex);
}

void Mach1Decode::decodeCoeffs(float * result, int bufferSize, int sampleIndex)
{
	Mach1DecodeCAPI_decodeCoeffs(M1obj, result, bufferSize, sampleIndex);
}
#endif

std::vector<float> Mach1Decode::decode(float Yaw, float Pitch, float Roll, int bufferSize, int sampleIndex)
{
	std::vector<float> vec(getFormatChannelCount());

	Mach1DecodeCAPI_decode(M1obj, Yaw, Pitch, Roll, vec.data(), bufferSize, sampleIndex);

	return vec;
    /// Call with current update's angles to return the resulting coefficients
    /// to apply to the audioplayer's volume
    ///
    /// Includes two modes of use:
    /// + Update decode results via audio callback
    ///   + *Use your audio player's buffersize and current sample index for sync callbacks*
    /// + Update decode results via main loop (or any loop)
    ///   + *Default null or 0 values to **bufferSize** or **sampleIndex** will use the second mode*
    ///
    /// - Parameters: 
    ///     - Yaw: float for device/listener yaw angle: [Range: 0->360 | -180->180]
    ///     - Pitch: float for device/listener pitch angle: [Range: -90->90]
    ///     - Roll: float for device/listener roll angle: [Range: -90->90]
    ///     - bufferSize: int for number of samples in a buffer, ideally supplied from your audioplayer/engine
    ///     - sampleIndex: int for current sample index array, ideally supplied from your audioplayer/engine
}

std::vector<float> Mach1Decode::decodeCoeffs(int bufferSize, int sampleIndex)
{
	std::vector<float> vec(getFormatChannelCount());

	Mach1DecodeCAPI_decodeCoeffs(M1obj, vec.data(), bufferSize, sampleIndex);

	return vec;
    /// Call with current `setRotationDegrees` to return the resulting coefficients
    /// to apply to the audioplayer's volume
    ///
    /// Includes two modes of use:
    /// + Update decode results via audio callback
    ///   + *Use your audio player's buffersize and current sample index for sync callbacks*
    /// + Update decode results via main loop (or any loop)
    ///   + *Default null or 0 values to **bufferSize** or **sampleIndex** will use the second mode*
    ///
    /// - Parameters: 
    ///     - bufferSize: int for number of samples in a buffer, ideally supplied from your audioplayer/engine
    ///     - sampleIndex: int for current sample index array, ideally supplied from your audioplayer/engine
}

std::vector<float> Mach1Decode::decodeCoeffsUsingTranscodeMatrix(std::vector< std::vector<float> > matrix, int channels, int bufferSize, int sampleIndex)
{
	std::vector<float> vec(2 * channels);

	int inChans = channels;
	int outChans = (getFormatChannelCount() - 1) / 2;

	float* m = new float[inChans * outChans];
	for (int i = 0; i < outChans; i++) {
		for (int j = 0; j < inChans; j++) {
			m[i * inChans + j] = matrix[i][j];
		}
	}

	Mach1DecodeCAPI_decodeCoeffsUsingTranscodeMatrix(M1obj, m, channels, vec.data(), bufferSize, sampleIndex);

	delete[] m;
	return vec;
}

int Mach1Decode::getFormatChannelCount()
{
	return Mach1DecodeCAPI_getFormatChannelCount(M1obj);
}

void Mach1Decode::setRotation(Mach1Point3D newRotationFromMinusOnetoOne)
{
	Mach1DecodeCAPI_setRotation(M1obj, newRotationFromMinusOnetoOne);
	/// Set current buffer/sample intended decoding orientation YPR.
	///
    /// - Parameters: 
    ///     - Yaw: float for device/listener yaw angle: 	[Range: 0.0 -> 1.0 | -0.5 -> 0.5]
    ///														[Range: 0.0 -> 360 | -180 -> 180]
    ///     - Pitch: float for device/listener pitch angle: [Range: -0.25 -> 0.25]
    ///														[Range: -90   -> 90]
    ///     - Roll: float for device/listener roll angle: 	[Range: -0.25 -> 0.25]
    ///													  	[Range: -90   -> 90]
}

void Mach1Decode::setRotationDegrees(Mach1Point3D newRotationDegrees)
{
	Mach1DecodeCAPI_setRotationDegrees(M1obj, newRotationDegrees);
	/// Set current buffer/sample intended decoding orientation YPR.
	///
    /// - Parameters: 
    ///     - Yaw: float for device/listener yaw angle: [Range: 0.0 -> 360 | -180 -> 180]
    ///     - Pitch: float for device/listener pitch angle: [Range: -90 -> 90]
    ///     - Roll: float for device/listener roll angle: [Range: -90 -> 90]
}

void Mach1Decode::setRotationRadians(Mach1Point3D newRotationRadians)
{
	Mach1DecodeCAPI_setRotationRadians(M1obj, newRotationRadians);
    /// Set current buffer/sample intended decoding orientation YPR in radians.
    ///
    /// - Parameters: 
    ///     - Yaw: float for device/listener yaw angle: 	[Range: 0.0 -> 2PI | -PI  -> PI]
    ///														[Range: 0.0 -> 360 | -180 -> 180]
    ///     - Pitch: float for device/listener pitch angle: [Range: -PI/2 -> PI/2]
    ///														[Range: -90   -> 90]
    ///     - Roll: float for device/listener roll angle: 	[Range: -PI/2 -> PI/2]
    ///														[Range: -90   -> 90]
}

void Mach1Decode::setRotationQuat(Mach1Point4D newRotationQuat)
{
	Mach1DecodeCAPI_setRotationQuat(M1obj, newRotationQuat);
    /// Set current buffer/sample intended decoding orientation YPR in quaternion.
    ///
    /// - Parameters: 
    ///     - W: float for device/listener W: [Range: -1.0->1.0]
    ///     - X: float for device/listener X: [Range: -1.0->1.0]
    ///     - Y: float for device/listener Y: [Range: -1.0->1.0]
    ///     - Z: float for device/listener Z: [Range: -1.0->1.0]
}

void Mach1Decode::beginBuffer()
{
	Mach1DecodeCAPI_beginBuffer(M1obj);
	/// Call this function before reading from the Mach1Decode buffer
}

void Mach1Decode::endBuffer()
{
	Mach1DecodeCAPI_endBuffer(M1obj);
	/// Call this function after reading from the Mach1Decode buffer
}

Mach1Point3D Mach1Decode::getCurrentAngle()
{
	return Mach1DecodeCAPI_getCurrentAngle(M1obj);
	/// Returns the Mach1Decode lib object's current 3D angle for feedback design
}
