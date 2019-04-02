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
    }
    
    public func setDecodeAlgoType(newAlgorithmType: Mach1DecodeAlgoType) {
        Mach1DecodeCAPI_setDecodeAlgoType(M1obj, newAlgorithmType)
    }

    public func setFilterSpeed(filterSpeed: Float) {
        Mach1DecodeCAPI_setFilterSpeed(M1obj, filterSpeed)
    }
    
    public func beginBuffer() {
        Mach1DecodeCAPI_beginBuffer(M1obj)
    }
    
    public func endBuffer() {
        Mach1DecodeCAPI_endBuffer(M1obj)
    }
    
    public func getCurrentTime() -> Int {
        let t = Mach1DecodeCAPI_getCurrentTime(M1obj)
        return t
    }
    
    public func getLog() -> String {
        let str = String(cString: UnsafePointer(Mach1DecodeCAPI_getLog(M1obj)))
        return str
    }
    
    public func decode(Yaw: Float, Pitch: Float, Roll: Float, bufferSize: Int = 0, sampleIndex: Int = 0) -> [Float] {
        var array: [Float] = Array(repeating: 0.0, count: 18)
        Mach1DecodeCAPI_decode(M1obj, Yaw, Pitch, Roll, &array, CInt(bufferSize), CInt(sampleIndex))
        return array
    }
   
}

