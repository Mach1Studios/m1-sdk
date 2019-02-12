import Foundation

class Mach1DecodePositionalCAPI {
    var M1obj : UnsafeMutableRawPointer
    
    init() {
        M1obj = Mach1DecodePositionalCAPI_create()
    }
    
    deinit {
        Mach1DecodePositionalCAPI_delete(M1obj)
    }
    
    func setPlatformType(type: Mach1PlatformType) {
        Mach1DecodePositionalCAPI_setPlatformType(M1obj, type)
    }
    
    func setDecodeAlgoType(newAlgorithmType: Mach1DecodeAlgoType) {
        Mach1DecodePositionalCAPI_setDecodeAlgoType(M1obj, newAlgorithmType)
    }
    
    // settings
    func setUseBlendMode(useBlendMode: Bool) {
        Mach1DecodePositionalCAPI_setUseBlendMode(M1obj, useBlendMode);
    }
    func setIgnoreTopBottom(ignoreTopBottom: Bool) {
        Mach1DecodePositionalCAPI_setIgnoreTopBottom(M1obj, ignoreTopBottom);
    }
    
    func setMuteWhenOutsideObject(muteWhenOutsideObject: Bool) {
        Mach1DecodePositionalCAPI_setMuteWhenOutsideObject(M1obj, muteWhenOutsideObject);
    }
    
    func setMuteWhenInsideObject(muteWhenInsideObject: Bool) {
        Mach1DecodePositionalCAPI_setMuteWhenInsideObject(M1obj, muteWhenInsideObject);
    }
    
    func setUseFalloff(useFalloff: Bool) {
        Mach1DecodePositionalCAPI_setUseFalloff(M1obj, useFalloff);
    }
    func setFalloffCurve(falloffCurve: Float) {
        Mach1DecodePositionalCAPI_setFalloffCurve(M1obj, falloffCurve);
    }
    func setFalloffCurveBlendMode(falloffCurveBlendMode: Float) {
        Mach1DecodePositionalCAPI_setFalloffCurveBlendMode(M1obj, falloffCurveBlendMode);
    }
    
    func setUseClosestPointRotationMuteInside(bool useClosestPointRotationMuteInside: Bool) {
        Mach1DecodePositionalCAPI_setUseClosestPointRotationMuteInside(M1obj, useClosestPointRotationMuteInside);
    }
    
    func setUseYawForRotation(bool useYawForRotation: Bool) {
        Mach1DecodePositionalCAPI_setUseYawForRotation(M1obj, useYawForRotation);
        
    }
    func setUsePitchForRotation(bool usePitchForRotation: Bool) {
        Mach1DecodePositionalCAPI_setUsePitchForRotation(M1obj, usePitchForRotation);
        
    }
    func setUseRollForRotation(bool useRollForRotation: Bool) {
        Mach1DecodePositionalCAPI_setUseRollForRotation(M1obj, useRollForRotation);
        
    }
    
    // parameters for algo
    func setCameraPosition(point: Mach1Point3D) {
        Mach1DecodePositionalCAPI_setCameraPosition(M1obj, point);
    }
    func setCameraRotation(point: Mach1Point3D) {
        Mach1DecodePositionalCAPI_setCameraRotation(M1obj, point);
    }
    func setCameraRotationQuat(point: Mach1Point4D) {
        Mach1DecodePositionalCAPI_setCameraRotationQuat(M1obj, point);
    }
    func setDecoderAlgoPosition(point: Mach1Point3D) {
        Mach1DecodePositionalCAPI_setDecoderAlgoPosition(M1obj, point);
        
    }
    func setDecoderAlgoRotation(point: Mach1Point3D) {
        Mach1DecodePositionalCAPI_setDecoderAlgoRotation(M1obj, point);
    }
    func setDecoderAlgoRotationQuat(point: Mach1Point4D) {
        Mach1DecodePositionalCAPI_setDecoderAlgoRotationQuat(M1obj, point);
    }
    func setDecoderAlgoScale(point: Mach1Point3D) {
        Mach1DecodePositionalCAPI_setDecoderAlgoScale(M1obj, point);
    }
    
    func evaluatePostionResults() {
        Mach1DecodePositionalCAPI_evaluatePostionResults(M1obj);
    }
    
    func getVolumesWalls(result: inout [Float]) {
        let pointer: UnsafeMutablePointer< Float > = UnsafeMutablePointer(&result);
        Mach1DecodePositionalCAPI_getVolumesWalls(M1obj, pointer);
    }
    func getVolumesRoom(result: inout [Float]) {
        let pointer: UnsafeMutablePointer< Float > = UnsafeMutablePointer(&result);
        Mach1DecodePositionalCAPI_getVolumesRoom(M1obj, pointer);
    }
    func getDist() -> Float {
        return Mach1DecodePositionalCAPI_getDist(M1obj);
    }
    func getCurrentAngle() -> struct Mach1Point3D {
    return Mach1DecodePositionalCAPI_getCurrentAngle(M1obj);
    }
    func getVolumeRotation() -> struct Mach1Point3D {
    return Mach1DecodePositionalCAPI_getVolumeRotation(M1obj);
    }
    func setFilterSpeed(filterSpeed: Float) {
        Mach1DecodePositionalCAPI_setFilterSpeed(M1obj, filterSpeed)
    }
}




