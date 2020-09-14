//  Mach1 Spatial SDK
//  Copyright © 2017-2020 Mach1. All rights reserved.

import Foundation

public class Mach1Transcode {
    var M1obj : UnsafeMutableRawPointer
    
    public init() {
        M1obj = Mach1TranscodeCAPI_create()
    }
    
    deinit {
        Mach1TranscodeCAPI_delete(M1obj)
    }
    
    public func getInputNumChannels() -> Int {
        let i = Mach1TranscodeCAPI_getInputNumChannels(M1obj)
        return Int(i)
        /// Returns the number of channels for indicated input format
        ///
        /// - Parameters:
        ///     - integer of number of output channels
    }
    
    public func getOutputNumChannels() -> Int {
        let i = Mach1TranscodeCAPI_getOutputNumChannels(M1obj)
        return Int(i)
        /// Returns the number of channels for indicated output format
        ///
        /// - Parameters:
        ///     - integer of number of output channels
    }
    
    public func getFormatFromString(str:String) -> Mach1TranscodeFormatType {
        let pointer = UnsafeMutablePointer<Int8>(mutating: (str as NSString).utf8String)
        return Mach1TranscodeCAPI_getFormatFromString(M1obj, pointer)
    }
    
    public func getFormatName(fmt:Mach1TranscodeFormatType) -> String {
        let cStr = Mach1TranscodeCAPI_getFormatName(M1obj, fmt)
        return String(cString: cStr!)
    }
    
    public func processNormalization(bufs: UnsafeMutablePointer<UnsafeMutablePointer<Float>?>!, numSamples:Int) -> Float {
        let pointer: UnsafeMutablePointer = UnsafeMutablePointer(mutating: bufs)
        return Mach1TranscodeCAPI_processNormalization(M1obj, pointer, CInt(numSamples))
    }
    
    public func processMasterGain(bufs: UnsafeMutablePointer<UnsafeMutablePointer<Float>?>!, numSamples:Int, masterGain:Float) {
        let pointer: UnsafeMutablePointer = UnsafeMutablePointer(mutating: bufs)
        Mach1TranscodeCAPI_processMasterGain(M1obj, pointer, CInt(numSamples), masterGain)
        /// Applys an input gain to the output soundfield
        ///
        /// - Parameters: 
        ///     - input buffer
        ///     - integer of input number of samples
        ///     - float for gain multiplier
    }
    
    public func db2level(db:Float) -> Float {
        return Mach1TranscodeCAPI_db2level(M1obj, db)
    }
    
    public func level2db(level:Float) -> Float {
        return Mach1TranscodeCAPI_level2db(M1obj, level)
    }
    
    public func setLFESub(subChannelIndices:[Int], sampleRate:Int) {
        let subChannelIndicesArray = subChannelIndices.map { CInt($0) }
        let pointer: UnsafeMutablePointer = UnsafeMutablePointer(mutating: subChannelIndicesArray)
        Mach1TranscodeCAPI_setLFESub(M1obj, pointer, CInt(subChannelIndices.count), CInt(sampleRate))
        /// Applys a low pass filter (LPF) to each indicated channel index of the input format and soundfield
        ///
        /// - Parameters: 
        ///     - integer inputs for channel indices to be processed
        ///     - integer for samplerate 
    }
    
    public func setSpatialDownmixer(corrThreshold:Float) {
        Mach1TranscodeCAPI_setSpatialDownmixer(M1obj, corrThreshold)
        /// Sets the threshold float for `getSpatialDownmixerPossibility` calculation
        ///
        /// - Parameters: 
        ///     - float from 0.0 to 1.0 where 0.0 no difference and incrementing to 1.0 is more difference
    }
    
    public func getSpatialDownmixerPossibility() -> Bool {
        return Mach1TranscodeCAPI_getSpatialDownmixerPossibility(M1obj)
        /// Returns true if compared signals are less than the `setSpatialDownmixer(corrThreshold)`
        ///
        /// - Returns: 
        ///     - boolean returned true if the compared signal difference is less then the `setSpatialDownmixer(corrThreshold)`
        ///
        /// - Remarks:
        ///     - when true; transcodings that are set to ouput to `Mach1Spatial` will process an additional conversion to `Mach1Horizon` 
    }
    
    public func setInputFormat(inFmt:Mach1TranscodeFormatType) {
        Mach1TranscodeCAPI_setInputFormat(M1obj, inFmt)
        /// Sets the input format for transcoding from the preset Mach1Transcode options
        ///
        /// Remarks:
        ///     View the current list of Mach1Transcode preset formats here: https://dev.mach1.tech/#formats-supported
    }
    
