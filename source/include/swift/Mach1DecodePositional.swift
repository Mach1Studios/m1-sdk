//  Mach1 Spatial SDK
//  Copyright © 2017-2020 Mach1. All rights reserved.

import Foundation

public class Mach1DecodePositional {
    var M1obj : UnsafeMutableRawPointer
    
    public init() {
        M1obj = Mach1DecodePositionalCAPI_create()
    }

    deinit {
        Mach1DecodePositionalCAPI_delete(M1obj)
    }
    
    public func setPlatformType(type: Mach1PlatformType) {
        Mach1DecodePositionalCAPI_setPlatformType(M1obj, type)
        /// Set the device's angle order and convention if applicable
        ///
        /// - Parameters:
        ///     - Mach1PlatformDefault = 0
        ///     - Mach1PlatformUnity = 1
        ///     - Mach1PlatformUE = 2
        ///     - Mach1PlatformOfEasyCam = 3
        ///     - Mach1PlatformAndroid = 4
        ///     - Mach1PlatformiOS = 5
        ///     - Mach1PlatformiOSTableTop_ZVertical = 6
        ///     - Mach1PlatformiOSPortraitHandheld_YVertical = 7
        ///     - Mach1PlatformiOSPortrait_YawOnly = 8
    }
    public func setDecodeAlgoType(newAlgorithmType: Mach1DecodeAlgoType) {
        Mach1DecodePositionalCAPI_setDecodeAlgoType(M1obj, newAlgorithmType)
        /// Set the decoding algorithm
        ///
        /// - Parameters:
        ///     - Mach1DecodeAlgoSpatial_8 (default spatial | 8 channels)
        ///     - Mach1DecodeAlgoHorizon_4 (compass / yaw | 4 channels)
        ///     - Mach1DecodeAlgoHorizonPairs (compass / yaw | 4x stereo mastered pairs)
        ///     - Mach1DecodeAlgoSpatialPairs (experimental periphonic pairs | 8x stereo mastered pairs)
        ///     - Mach1DecodeAlgoSpatial_12 (higher order spatial | 12 channels)
        ///     - Mach1DecodeAlgoSpatial_14 (higher order spatial | 14 channels)
        ///     - Mach1DecodeAlgoSpatial_18 (higher order spatial | 18 channels)
        ///     - Mach1DecodeAlgoSpatial_22 (higher order spatial | 20 channels)
        ///     - Mach1DecodeAlgoSpatial_32 (higher order spatial | 32 channels)
        ///     - Mach1DecodeAlgoSpatial_36 (higher order spatial | 36 channels)
        ///     - Mach1DecodeAlgoSpatial_48 (higher order spatial | 48 channels)
        ///     - Mach1DecodeAlgoSpatial_60 (higher order spatial | 60 channels)
    }
    
    public func setUseBlendMode(useBlendMode: Bool) {
        Mach1DecodePositionalCAPI_setUseBlendMode(M1obj, useBlendMode)
        /// - Warning: Experimental feature
        /// - Remark: Part of BlendMode
    }

    public func setIgnoreTopBottom(ignoreTopBottom: Bool) {
        Mach1DecodePositionalCAPI_setIgnoreTopBottom(M1obj, ignoreTopBottom)
        /// - Warning: Experimental feature
        /// - Remark: Part of BlendMode
    }
    
    public func setMuteWhenOutsideObject(muteWhenOutsideObject: Bool) {
        Mach1DecodePositionalCAPI_setMuteWhenOutsideObject(M1obj, muteWhenOutsideObject)
        /// Mute mach1decode object (all coefficifient results becomes 0) 
        /// when outside the positional reference shape/point
    }
    
    public func setMuteWhenInsideObject(muteWhenInsideObject: Bool) {
        Mach1DecodePositionalCAPI_setMuteWhenInsideObject(M1obj, muteWhenInsideObject)
        /// Mute mach1decode object (all coefficifient results becomes 0) 
        /// when inside the positional reference shape/point
    }
    
    public func setUseAttenuation(useAttenuation: Bool) {
        Mach1DecodePositionalCAPI_setUseAttenuation(M1obj, useAttenuation)
        /// Set distance attenuation calculations on that mach1decode object on/off
        ///
        /// Remark: When off, distance remains 0.0
    }

    public func setAttenuationCurve(attenuationCurve: Float) {
        Mach1DecodePositionalCAPI_setAttenuationCurve(M1obj, attenuationCurve)
        /// Set the current normalized distance per update for attenuation coefficient
        ///
        /// - Parameters: 
        ///     - Normalized range: 0.0 -> 1.0
    }

    public func setAttenuationCurveBlendMode(attenuationCurveBlendMode: Float) {
        Mach1DecodePositionalCAPI_setAttenuationCurveBlendMode(M1obj, attenuationCurveBlendMode)
        /// Set the current normalized distance per update for blendmode's 
        /// internalattenuation coefficient
        ///
        /// - Parameters: 
        ///     - Normalized range: 0.0 -> 1.0
        ///
        /// - Warning: Experimental feature
        /// - Remark: Part of BlendMode
    }
    
