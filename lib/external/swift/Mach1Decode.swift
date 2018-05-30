import Foundation

enum AngularSettingsType : Int {
    case m1Default = 0
    case m1Unity = 1
    case m1UE = 2
    case m1oFEasyCam = 3
    case m1Android = 4
    case m1iOSPortrait = 5
    case m1iOSLandscape = 6
}

enum Mach1AlgorithmType : Int {
    case m1Spatial = 0
    case m1AltSpatial = 1
    case m1Horizon = 2
    case m1HorizonPairs = 3
    case m1SpatialPairs = 4
}

class Mach1Decode {
    var M1obj : UnsafeMutableRawPointer
    
    init() {
        M1obj = Mach1DecodeCAPI_create()
    }
    
    deinit {
        Mach1DecodeCAPI_delete(M1obj)
    }
    
    func setAngularSettingsType(type: AngularSettingsType) {
        Mach1DecodeCAPI_setAngularSettingsType(M1obj, CInt(type.rawValue))
    }
    
    func setAlgorithmType(newAlgorithmType: Mach1AlgorithmType) {
        Mach1DecodeCAPI_setAlgorithmType(M1obj, CInt(newAlgorithmType.rawValue))
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
        var array: [Float] = []
        let data = Mach1DecodeCAPI_decode(M1obj, Yaw, Pitch, Roll, &array, CInt(bufferSize), CInt(sampleIndex))
        for i in 0..<18 {
            array.append(Float(data[i]))
        }
        return array
    }
   
}