    public func setInputFormatADM(inXml : String) {
        let pointer = UnsafeMutablePointer<Int8>(mutating: (inXml as NSString).utf8String)
        Mach1TranscodeCAPI_setInputFormatADM(M1obj, pointer)
        /// Sets the input format for transcoding from the parsed ADM metadata within the audiofile
    }
    
    public func setInputFormatTTJson(strJson : String) {
        let pointer = UnsafeMutablePointer<Int8>(mutating: (strJson as NSString).utf8String)
        Mach1TranscodeCAPI_setInputFormatTTJson(M1obj, pointer)
        /// Sets the input format for transcoding from an external JSON source
        ///
        /// Remarks:
        ///     View the JSON spec for describing a format here: https://dev.mach1.tech/#json-descriptions
    }
    
    public func setInputFormatTTPoints(points : [Mach1Point3D]) {
        let pointer = UnsafeMutablePointer<Mach1Point3D>(mutating: points)
        Mach1TranscodeCAPI_setInputFormatTTPoints(M1obj, pointer, CInt(points.count));
        /// Sets the output format for transcoding from TT directly
        ///
        /// Remarks:
        ///     View the JSON spec for describing a format here: https://dev.mach1.tech/#json-descriptions
    }
    
    public func setOutputFormat(outFmt:Mach1TranscodeFormatType) {
        Mach1TranscodeCAPI_setOutputFormat(M1obj, outFmt)
        /// Sets the output format for transcoding from the preset Mach1Transcode options
        ///
        /// Remarks:
        ///     View the current list of Mach1Transcode preset formats here: https://dev.mach1.tech/#formats-supported
    }
    
    public func setOutputFormatTTJson (strJson : String) {
        let pointer = UnsafeMutablePointer<Int8>(mutating: (strJson as NSString).utf8String)
        Mach1TranscodeCAPI_setOutputFormatTTJson(M1obj, pointer)
        /// Sets the output format for transcoding from an external JSON source
        ///
        /// Remarks:
        ///     View the JSON spec for describing a format here: https://dev.mach1.tech/#json-descriptions
    }
    
    public func setOutputFormatTTPoints(points : [Mach1Point3D]) {
        let pointer = UnsafeMutablePointer<Mach1Point3D>(mutating: points)
        Mach1TranscodeCAPI_setOutputFormatTTPoints(M1obj, pointer, CInt(points.count));
        /// Sets the output format for transcoding from TT directly
        ///
        /// Remarks:
        ///     View the JSON spec for describing a format here: https://dev.mach1.tech/#json-descriptions
    }
    
    public func processConversionPath() -> Bool {
        return Mach1TranscodeCAPI_processConversionPath(M1obj)
        /// Use this function to control when to call for calculating the format transcoding calculations
    }
    
    public func getMatrixConversion() -> [[Float]] {
        let matrix = [Float](repeating: 0.0, count: getInputNumChannels() * getOutputNumChannels())
        let pointer = UnsafeMutablePointer<Float>(mutating: matrix)
        Mach1TranscodeCAPI_getMatrixConversion(M1obj, pointer)
       
        var vec = [[Float]](repeating: [], count: getOutputNumChannels())
        for i in 0..<getOutputNumChannels() {
            vec[i] = [Float](repeating: 0.0, count: getInputNumChannels())
            for j in 0..<getInputNumChannels() {
                vec[i][j] = matrix[i * getInputNumChannels() + j]
            }
        }
        return vec
        /// Returns the transcoding matrix of coefficients based on the set input and output formats
    }
    
    public func processConversion(inBufs: UnsafeMutablePointer<UnsafeMutablePointer<Float>?>!, outBufs: UnsafeMutablePointer<UnsafeMutablePointer<Float>?>!, numSamples: Int) {
        Mach1TranscodeCAPI_processConversion(M1obj, inBufs, outBufs, CInt(numSamples))
    }
    
    public func getFormatConversionPath() -> [Mach1TranscodeFormatType] {
        var count:CInt = 0
        let pointer:UnsafeMutablePointer<Mach1TranscodeFormatType> = Mach1TranscodeCAPI_getFormatConversionPath(M1obj, &count)

        let vec = Array(UnsafeBufferPointer(start: pointer, count: Int(count)))
        return vec
    }
}