    public func setUsePlaneCalculation(bool usePlaneCalculation: Bool) {
        Mach1DecodePositionalCAPI_setUsePlaneCalculation(M1obj, usePlaneCalculation)
        /// Calculate the rotation to the decode object from it's center point
        /// or from the closest point to the object on a plane
        ///
        /// - Parameters:
        ///     - True: Closest point of plane of object for calculations of rotation
        ///     - False (default): center point of object for calculations of rotation 
    }
    
    public func setUseYawForRotation(bool useYawForRotation: Bool) {
        Mach1DecodePositionalCAPI_setUseYawForRotation(M1obj, useYawForRotation)
        /// Set use/ignore Yaw angle rotation results from pivoting positionally
        ///
        /// Remark: Default is true
    }

    public func setUsePitchForRotation(bool usePitchForRotation: Bool) {
        Mach1DecodePositionalCAPI_setUsePitchForRotation(M1obj, usePitchForRotation)
        /// Set use/ignore Pitch angle rotation results from pivoting positionally
        ///
        /// - Remark: Default is true
    }

    public func setUseRollForRotation(bool useRollForRotation: Bool) {
        Mach1DecodePositionalCAPI_setUseRollForRotation(M1obj, useRollForRotation)
        /// Set use/ignore Roll angle rotation results from pivoting positionally
        ///
        /// - Remark: Default is true
    }
    
    public func setListenerPosition(point: Mach1Point3D) {
        Mach1DecodePositionalCAPI_setListenerPosition(M1obj, point)
        /// Sets the device/camera's position in desired x,y,z space
        ///
        /// - Parameters:
        ///     - Mach1Point3D Position: x,y,z
    }

    public func setListenerRotation(point: Mach1Point3D) {
        Mach1DecodePositionalCAPI_setListenerRotation(M1obj, point)
        /// Sets the device/camera's orientation with Euler angles
        ///
        /// - Parameters:
        ///     - Mach1Point3D Rotation: yaw,pitch,roll
    }

    public func setListenerRotationQuat(point: Mach1Point4D) {
        Mach1DecodePositionalCAPI_setListenerRotationQuat(M1obj, point)
        /// Sets the device/camera's orientation with Quaternion
        ///
        /// - Parameters:
        ///     - Mach1Point4D Rotation: Quaternion
    }

    public func setDecoderAlgoPosition(point: Mach1Point3D) {
        Mach1DecodePositionalCAPI_setDecoderAlgoPosition(M1obj, point)
        /// Sets the decode object's position in desired x,y,z space
        ///
        /// - Parameters:
        ///     - Mach1Point3D Rotation: x,y,z
    }

    public func setDecoderAlgoRotation(point: Mach1Point3D) {
        Mach1DecodePositionalCAPI_setDecoderAlgoRotation(M1obj, point)
        /// Sets the decode object's orientation with Euler angles
        ///
        /// - Parameters:
        ///     - Mach1Point3D Rotation: yaw,pitch,roll
    }

    public func setDecoderAlgoRotationQuat(point: Mach1Point4D) {
        Mach1DecodePositionalCAPI_setDecoderAlgoRotationQuat(M1obj, point)
        /// Sets the decode object's orientation with Quaternion
        ///
        /// - Parameters:
        ///     - Mach1Point4D Rotation: Quaternion
    }

    public func setDecoderAlgoScale(point: Mach1Point3D) {
        Mach1DecodePositionalCAPI_setDecoderAlgoScale(M1obj, point)
        /// Sets the decode object's scale in desired x,y,z space
        ///
        /// - Parameters:
        ///     - Mach1Point3D Scale: x,y,z multiplier
    }
    
    public func evaluatePositionResults() {
        Mach1DecodePositionalCAPI_evaluatePositionResults(M1obj)
    }
    
    public func getCoefficients(result: inout [Float]) {
        let pointer: UnsafeMutablePointer< Float > = UnsafeMutablePointer(&result)
        Mach1DecodePositionalCAPI_getCoefficients(M1obj, pointer);
        /// Return the current coefficients to be applied to the audioplayer's volume
        ///
        /// - Remark: Result is returned back as the argument, an array of 18 floats is required as an input
    }

    public func getCoefficientsInterior(result: inout [Float]) {
        let pointer: UnsafeMutablePointer< Float > = UnsafeMutablePointer(&result)
        Mach1DecodePositionalCAPI_getCoefficientsInterior(M1obj, pointer);
        /// - Warning: Experimental feature
        /// - Remark: Part of BlendMode
        ///
        /// - Remark: Result is returned back as the argument, an array of 18 floats is required as an input
    }

