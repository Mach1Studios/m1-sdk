import Foundation

enum Mach1PlatformType : Int {
    case Mach1PlatformDefault = 0
    case Mach1PlatformUnity = 1
    case Mach1PlatformUE = 2
    case Mach1PlatformOfEasyCam = 3
    case Mach1PlatformAndroid = 4
    case Mach1PlatformiOSPortrait = 5
    case Mach1PlatformiOSLandscape = 6
}

enum Mach1DecodeAlgoType : Int {
    case Mach1DecodeAlgoSpatial = 0
    case Mach1DecodeAlgoAltSpatial = 1
    case Mach1DecodeAlgoHorizon = 2
    case Mach1DecodeAlgoHorizonPairs = 3
    case Mach1DecodeAlgoSpatialPairs = 4
}

class Mach1Decode {
    var M1obj : UnsafeMutableRawPointer
    
    init() {
        M1obj = Mach1DecodeCAPI_create()
    }
    
    deinit {
        Mach1DecodeCAPI_delete(M1obj)
    }
    
    func setPlatformType(type: Mach1PlatformType) {
        Mach1DecodeCAPI_setPlatformType(M1obj, CInt(type.rawValue))
    }
    
    func setDecodeAlgoType(newAlgorithmType: Mach1DecodeAlgoType) {
        Mach1DecodeCAPI_setDecodeAlgoType(M1obj, CInt(newAlgorithmType.rawValue))
    }

    func setFilterSpeed(filterSpeed: Float) {
        Mach1DecodeCAPI_setFilterSpeed(M1obj, filterSpeed)
    }
    
    func beginBuffer() {
        Mach1DecodeCAPI_beginBuffer(M1obj)
    }
    
    func endBuffer() {
        Mach1DecodeCAPI_endBuffer(M1obj)
    }
    
    func getCurrentTime() -> Int {
        let t = Mach1DecodeCAPI_getCurrentTime(M1obj)
        return t
    }
    
    func getLog() -> String {
        let str =  String(cString: UnsafePointer(Mach1DecodeCAPI_getLog(M1obj)))
        return str
    }
    
    func decode(Yaw: Float, Pitch: Float, Roll: Float, bufferSize: Int = 0, sampleIndex: Int = 0) -> [Float] {
        var array: [Float] = [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ]
        Mach1DecodeCAPI_decode(M1obj, Yaw, Pitch, Roll, &array, CInt(bufferSize), CInt(sampleIndex))
        return array
    }
   
}

