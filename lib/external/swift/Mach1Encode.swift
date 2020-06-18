//  Mach1 Spatial SDK
//  Copyright © 2017-2020 Mach1. All rights reserved.

import Foundation
import SceneKit

public class Mach1Encode {
    var M1obj : UnsafeMutableRawPointer

    public init() {
        M1obj = Mach1EncodeCAPI_create()
    }

    deinit {
        Mach1EncodeCAPI_delete(M1obj)
    }

    public func getPoints() -> [SCNVector3] {
        var array = Array(repeating: SCNVector3(x: 0, y: 0, z: 0), count: Int(Mach1EncodeCAPI_getPointsCount(M1obj)))
        let pointsPtr = unsafeBitCast( Mach1EncodeCAPI_getPoints(M1obj), to: UnsafeMutablePointer<Float>?.self)
        for i in 0..<array.count {
            array[i].x = (pointsPtr! + 3 * i + 0).pointee
            array[i].y = (pointsPtr! + 3 * i + 1).pointee
            array[i].z = (pointsPtr! + 3 * i + 2).pointee
        }
        return array
        /// Retruns the control center reference point's normalized coordinate location (XYZ) 
        /// within the vector panning space
        ///
        /// - Parameters:
        ///     - X: front-back (0.0 (back) -> 1.0 (front))
        ///     - Y: up-down (0.0 (down) -> 1.0 (up))
        ///     - Z: left-right (0.0 (left) -> 1.0 (right))
    }

    public func getGains() -> [[Float]] {
        var array = Array(repeating: Array(repeating: Float(0), count: 8), count: Int(Mach1EncodeCAPI_getPointsCount(M1obj)))
        let gainsPtr = unsafeBitCast( Mach1EncodeCAPI_getGains(M1obj), to: UnsafeMutablePointer<UnsafeMutablePointer<Float>?>?.self)
        for i in 0..<Int(Mach1EncodeCAPI_getPointsCount(M1obj)){
            for j in 0..<8 {
                array[i][j] = ((gainsPtr! + i).pointee! + j).pointee
            }
        }
        return array
        /// Returns an array per input channel, resulting in an array of array results
        /// each internal array (per input channel) is a list of the needed coefficients to
        /// encode to a Mach1 VVBP/SPS format (depending on the selected `setOutputMode`)
        ///
        /// - Returns:
        ///     - 1st dimension of array is the number of input channels/points
        ///     - 2nd dimension of array is the resulting coefficient gains to be applied for encode
    }

    public func getPointsNames() -> [String] {
        var array = Array(repeating: String(), count: Int(Mach1EncodeCAPI_getPointsCount(M1obj)))
        let pointsPtr = unsafeBitCast( Mach1EncodeCAPI_getPointsNames(M1obj), to: UnsafeMutablePointer<UnsafeMutablePointer<Int8>?>?.self)
        for i in 0..<array.count {
            array[i] = String(cString: (pointsPtr! + i).pointee!) // String.cString((pointsPtr! + i).pointee)
        }
        return array
        /// Return the string name for each input channel/point
    }

    public func getGainsForInputChannelNamed(pointName: String) -> [Float] {
        var array: [Float] = Array(repeating: 0.0, count: 8)
        let gainsPtr = unsafeBitCast( Mach1EncodeCAPI_getGainsForInputChannelNamed(M1obj,  UnsafeMutablePointer<Int8>(mutating: (pointName as NSString).utf8String)), to: UnsafeMutablePointer<Float>?.self)
        for i in 0..<array.count {
            array[i] = (gainsPtr! + i).pointee
        }
        return array
        /// Returns array of gain coefficients for specificed input channel/point
    }

    public func generatePointResults() {
        Mach1EncodeCAPI_generatePointResults(M1obj)
        /// Function for controlling the update of all calculations of a Mach1Encode instance
        ///
        /// - Remark: Call when an update to Mach1 vector calculations is desired
    }

    public func getPointsCount() -> Int {
        let count = Mach1EncodeCAPI_getPointsCount(M1obj)
        return Int(count)
        /// Returns the number of input channels/points that Mach1Encode instance has
        ///
        /// - Returns:
        ///     - integer of number of input channels/points
    }