    public func getDist() -> Float {
        return Mach1DecodePositionalCAPI_getDist(M1obj)
        /// Return a distance calculation of the position of the device/camera
        /// to the position of the m1obj
    }
    
    public func getCurrentAngle() -> Mach1Point3D {
        return Mach1DecodePositionalCAPI_getCurrentAngle(M1obj)
        /// Return the current orientation angle of the m1obj
    }
    
    public func getCoefficientsRotation() -> Mach1Point3D {
        return Mach1DecodePositionalCAPI_getCoefficientsRotation(M1obj)
        /// Return the current pivot rotation due to the m1obj comparison to the
        /// device/camera
    }
    
    public func setFilterSpeed(filterSpeed: Float) {
        Mach1DecodePositionalCAPI_setFilterSpeed(M1obj, filterSpeed)
        /// Filter speed determines the amount of angle smoothing applied 
        /// to the orientation angles used for the Mach1DecodeCore class
        ///
        /// - Parameters: 
        ///     - value range: 0.0001 -> 1.0 (where 0.1 would be a slow filter
        ///     and 1.0 is no filter)
    }

/* DEPRECATED START*/
    @available(*, deprecated, message: "Please use setUseAttenuation instead")
    public func setUseFalloff(useFalloff: Bool) {
        Mach1DecodePositionalCAPI_setUseFalloff(M1obj, useFalloff)
        /// Set distance attenuation calculations on that mach1decode object on/off
        ///
        /// - Remark: When off, distance remains 1.0
    }
    @available(*, deprecated, message: "Please use setAttenuationCurve instead")
    public func setFalloffCurve(falloffCurve: Float) {
        Mach1DecodePositionalCAPI_setFalloffCurve(M1obj, falloffCurve)
    }
    @available(*, deprecated, message: "Please use setAttenuationCurveBlendMode instead")
    public func setFalloffCurveBlendMode(falloffCurveBlendMode: Float) {
        Mach1DecodePositionalCAPI_setFalloffCurveBlendMode(M1obj, falloffCurveBlendMode)
        /// - Warning: Experimental feature
        /// - Remark: Part of BlendMode
    }
    @available(*, deprecated, message: "Please use setUsePlaneCalculation instead")
    public func setUseClosestPointRotationMuteInside(bool useClosestPointRotationMuteInside: Bool) {
        Mach1DecodePositionalCAPI_setUseClosestPointRotationMuteInside(M1obj, useClosestPointRotationMuteInside)
    }
    @available(*, deprecated, message: "Please use setListenerPosition instead")
    public func setCameraPosition(point: Mach1Point3D) {
       Mach1DecodePositionalCAPI_setCameraPosition(M1obj, point)
        /// Sets the device/camera's position in desired x,y,z space
        ///
        /// - Parameters:
        ///     - Mach1Point3D Position: x,y,z
    }
    @available(*, deprecated, message: "Please use setListenerRotation instead")
    public func setCameraRotation(point: Mach1Point3D) {
        Mach1DecodePositionalCAPI_setCameraRotation(M1obj, point)
        /// Sets the device/camera's orientation with Euler angles
        ///
        /// - Parameters:
        ///     - Mach1Point3D Rotation: yaw,pitch,roll
    }
    @available(*, deprecated, message: "Please use setListenerRotationQuat instead")
    public func setCameraRotationQuat(point: Mach1Point4D) {
        Mach1DecodePositionalCAPI_setCameraRotationQuat(M1obj, point)
        /// Sets the device/camera's orientation with Quaternion
        ///
        /// - Parameters:
    }
    @available(*, deprecated, message: "Please use getCoefficients instead")
    public func getVolumesWalls(result: inout [Float]) {
        let pointer: UnsafeMutablePointer< Float > = UnsafeMutablePointer(&result)
        Mach1DecodePositionalCAPI_getVolumesWalls(M1obj, pointer);
        /// Return the current coefficients to be applied to the audioplayer's volume
        ///
        /// - Remark: Result is returned back as the argument, an array of 18 floats is required as an input
    }
    @available(*, deprecated, message: "Please use getCoefficientsInterior instead")
    public func getVolumesRoom(result: inout [Float]) {
         let pointer: UnsafeMutablePointer< Float > = UnsafeMutablePointer(&result)
        Mach1DecodePositionalCAPI_getVolumesRoom(M1obj, pointer);
        /// - Warning: Experimental feature
        /// - Remark: Part of BlendMode
        ///
        /// - Remark: Result is returned back as the argument, an array of 18 floats is required as an input
    }
    @available(*, deprecated, message: "Please use getCoefficientsRotation instead")
    public func getVolumeRotation() -> Mach1Point3D {
        return Mach1DecodePositionalCAPI_getVolumeRotation(M1obj)
        /// Return the current pivot rotation due to the m1obj comparison to the
        /// device/camera
    }
/* DEPRECATED END*/

}