import Foundation

enum AngularSettingsType : Int {
    case m1Default = 0
    case m1Unity = 1
    case m1UE = 2
    case m1oFEasyCam = 3
    case m1Android = 4
    case m1iOS = 5
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
    
    func horizonAlgo(Yaw: Float, Pitch: Float, Roll: Float, bufferSize: Int = 0, sampleIndex: Int = 0) -> [Float] {
        let data = Mach1DecodeCAPI_horizonAlgo(M1obj, Yaw, Pitch, Roll, CInt(bufferSize), CInt(sampleIndex))
        var array: [Float] = []
        for i in 0..<10 {
            array.append(Float(data![i]))
        }
        return array
    }
    
    func horizonPairsAlgo(Yaw: Float, Pitch: Float, Roll: Float, bufferSize: Int = 0, sampleIndex: Int = 0) -> [Float] {
        let data = Mach1DecodeCAPI_horizonPairsAlgo(M1obj, Yaw, Pitch, Roll, CInt(bufferSize), CInt(sampleIndex))
        var array: [Float] = []
        for i in 0..<8 {
            array.append(Float(data![i]))
        }
        return array
    }
    
    func spatialAlgo(Yaw: Float, Pitch: Float, Roll: Float, bufferSize: Int = 0, sampleIndex: Int = 0) -> [Float] {
        let data = Mach1DecodeCAPI_spatialAlgo(M1obj, Yaw, Pitch, Roll, CInt(bufferSize), CInt(sampleIndex))
        var array: [Float] = []
        for i in 0..<18 {
            array.append(Float(data![i]))
        }
        return array
    }
    
    func spatialAltAlgo(Yaw: Float, Pitch: Float, Roll: Float, bufferSize: Int = 0, sampleIndex: Int = 0) -> [Float] {
        let data = Mach1DecodeCAPI_spatialAltAlgo(M1obj, Yaw, Pitch, Roll, CInt(bufferSize), CInt(sampleIndex))
        var array: [Float] = []
        for i in 0..<18 {
            array.append(Float(data![i]))
        }
        return array
    }
    
    func spatialPairsAlgo(Yaw: Float, Pitch: Float, Roll: Float, bufferSize: Int = 0, sampleIndex: Int = 0) -> [Float] {
        let data = Mach1DecodeCAPI_spatialPairsAlgo(M1obj, Yaw, Pitch, Roll, CInt(bufferSize), CInt(sampleIndex))
        var array: [Float] = []
        for i in 0..<16 {
            array.append(Float(data![i]))
        }
        return array
    }
    
   
}