    public func getResultingCoeffsDecoded(decodeType: Mach1DecodeAlgoType, decodeResult: [Float] ) -> [Float] {
        let pointer: UnsafeMutablePointer = UnsafeMutablePointer(mutating: decodeResult)
        let volumesPtr = unsafeBitCast( Mach1EncodeCAPI_getResultingCoeffsDecoded(M1obj, decodeType, pointer), to: UnsafeMutablePointer<Float>?.self)

        var array: [Float] = Array(repeating: 0.0, count: 18)
        for i in 0..<array.count {
            array[i] = (volumesPtr! + i).pointee
        }
        return array
        /// A shorthand function for encoding->decoding audio object handling,
        /// useful preview UX so that a full input->mach1spatial_multichannel->stereo
        /// rendeering to disk isnt required and instead designs that stack decode results 
        /// live can more easily be created
        ///
        /// - Remark: Each input audio channel results a direct decode instead of the encode coefficients
    }

    public func getInputMode() -> Mach1EncodeInputModeType {
        let inputMode = Mach1EncodeCAPI_getInputMode(M1obj)
        return inputMode
        /// Returns the number of input channels/points that Mach1Encode instance has
        ///
        /// - Returns:
        ///     - integer of number of input channels/points
    }

    public func getOutputMode() -> Mach1EncodeOutputModeType {
        let outputMode = Mach1EncodeCAPI_getOutputMode(M1obj)
        return outputMode
        /// Returns the number of input channels/points that Mach1Encode instance has
        ///
        /// - Returns:
        ///     - integer of number of input channels/points
    }

    public func getInputChannelsCount() -> Int {
        let count = Mach1EncodeCAPI_getInputChannelsCount(M1obj)
        return Int(count)
        /// Returns the number of input channels/points that Mach1Encode instance has
        ///
        /// - Returns:
        ///     - integer of number of input channels/points
    }

    public func getOutputChannelsCount() -> Int {
        let count = Mach1EncodeCAPI_getOutputChannelsCount(M1obj)
        return Int(count)
        /// Returns the number of output channels/points that Mach1Encode instance has
        ///
        /// - Returns:
        ///     - integer of number of output channels/points
    }

    public func setInputMode(inputMode: Mach1EncodeInputModeType) {
        Mach1EncodeCAPI_setInputMode(M1obj, inputMode)
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
        ///     - INPUT_FOAFUM
        ///     - INPUT_2OAACN
        ///     - INPUT_2OAFUM
        ///     - INPUT_3OAACN
        ///     - INPUT_3OAFUMA
        ///     - INPUT_LCR
    }

