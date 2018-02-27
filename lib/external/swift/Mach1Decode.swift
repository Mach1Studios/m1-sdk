import Foundation

enum AngularSettingsType : Int {
    case m1Default = 0
    case m1Unity = 1
    case m1UE = 2
    case m1oFEasyCam = 3
    case m1Android = 4
    case m1iOS = 5
}

@_silgen_name("Mach1DecodeCAPI_create") func Mach1DecodeCAPI_create() -> UnsafeRawPointer
@_silgen_name("Mach1DecodeCAPI_delete") func Mach1DecodeCAPI_delete(M1obj: UnsafeRawPointer)

@_silgen_name("Mach1DecodeCAPI_horizonAlgo") func Mach1DecodeCAPI_horizonAlgo(M1obj : UnsafeRawPointer, Yaw : CFloat, Pitch: CFloat, Roll: CFloat, bufferSize : CInt, sampleIndex : CInt) -> UnsafePointer<CFloat>
@_silgen_name("Mach1DecodeCAPI_horizonPairsAlgo") func Mach1DecodeCAPI_horizonPairsAlgo(M1obj : UnsafeRawPointer, Yaw : CFloat, Pitch: CFloat, Roll: CFloat, bufferSize : CInt, sampleIndex : CInt) -> UnsafePointer<CFloat>
@_silgen_name("Mach1DecodeCAPI_spatialAlgo") func Mach1DecodeCAPI_spatialAlgo(M1obj : UnsafeRawPointer, Yaw : CFloat, Pitch: CFloat, Roll: CFloat, bufferSize : CInt, sampleIndex : CInt) -> UnsafePointer<CFloat>
@_silgen_name("Mach1DecodeCAPI_spatialAltAlgo") func Mach1DecodeCAPI_spatialAltAlgo(M1obj : UnsafeRawPointer, Yaw : CFloat, Pitch: CFloat, Roll: CFloat, bufferSize : CInt, sampleIndex : CInt) -> UnsafePointer<CFloat>
@_silgen_name("Mach1DecodeCAPI_spatialPairsAlgo") func Mach1DecodeCAPI_spatialPairsAlgo(M1obj : UnsafeRawPointer, Yaw : CFloat, Pitch: CFloat, Roll: CFloat, bufferSize : CInt, sampleIndex : CInt) -> UnsafePointer<CFloat>

@_silgen_name("Mach1DecodeCAPI_setAngularSettingsType") func Mach1DecodeCAPI_setAngularSettingsType(M1obj : UnsafeRawPointer, type : CInt )
@_silgen_name("Mach1DecodeCAPI_beginBuffer") func Mach1DecodeCAPI_beginBuffer(M1obj : UnsafeRawPointer)
@_silgen_name("Mach1DecodeCAPI_endBuffer") func Mach1DecodeCAPI_endBuffer(M1obj : UnsafeRawPointer)

@_silgen_name("Mach1DecodeCAPI_getCurrentTime") func Mach1DecodeCAPI_getCurrentTime(M1obj : UnsafeRawPointer) -> CLong
@_silgen_name("Mach1DecodeCAPI_getLog") func Mach1DecodeCAPI_getLog(M1obj : UnsafeRawPointer) -> UnsafePointer<CChar>

class Mach1Decode {
    var M1obj : UnsafeRawPointer
    
    init() {
        M1obj = Mach1DecodeCAPI_create()
    }
    
    deinit {
        Mach1DecodeCAPI_delete(M1obj: M1obj)
    }
    
    func setAngularSettingsType(type: AngularSettingsType) {
        Mach1DecodeCAPI_setAngularSettingsType(M1obj: M1obj, type: CInt(type.rawValue))
    }
    
    func beginBuffer() {
        Mach1DecodeCAPI_beginBuffer(M1obj: M1obj)
    }
    
    func endBuffer() {
        Mach1DecodeCAPI_endBuffer(M1obj: M1obj)
    }
    
    func getCurrentTime() -> Int {
        let t = Mach1DecodeCAPI_getCurrentTime(M1obj: M1obj)
        return t
    }
    
    func getLog() -> String {
        let str =  String(cString: UnsafePointer(Mach1DecodeCAPI_getLog(M1obj: M1obj)))
        return str
    }
    
    func horizonAlgo(Yaw: Float, Pitch: Float, Roll: Float, bufferSize: Int = 0, sampleIndex: Int = 0) -> [Float] {
        let data = Mach1DecodeCAPI_horizonAlgo(M1obj: M1obj, Yaw: Yaw, Pitch: Pitch, Roll: Roll, bufferSize: CInt(bufferSize), sampleIndex: CInt(sampleIndex))
        var array: [Float] = []
        for i in 0..<10 {
            array.append(Float(data[i]))
        }
        return array
    }
    
    func horizonPairsAlgo(Yaw: Float, Pitch: Float, Roll: Float, bufferSize: Int = 0, sampleIndex: Int = 0) -> [Float] {
        let data = Mach1DecodeCAPI_horizonPairsAlgo(M1obj: M1obj, Yaw: Yaw, Pitch: Pitch, Roll: Roll, bufferSize: CInt(bufferSize), sampleIndex: CInt(sampleIndex))
        var array: [Float] = []
        for i in 0..<8 {
            array.append(Float(data[i]))
        }
        return array
    }
    
    func spatialAlgo(Yaw: Float, Pitch: Float, Roll: Float, bufferSize: Int = 0, sampleIndex: Int = 0) -> [Float] {
        let data = Mach1DecodeCAPI_spatialAlgo(M1obj: M1obj, Yaw: Yaw, Pitch: Pitch, Roll: Roll, bufferSize: CInt(bufferSize), sampleIndex: CInt(sampleIndex))
        var array: [Float] = []
        for i in 0..<18 {
            array.append(Float(data[i]))
        }
        return array
    }
    
    func spatialAltAlgo(Yaw: Float, Pitch: Float, Roll: Float, bufferSize: Int = 0, sampleIndex: Int = 0) -> [Float] {
        let data = Mach1DecodeCAPI_spatialAltAlgo(M1obj: M1obj, Yaw: Yaw, Pitch: Pitch, Roll: Roll, bufferSize: CInt(bufferSize), sampleIndex: CInt(sampleIndex))
        var array: [Float] = []
        for i in 0..<18 {
            array.append(Float(data[i]))
        }
        return array
    }
    
    func spatialPairsAlgo(Yaw: Float, Pitch: Float, Roll: Float, bufferSize: Int = 0, sampleIndex: Int = 0) -> [Float] {
        let data = Mach1DecodeCAPI_spatialPairsAlgo(M1obj: M1obj, Yaw: Yaw, Pitch: Pitch, Roll: Roll, bufferSize: CInt(bufferSize), sampleIndex: CInt(sampleIndex))
        var array: [Float] = []
        for i in 0..<16 {
            array.append(Float(data[i]))
        }
        return array
    }
    
   
}

