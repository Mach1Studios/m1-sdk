//  Mach1 SDK
//  Copyright © 2017 Mach1. All rights reserved.

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
	///     - Mach1PlatformDefault = 0
	///     - Mach1PlatformUnity = 1
	///     - Mach1PlatformUE = 2
	///     - Mach1PlatformOfEasyCam = 3
	///     - Mach1PlatformAndroid = 4
	///     - Mach1PlatformiOS = 5
}

void Mach1Decode::setDecodeAlgoType(Mach1DecodeAlgoType newAlgorithmType)
{
	Mach1DecodeCAPI_setDecodeAlgoType(M1obj, newAlgorithmType);
	/// Set the decoding algorithm
	///
	/// - Parameters:
	///     - Mach1DecodeAlgoSpatial = 0 (default spatial | 8 channels)
	///     - Mach1DecodeAlgoAltSpatial = 1 (periphonic spatial | 8 channels)
	///     - Mach1DecodeAlgoHorizon = 2 (compass / yaw | 4 channels)
	///     - Mach1DecodeAlgoHorizonPairs = 3 (compass / yaw | 4x stereo mastered pairs)
	///     - Mach1DecodeAlgoSpatialPairs = 4 (experimental periphonic pairs | 8x stereo mastered pairs)
}

#ifndef  __EMSCRIPTEN__
void Mach1Decode::decode(float Yaw, float Pitch, float Roll, float * result, int bufferSize, int sampleIndex)
{
	Mach1DecodeCAPI_decode(M1obj, Yaw, Pitch, Roll, result, bufferSize, sampleIndex);
}
#endif

std::vector<float> Mach1Decode::decode(float Yaw, float Pitch, float Roll, int bufferSize, int sampleIndex)
{
	static std::vector<float> vec(18);

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