//  Mach1 Spatial SDK
//  Copyright © 2017-2020 Mach1. All rights reserved.

#include "Mach1Decode.h"

Mach1Decode::Mach1Decode()
{
	M1obj = Mach1DecodeCAPI_create();
}

Mach1Decode::~Mach1Decode()
{
	Mach1DecodeCAPI_delete(M1obj);
}

void Mach1Decode::setPlatformType(Mach1PlatformType type)
{
	Mach1DecodeCAPI_setPlatformType(M1obj, type);
	/// Set the device's angle order and convention if applicable
	///
	/// - Parameters:
	///     - Mach1PlatformDefault
	///     - Mach1PlatformUnity
	///     - Mach1PlatformUE
	///     - Mach1PlatformOfEasyCam
	///     - Mach1PlatformAndroid
	///     - Mach1PlatformiOS
}

void Mach1Decode::setDecodeAlgoType(Mach1DecodeAlgoType newAlgorithmType)
{
	Mach1DecodeCAPI_setDecodeAlgoType(M1obj, newAlgorithmType);
	/// Set the decoding algorithm
	///
	/// - Parameters:
	///     - Mach1DecodeAlgoSpatial_8 (default spatial | 8 channels)
	///     - Mach1DecodeAlgoSpatialAlt_8 (periphonic spatial | 8 channels)
	///     - Mach1DecodeAlgoHorizon_4 (compass / yaw | 4 channels)
	///     - Mach1DecodeAlgoHorizonPairs (compass / yaw | 4x stereo mastered pairs)
	///     - Mach1DecodeAlgoSpatialPairs (experimental periphonic pairs | 8x stereo mastered pairs)
	///     - Mach1DecodeAlgoSpatial_12 (higher order spatial | 12 channels)
	///     - Mach1DecodeAlgoSpatial_14 (higher order spatial | 14 channels)
	///     - Mach1DecodeAlgoSpatial_16 (higher order spatial | 16 channels)
	///     - Mach1DecodeAlgoSpatial_18 (higher order spatial | 18 channels)
	///     - Mach1DecodeAlgoSpatial_24 (higher order spatial | 24 channels)
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

void Mach1Decode::decodePannedCoeffs(float * result, int bufferSize, int sampleIndex, bool applyPanLaw)
{
	Mach1DecodeCAPI_decodePannedCoeffs(M1obj, result, bufferSize, sampleIndex, applyPanLaw);
}
#endif

std::vector<float> Mach1Decode::decode(float Yaw, float Pitch, float Roll, int bufferSize, int sampleIndex)
{
	std::vector<float> vec(getFormatCoeffCount());

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
	std::vector<float> vec(getFormatCoeffCount());

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

std::vector<float> Mach1Decode::decodePannedCoeffs(int bufferSize, int sampleIndex, bool applyPanLaw)
{
	std::vector<float> vec(getFormatCoeffCount());

	Mach1DecodeCAPI_decodePannedCoeffs(M1obj, vec.data(), bufferSize, sampleIndex, applyPanLaw);

	return vec;
    /// Call with current `setRotationDegrees` to return the resulting coefficient per channel along with an associated pan float
    /// to apply to the audioplayer's volume and left/right panner per channel
    ///
    /// 2D Vector Structure: 
    /// [0]{ gain, pan } // gain & pan for first channel
    /// [1]{ gain, pan } // gain & pan for second channel
    /// [2]{ gain, pan } // etc.
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
    ///		- applyPanLaw: bool for control over panLaw application
}

std::vector<float> Mach1Decode::decodeCoeffsUsingTranscodeMatrix(std::vector< std::vector<float> > matrix, int channels, int bufferSize, int sampleIndex)
{
	std::vector<float> vec(2 * channels);

	int inChans = channels;
	int outChans = getFormatChannelCount();

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

int Mach1Decode::getFormatCoeffCount()
{
	return Mach1DecodeCAPI_getFormatCoeffCount(M1obj);
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

void Mach1Decode::setFilterSpeed(float filterSpeed)
{
	Mach1DecodeCAPI_setFilterSpeed(M1obj, filterSpeed);
	/// Filter speed determines the amount of angle smoothing applied
	/// to the orientation angles used for the Mach1DecodeCore class
	///
	/// - Parameters:
	///     - value range: 0.0001 -> 1.0 (where 0.1 would be a slow filter
	///     and 1.0 is no filter)
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

long Mach1Decode::getCurrentTime()
{
	return Mach1DecodeCAPI_getCurrentTime(M1obj);
	/// Returns the current elapsed time in milliseconds (ms) since Mach1Decode object creation
}

#ifndef  __EMSCRIPTEN__
char* Mach1Decode::getLog()
{
	return Mach1DecodeCAPI_getLog(M1obj);
}
#else
std::string Mach1Decode::getLog()
{
	return std::string(Mach1DecodeCAPI_getLog(M1obj));
	/// Returns the current elapsed time in milliseconds (ms) since Mach1Decode object creation
}
#endif

Mach1Point3D Mach1Decode::getCurrentAngle()
{
	return Mach1DecodeCAPI_getCurrentAngle(M1obj);
	/// Returns the Mach1Decode lib object's current 3D angle for feedback design
}
