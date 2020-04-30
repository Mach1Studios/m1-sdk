//  Mach1 Spatial SDK
//  Copyright © 2017-2020 Mach1. All rights reserved.

import Foundation

public class Mach1Decode {
    var M1obj : UnsafeMutableRawPointer
    
    public init() {
        M1obj = Mach1DecodeCAPI_create()
    }
    
    deinit {
        Mach1DecodeCAPI_delete(M1obj)
    }
    
    public func setPlatformType(type: Mach1PlatformType) {
        Mach1DecodeCAPI_setPlatformType(M1obj, type)
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
    
    public func setDecodeAlgoType(newAlgorithmType: Mach1DecodeAlgoType) {
        Mach1DecodeCAPI_setDecodeAlgoType(M1obj, newAlgorithmType)
        /// Set the decoding algorithm
        ///
        /// - Parameters:
        ///     - Mach1DecodeAlgoSpatial = 0 (default spatial | 8 channels)
        ///     - Mach1DecodeAlgoAltSpatial = 1 (periphonic spatial | 8 channels)
        ///     - Mach1DecodeAlgoHorizon = 2 (compass / yaw | 4 channels)
        ///     - Mach1DecodeAlgoHorizonPairs = 3 (compass / yaw | 4x stereo mastered pairs)
        ///     - Mach1DecodeAlgoSpatialPairs = 4 (experimental periphonic pairs | 8x stereo mastered pairs)
        ///     - Mach1DecodeAlgoSpatialPlus = 5 (higher order spatial | 12 channels)
        ///     - Mach1DecodeAlgoSpatialPlusPlus = 6 (higher order spatial | 14 channels)
        ///     - Mach1DecodeAlgoSpatialExt = 7 (higher order spatial | 16 channels)
        ///     - Mach1DecodeAlgoSpatialExtPlus = 8 (higher order spatial | 18 channels)
    }

    public func getFormatChannelCount() -> Int {
        let count = Mach1DecodeCAPI_getFormatChannelCount(M1obj)
        return Int(count)
        /// Returns the number of channels for format to be decoded
    }

    public func setRotation(newRotationFromMinusOnetoOne: Mach1Point3D) {
        Mach1DecodeCAPI_setRotation(M1obj, newRotationFromMinusOnetoOne)
        /// Set current buffer/sample intended decoding orientation YPR.
        ///
        /// - Parameters: 
        ///     - Yaw: float for device/listener yaw angle: [Range: -1.0 -> 1.0]
        ///     - Pitch: float for device/listener pitch angle: [Range: -1.0 -> 1.0]
        ///     - Roll: float for device/listener roll angle: [Range: -1.0 -> 1.0]
    }

    public func setRotationDegrees(newRotationDegrees: Mach1Point3D) {
        Mach1DecodeCAPI_setRotationDegrees(M1obj, newRotationDegrees)
        /// Set current buffer/sample intended decoding orientation YPR.
        ///
        /// - Parameters: 
        ///     - Yaw: float for device/listener yaw angle: [Range: 0->360 | -180->180]
        ///     - Pitch: float for device/listener pitch angle: [Range: -90->90]
        ///     - Roll: float for device/listener roll angle: [Range: -90->90]
    }

    public func setRotationRadians(newRotationRadians: Mach1Point3D) {
        Mach1DecodeCAPI_setRotationRadians(M1obj, newRotationRadians)
        /// Set current buffer/sample intended decoding orientation YPR in radians.
        ///
        /// - Parameters: 
        ///     - Yaw: float for device/listener yaw angle: [Range: 0->2PI | -PI->PI]
        ///     - Pitch: float for device/listener pitch angle: -PI/2 -> PI/2
        ///     - Roll: float for device/listener roll angle: -PI/2 -> PI/2
    }

    public func setRotationQuat(newRotationQuat: Mach1Point4D) {
        Mach1DecodeCAPI_setRotationQuat(M1obj, newRotationQuat)
        /// Set current buffer/sample intended decoding orientation YPR in quaternion.
        ///
        /// - Parameters: 
        ///     - W: float for device/listener W: [Range: -1.0->1.0]
        ///     - X: float for device/listener X: [Range: -1.0->1.0]
        ///     - Y: float for device/listener Y: [Range: -1.0->1.0]
        ///     - Z: float for device/listener Z: [Range: -1.0->1.0]
    }

    public func setFilterSpeed(filterSpeed: Float) {
        Mach1DecodeCAPI_setFilterSpeed(M1obj, filterSpeed)
        /// Filter speed determines the amount of angle smoothing applied 
        /// to the orientation angles used for the Mach1DecodeCore class
        ///
        /// - Parameters: 
        ///     - value range: 0.0001 -> 1.0 (where 0.1 would be a slow filter
        ///     and 1.0 is no filter)
    }
    
    public func beginBuffer() {
        Mach1DecodeCAPI_beginBuffer(M1obj)
        /// Call this function before reading from the Mach1Decode buffer
    }
    
    public func endBuffer() {
        Mach1DecodeCAPI_endBuffer(M1obj)
        /// Call this function after reading from the Mach1Decode buffer
    }
    
    public func getCurrentTime() -> Int {
        let t = Mach1DecodeCAPI_getCurrentTime(M1obj)
        return t
        /// Returns the current elapsed time in milliseconds (ms) since Mach1Decode object's creation
    }
    
    public func getLog() -> String {
        let str = String(cString: UnsafePointer(Mach1DecodeCAPI_getLog(M1obj)))
        return str
        /// Returns a string of the last log message (or empty string if none) from Mach1DecodeCAPI binary library
    }
    
    public func decode(Yaw: Float, Pitch: Float, Roll: Float, bufferSize: Int = 0, sampleIndex: Int = 0) -> [Float] {
        let rotation = Mach1Point3D(x: Yaw, y: Pitch, z: Roll)
        setRotationDegrees(newRotationDegrees: rotation)
        return decodeCoeffs(bufferSize: bufferSize, sampleIndex: sampleIndex)
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

    public func decodeCoeffs(bufferSize: Int = 0, sampleIndex: Int = 0) -> [Float] {
        var array: [Float] = Array(repeating: 0.0, count: getFormatChannelCount())
        Mach1DecodeCAPI_decodeCoeffs(M1obj, &array, CInt(bufferSize), CInt(sampleIndex))
        return array
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

}
