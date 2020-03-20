//  Mach1 Spatial SDK
//  Copyright © 2017-2020 Mach1. All rights reserved.

#include "Mach1DecodePositional.h"

#ifndef DEG_TO_RAD
#define DEG_TO_RAD (PI/180.0)
#endif

#ifndef __FLT_EPSILON__
#define __FLT_EPSILON__ 1.19209290e-07F
#endif


Mach1DecodePositional::Mach1DecodePositional()
{
	M1obj = Mach1DecodePositionalCAPI_create();
}

Mach1DecodePositional::~Mach1DecodePositional()
{
	Mach1DecodePositionalCAPI_delete(M1obj);
}

void Mach1DecodePositional::setPlatformType(Mach1PlatformType type)
{
	Mach1DecodePositionalCAPI_setPlatformType(M1obj, type);
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

void Mach1DecodePositional::setDecodeAlgoType(Mach1DecodeAlgoType newAlgorithmType)
{
	Mach1DecodePositionalCAPI_setDecodeAlgoType(M1obj, newAlgorithmType);
  /// Set the decoding algorithm
  ///
  /// - Parameters:
  ///     - Mach1DecodeAlgoSpatial = 0 (default spatial | 8 channels)
  ///     - Mach1DecodeAlgoAltSpatial = 1 (periphonic spatial | 8 channels)
  ///     - Mach1DecodeAlgoHorizon = 2 (compass / yaw | 4 channels)
  ///     - Mach1DecodeAlgoHorizonPairs = 3 (compass / yaw | 4x stereo mastered pairs)
  ///     - Mach1DecodeAlgoSpatialPairs = 4 (experimental periphonic pairs | 8x stereo mastered pairs)
}

void Mach1DecodePositional::setUseBlendMode(bool useBlendMode)
{
	Mach1DecodePositionalCAPI_setUseBlendMode(M1obj, useBlendMode);
  /// - Warning: Experimental feature
  /// - Remark: Part of BlendMode
}

void Mach1DecodePositional::setIgnoreTopBottom(bool ignoreTopBottom)
{
	Mach1DecodePositionalCAPI_setIgnoreTopBottom(M1obj, ignoreTopBottom);
  /// - Warning: Experimental feature
  /// - Remark: Part of BlendMode
}

void Mach1DecodePositional::setMuteWhenOutsideObject(bool muteWhenOutsideObject)
{
	Mach1DecodePositionalCAPI_setMuteWhenOutsideObject(M1obj, muteWhenOutsideObject);
  /// Mute mach1decode object (all coefficifient results becomes 0)
  /// when outside the positional reference shape/point
}

void Mach1DecodePositional::setMuteWhenInsideObject(bool muteWhenInsideObject)
{
	Mach1DecodePositionalCAPI_setMuteWhenInsideObject(M1obj, muteWhenInsideObject);
  /// Mute mach1decode object (all coefficifient results becomes 0)
  /// when inside the positional reference shape/point
}

void Mach1DecodePositional::setUseAttenuation(bool useAttenuation)
{
	Mach1DecodePositionalCAPI_setUseAttenuation(M1obj, useAttenuation);
  /// Set distance attenuation calculations on that mach1decode object on/off
  ///
  /// - Remark: When off, distance remains 1.0
}

void Mach1DecodePositional::setAttenuationCurve(float attenuationCurve)
{
	Mach1DecodePositionalCAPI_setAttenuationCurve(M1obj, attenuationCurve);
  /// Set the current normalized distance per update for attenuation coefficient
  ///
  /// - Parameters: 
  ///     - Normalized range: 0.0 -> 1.0
}

void Mach1DecodePositional::setAttenuationCurveBlendMode(float attenuationCurveBlendMode)
{
	Mach1DecodePositionalCAPI_setAttenuationCurveBlendMode(M1obj, attenuationCurveBlendMode);
  /// Set the current normalized distance per update for blendmode's 
  /// internalattenuation coefficient
  ///
  /// - Parameters: 
  ///     - Normalized range: 0.0 -> 1.0
  ///
  /// - Warning: Experimental feature
  /// - Remark: Part of BlendMode
}

void Mach1DecodePositional::setUsePlaneCalculation(bool usePlaneCalculation)
{
	Mach1DecodePositionalCAPI_setUsePlaneCalculation(M1obj, usePlaneCalculation);
  /// Calculate the rotation to the decode object from it's center point
  /// or from the closest point to the object on a plane
  ///
  /// - Parameters:
  ///     - True: Closest point of plane of object for calculations of rotation
  ///     - False (default): center point of object for calculations of rotation 
}

void Mach1DecodePositional::setUseYawForRotation(bool useYawForRotation)
{
	Mach1DecodePositionalCAPI_setUseYawForRotation(M1obj, useYawForRotation);
  /// Set use/ignore Yaw angle rotation results from pivoting positionally
  ///
  /// - Remark: Default is true
}

void Mach1DecodePositional::setUsePitchForRotation(bool usePitchForRotation)
{
	Mach1DecodePositionalCAPI_setUsePitchForRotation(M1obj, usePitchForRotation);
  /// Set use/ignore Pitch angle rotation results from pivoting positionally
  ///
  /// - Remark: Default is true
}

void Mach1DecodePositional::setUseRollForRotation(bool useRollForRotation)
{
	Mach1DecodePositionalCAPI_setUseRollForRotation(M1obj, useRollForRotation);
  /// Set use/ignore Roll angle rotation results from pivoting positionally
  ///
  /// - Remark: Default is true
}

void Mach1DecodePositional::setListenerPosition(Mach1Point3D point)
{
	Mach1DecodePositionalCAPI_setListenerPosition(M1obj, point);
  /// Sets the device/camera's position in desired x,y,z space
  ///
  /// - Parameters:
  ///     - Mach1Point3D Position: x,y,z
}

void Mach1DecodePositional::setListenerRotation(Mach1Point3D point)
{
	Mach1DecodePositionalCAPI_setListenerRotation(M1obj, point);
  /// Sets the device/camera's orientation with Euler angles
  ///
  /// - Parameters:
  ///     - Mach1Point3D Rotation: yaw,pitch,roll
}

void Mach1DecodePositional::setListenerRotationQuat(Mach1Point4D quat)
{
	Mach1DecodePositionalCAPI_setListenerRotationQuat(M1obj, quat);
  /// Sets the device/camera's orientation with Quaternion
  ///
  /// - Parameters:
  ///     - Mach1Point4D Rotation: Quaternion
}

void Mach1DecodePositional::setDecoderAlgoPosition(Mach1Point3D point)
{
	Mach1DecodePositionalCAPI_setDecoderAlgoPosition(M1obj, point);
  /// Sets the decode object's position in desired x,y,z space
  ///
  /// - Parameters:
  ///     - Mach1Point3D Rotation: x,y,z
}

void Mach1DecodePositional::setDecoderAlgoRotation(Mach1Point3D point)
{
	Mach1DecodePositionalCAPI_setDecoderAlgoRotation(M1obj, point);
  /// Sets the decode object's orientation with Euler angles
  ///
  /// - Parameters:
  ///     - Mach1Point3D Rotation: yaw,pitch,roll
}

void Mach1DecodePositional::setDecoderAlgoRotationQuat(Mach1Point4D quat)
{
	Mach1DecodePositionalCAPI_setDecoderAlgoRotationQuat(M1obj, quat);
  /// Sets the decode object's position in desired x,y,z space
  ///
  /// - Parameters:
  ///     - Mach1Point3D Rotation: x,y,z
}

void Mach1DecodePositional::setDecoderAlgoScale(Mach1Point3D point)
{
	Mach1DecodePositionalCAPI_setDecoderAlgoScale(M1obj, point);
  /// Sets the decode object's scale in desired x,y,z space
  ///
  /// - Parameters:
  ///     - Mach1Point3D Scale: x,y,z multiplier
}

void Mach1DecodePositional::evaluatePositionResults()
{
	Mach1DecodePositionalCAPI_evaluatePositionResults(M1obj);
}

#ifndef  __EMSCRIPTEN__ 
void Mach1DecodePositional::getCoefficients(float * result)
{
	Mach1DecodePositionalCAPI_getCoefficients(M1obj, result);
  /// Return the current coefficients to be applied to the audioplayer's volume
  ///
  /// - Remark: Result is returned back as the argument, an array of 18 floats is required as an input
}

void Mach1DecodePositional::getCoefficientsInterior(float * result)
{
	Mach1DecodePositionalCAPI_getCoefficientsInterior(M1obj, result);
  /// - Warning: Experimental feature
  /// - Remark: Part of BlendMode
  ///
  /// - Remark: Result is returned back as the argument, an array of 18 floats is required as an input
}
#endif

void Mach1DecodePositional::getCoefficients(std::vector<float>& result)
{
    if(result.size() < 18) result.resize(18);
	Mach1DecodePositionalCAPI_getCoefficients(M1obj, result.data());
  /// Return the current coefficients to be applied to the audioplayer's volume
  ///
  /// - Remark: Result is returned back as the argument, an array of 18 floats is required as an input
}

void Mach1DecodePositional::getCoefficientsInterior(std::vector<float>& result)
{
	Mach1DecodePositionalCAPI_getCoefficientsInterior(M1obj, result.data());
  /// - Warning: Experimental feature
  /// - Remark: Part of BlendMode
  ///
  /// - Remark: Result is returned back as the argument, an array of 18 floats is required as an input
}

float Mach1DecodePositional::getDist()
{
	return Mach1DecodePositionalCAPI_getDist(M1obj);
  /// Return a distance calculation of the position of the device/camera
  /// to the position of the m1obj
}

Mach1Point3D Mach1DecodePositional::getCurrentAngle()
{
	return Mach1DecodePositionalCAPI_getCurrentAngle(M1obj);
  /// Return the current orientation angle of the m1obj
}

Mach1Point3D Mach1DecodePositional::getCoefficientsRotation()
{
	return Mach1DecodePositionalCAPI_getCoefficientsRotation(M1obj);
  /// Return the current pivot rotation due to the m1obj comparison to the
  /// device/camera
}

void Mach1DecodePositional::setFilterSpeed(float filterSpeed)
{
	Mach1DecodePositionalCAPI_setFilterSpeed(M1obj, filterSpeed);
  /// Filter speed determines the amount of angle smoothing applied 
  /// to the orientation angles used for the Mach1DecodeCore class
  ///
  /// - Parameters: 
  ///     - value range: 0.0001 -> 1.0 (where 0.1 would be a slow filter
  ///     and 1.0 is no filter)
}

/* DEPRECATED START */
void Mach1DecodePositional::setUseFalloff(bool useFalloff)
{
  Mach1DecodePositionalCAPI_setUseFalloff(M1obj, useFalloff);
  /// Set distance attenuation calculations on that mach1decode object on/off
  ///
  /// - Remark: When off, distance remains 1.0
}
void Mach1DecodePositional::setFalloffCurve(float falloffCurve)
{
  Mach1DecodePositionalCAPI_setFalloffCurve(M1obj, falloffCurve);
  /// Set the current normalized distance per update for attenuation coefficient
  ///
  /// - Parameters: 
  ///     - Normalized range: 0.0 -> 1.0
}
void Mach1DecodePositional::setFalloffCurveBlendMode(float falloffCurveBlendMode)
{
  Mach1DecodePositionalCAPI_setFalloffCurveBlendMode(M1obj, falloffCurveBlendMode);
  /// Set the current normalized distance per update for blendmode's 
  /// internalattenuation coefficient
  ///
  /// - Parameters: 
  ///     - Normalized range: 0.0 -> 1.0
  ///
  /// - Warning: Experimental feature
  /// - Remark: Part of BlendMode
}
void Mach1DecodePositional::setUseClosestPointRotationMuteInside(bool useClosestPointRotationMuteInside)
{
  Mach1DecodePositionalCAPI_setUseClosestPointRotationMuteInside(M1obj, useClosestPointRotationMuteInside);
  /// Calculate the rotation to the decode object from it's center point
  /// or from the closest point to the object on a plane
  ///
  /// - Parameters:
  ///     - True: Closest point of plane of object for calculations of rotation
  ///     - False (default): center point of object for calculations of rotation 
}
void Mach1DecodePositional::setCameraPosition(Mach1Point3D point)
{
  Mach1DecodePositionalCAPI_setCameraPosition(M1obj, point);
  /// Sets the device/camera's position in desired x,y,z space
  ///
  /// - Parameters:
  ///     - Mach1Point3D Position: x,y,z
}
void Mach1DecodePositional::setCameraRotation(Mach1Point3D point)
{
  Mach1DecodePositionalCAPI_setCameraRotation(M1obj, point);
  /// Sets the device/camera's orientation with Euler angles
  ///
  /// - Parameters:
  ///     - Mach1Point3D Rotation: yaw,pitch,roll
}
void Mach1DecodePositional::setCameraRotationQuat(Mach1Point4D quat)
{
  Mach1DecodePositionalCAPI_setCameraRotationQuat(M1obj, quat);
  /// Sets the device/camera's orientation with Quaternion
  ///
  /// - Parameters:
  ///     - Mach1Point4D Rotation: Quaternion
}

#ifndef  __EMSCRIPTEN__ 
void Mach1DecodePositional::getVolumesWalls(float * result)
{
  Mach1DecodePositionalCAPI_getVolumesWalls(M1obj, result);
  /// Return the current coefficients to be applied to the audioplayer's volume
  ///
  /// - Remark: Result is returned back as the argument, an array of 18 floats is required as an input
}

void Mach1DecodePositional::getVolumesRoom(float * result)
{
  Mach1DecodePositionalCAPI_getVolumesRoom(M1obj, result);
  /// - Warning: Experimental feature
  /// - Remark: Part of BlendMode
  ///
  /// - Remark: Result is returned back as the argument, an array of 18 floats is required as an input
}
#endif

void Mach1DecodePositional::getVolumesWalls(std::vector<float>& result)
{
  Mach1DecodePositionalCAPI_getVolumesWalls(M1obj, result.data());
  /// Return the current coefficients to be applied to the audioplayer's volume
  ///
  /// - Remark: Result is returned back as the argument, an array of 18 floats is required as an input
}

void Mach1DecodePositional::getVolumesRoom(std::vector<float>& result)
{
  Mach1DecodePositionalCAPI_getVolumesRoom(M1obj, result.data());
  /// - Warning: Experimental feature
  /// - Remark: Part of BlendMode
  ///
  /// - Remark: Result is returned back as the argument, an array of 18 floats is required as an input
}

Mach1Point3D Mach1DecodePositional::getVolumeRotation()
{
  return Mach1DecodePositionalCAPI_getVolumeRotation(M1obj);
  /// Return the current pivot rotation due to the m1obj comparison to the
  /// device/camera
}
/* DEPRECATED END */