    public func setOutputMode(outputMode: Mach1EncodeOutputModeType) {
        Mach1EncodeCAPI_setOutputMode(M1obj, outputMode)
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

    public func setAzimuth(azimuthFromMinus1To1: Float) {
        Mach1EncodeCAPI_setAzimuth(M1obj, azimuthFromMinus1To1)
        /// Sets the point(s) azimuth rotation of the vector space
        ///
        /// - Parameters:
        ///     - value range: -1.0 -> 1.0
    }

    public func setAzimuthDegrees(azimuthDegrees: Float) {
        Mach1EncodeCAPI_setAzimuthDegrees(M1obj, azimuthDegrees)
        /// Sets the point(s) azimuth rotation of the vector space
        ///
        /// - Parameters:
        ///     - value range: 0 -> 360
    }

    public func setAzimuthRadians(azimuthRadians: Float) {
        Mach1EncodeCAPI_setAzimuthRadians(M1obj, azimuthRadians)
        /// Sets the point(s) azimuth rotation of the vector space
        ///
        /// - Parameters:
        ///     - value range: -PI/2 -> PI/2
    }

    public func setDiverge(diverge: Float) {
        Mach1EncodeCAPI_setDiverge(M1obj, diverge)
        /// Sets the point(s) to/from center origin of the vector space
        ///
        /// - Parameters:
        ///     - value range: -1.0 -> 1.0
    }

    public func setElevation(elevationFromMinus1to1: Float) {
        Mach1EncodeCAPI_setElevation(M1obj, elevationFromMinus1to1)
        /// Sets the point(s) up/down the vector space
        ///
        /// - Parameters:
        ///     - value range: -1.0 -> 1.0
    }

    public func setElevationDegrees(elevationFromMinus90to90: Float) {
        Mach1EncodeCAPI_setElevationDegrees(M1obj, elevationFromMinus90to90)
        /// Sets the point(s) up/down the vector space
        ///
        /// - Parameters:
        ///     - value range: -90->90
    }

    public func setElevationRadians(elevationFromMinusHalfPItoHalfPI: Float) {
        Mach1EncodeCAPI_setElevationRadians(M1obj, elevationFromMinusHalfPItoHalfPI)
        /// Sets the point(s) up/down the vector space
        ///
        /// - Parameters:
        ///     - value range: -PI/2 -> PI/2
    }

    public func setIsotropicEncode(setIsotropicEncode: Bool) {
        Mach1EncodeCAPI_setIsotropicEncode(M1obj, setIsotropicEncode)
        /// Sets both stereo points rotate in relation to the
        /// center point between them so that they always triangulate
        /// toward center of the cuboid
        ///
        /// - Remark: Default is true
    }

    public func setOrbitRotation(orbitRotationFromMinusOnetoOne: Float) {
        Mach1EncodeCAPI_setOrbitRotation(M1obj, orbitRotationFromMinusOnetoOne)
        /// Sets the two stereo points around the axis of the center point between them
        ///
        /// - Parameters:
        ///     - value range: -1.0 -> 1.0
    }

    public func setOrbitRotationDegrees(orbitRotationDegrees: Float) {
        Mach1EncodeCAPI_setOrbitRotationDegrees(M1obj, orbitRotationDegrees)
        /// Sets the two stereo points around the axis of the center point between them
        ///
        /// - Parameters:
        ///     - value range: -180.0->180.0
    }

    public func setOrbitRotationRadians(orbitRotationRadians: Float) {
        Mach1EncodeCAPI_setOrbitRotationRadians(M1obj, orbitRotationRadians)
        /// Sets the two stereo points around the axis of the center point between them
        ///
        /// - Parameters:
        ///     - value range: -PI -> PI
    }

    public func setStereoSpread(setStereoSpread: Float) {
        Mach1EncodeCAPI_setStereoSpread(M1obj, setStereoSpread)
        /// Sets the space between the two stereo points
        ///
        /// - Parameters:
        ///     - value range: 0.0 -> 1.0
    }

    public func setAutoOrbit(setAutoOrbit: Bool) {
        Mach1EncodeCAPI_setAutoOrbit(M1obj, setAutoOrbit)
        /// Sets encoding behavior acts evenly with distribution
        /// across all azimuth/rotation angles and all altitude/pitch angles
        ///
        /// - Remark: Default is true
    }

/*
DEPRECATED START
 */
    public func getResultingVolumesDecoded(decodeType: Mach1DecodeAlgoType, decodeResult: [Float] ) -> [Float] {
        let pointer: UnsafeMutablePointer = UnsafeMutablePointer(mutating: decodeResult)
        let volumesPtr = unsafeBitCast( Mach1EncodeCAPI_getResultingVolumesDecoded(M1obj, decodeType, pointer), to: UnsafeMutablePointer<Float>?.self)

        var array: [Float] = Array(repeating: 0.0, count: 18)
        for i in 0..<array.count {
            array[i] = (volumesPtr! + i).pointee
        }
        return array
        /// A shorthand function for encoding->decoding audio object handling,
        /// useful preview UX so that a full input->mach1spatial_multichannel->stereo
        /// rendeering to disk isnt required and instead designs that stack decode results 
        /// live can more easily be created
        ///
        /// - Remark: Each input audio channel results a direct decode instead of the encode coefficients
    }

    public func setRotation(rotation: Float) {
        Mach1EncodeCAPI_setRotation(M1obj, rotation)
        /// Sets the point(s) around the center origin of the vector space
        ///
        /// - Parameters:
        ///     - value range: 0.0 -> 1.0 (0->360)
    }

    public func setPitch(pitch: Float) {
        Mach1EncodeCAPI_setPitch(M1obj, pitch)
        /// Sets the point(s) up/down the vector space
        ///
        /// - Parameters:
        ///     - value range: -1.0 -> 1.0 (-90->90)
    }

    public func setStereoRotate(setStereoRotate: Float) {
        Mach1EncodeCAPI_setStereoRotate(M1obj, setStereoRotate)
        /// Sets the two stereo points around the axis of the center point between them
        ///
        /// - Parameters:
        ///     - value range: -180.0->180.0
    }
/*
DEPRECATED END
 */
}
