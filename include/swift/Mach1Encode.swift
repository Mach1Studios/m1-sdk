import Foundation

public class Mach1Encode {
    var M1obj : UnsafeMutableRawPointer

    public init() {
        M1obj = Mach1EncodeCAPI_create()
    }

    deinit {
        Mach1EncodeCAPI_delete(M1obj)
    }

    public func getPoints() {
        Mach1EncodeCAPI_getPoints(M1obj)
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
    }
    
    public func getResultingVolumesDecoded(decodeType: Mach1DecodeAlgoType, decodeResult: [Float] ) -> [Float] {
        let pointer: UnsafeMutablePointer = UnsafeMutablePointer(mutating: decodeResult)
        let volumesPtr = unsafeBitCast( Mach1EncodeCAPI_getResultingVolumesDecoded(M1obj, decodeType, pointer), to: UnsafeMutablePointer<Float>?.self)

        var array: [Float] = Array(repeating: 0.0, count: 18)
        for i in 0..<array.count {
            array[i] = (volumesPtr! + i).pointee
        }
        return array
    }
    
    public func getPointsNames() {
        Mach1EncodeCAPI_getPointsNames(M1obj)
    }

    public func getGainsForInputChannelNamed(pointName: String) {
        Mach1EncodeCAPI_getGainsForInputChannelNamed(M1obj,  UnsafeMutablePointer<Int8>(mutating: (pointName as NSString).utf8String))
    }

    public func generatePointResults() {
        Mach1EncodeCAPI_generatePointResults(M1obj)
    }

    public func getPointsCount() -> Int {
        let count = Mach1EncodeCAPI_getPointsCount(M1obj)
        return Int(count)
    }

    public func setInputMode(inputMode: Mach1EncodeInputModeType) {
        Mach1EncodeCAPI_setInputMode(M1obj, inputMode)
        /// Sets the number of input streams to be positioned as points
        ///
        /// - Parameters:
        ///     - INPUT_MONO
        ///     - INPUT_STEREO
        ///     - INPUT_QUAD
        ///     - INPUT_AFORMAT
        ///     - INPUT_BFORMAT
    }

    public func setOutputMode(outputMode: Mach1EncodeOutputModeType) {
        Mach1EncodeCAPI_setOutputMode(M1obj, outputMode)
        /// Sets the output spatial format, Mach1Spatial or Mach1Horizon
        ///
        /// - Parameters:
        ///     - OUTPUT_4CH (Mach1Horizon) [Yaw only]
        ///     - OUTPUT_8CH (Mach1Spatial) [Yaw, Pitch, Roll]
    }

    public func setRotation(rotation: Float) {
        Mach1EncodeCAPI_setRotation(M1obj, rotation)
        /// Sets the point(s) around the center origin of the vector space
        ///
        /// - Parameters:
        ///     - value range: 0.0 -> 1.0 (0->360)
    }

    public func setDiverge(diverge: Float) {
        Mach1EncodeCAPI_setDiverge(M1obj, diverge)
        /// Sets the point(s) to/from center origin of the vector space
        ///
        /// - Parameters:
        ///     - value range: -1.0 -> 1.0
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
        /// Remark: Default is true
    }

    public func setIsotropicEncode(setIsotropicEncode: Bool) {
        Mach1EncodeCAPI_setIsotropicEncode(M1obj, setIsotropicEncode)
        /// Sets both stereo points rotate in relation to the
        /// center point between them so that they always triangulate
        /// toward center of the cuboid
        ///
        /// Remark: Default is true
    }

}
