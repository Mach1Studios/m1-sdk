import Foundation

public class Mach1DecodePositional {
    var M1obj : UnsafeMutableRawPointer
    
    public init() {
        M1obj = Mach1DecodePositionalCAPI_create()
    }
    deinit {
        Mach1DecodePositionalCAPI_delete(M1obj)
    }
    
    public func setPlatformType(type: Mach1PlatformType) {
        Mach1DecodePositionalCAPI_setPlatformType(M1obj, type)
    }
    public func setDecodeAlgoType(newAlgorithmType: Mach1DecodeAlgoType) {
        Mach1DecodePositionalCAPI_setDecodeAlgoType(M1obj, newAlgorithmType)
    }
    
    // settings
    public func setUseBlendMode(useBlendMode: Bool) {
        Mach1DecodePositionalCAPI_setUseBlendMode(M1obj, useBlendMode)
    }
    public func setIgnoreTopBottom(ignoreTopBottom: Bool) {
        Mach1DecodePositionalCAPI_setIgnoreTopBottom(M1obj, ignoreTopBottom)
    }
    
    public func setMuteWhenOutsideObject(muteWhenOutsideObject: Bool) {
        Mach1DecodePositionalCAPI_setMuteWhenOutsideObject(M1obj, muteWhenOutsideObject)
    }
    
    public func setMuteWhenInsideObject(muteWhenInsideObject: Bool) {
        Mach1DecodePositionalCAPI_setMuteWhenInsideObject(M1obj, muteWhenInsideObject)
    }
    
    public func setUseFalloff(useFalloff: Bool) {
        Mach1DecodePositionalCAPI_setUseFalloff(M1obj, useFalloff)
    }
    public func setFalloffCurve(falloffCurve: Float) {
        Mach1DecodePositionalCAPI_setFalloffCurve(M1obj, falloffCurve)
    }
    public func setFalloffCurveBlendMode(falloffCurveBlendMode: Float) {
        Mach1DecodePositionalCAPI_setFalloffCurveBlendMode(M1obj, falloffCurveBlendMode)
    }
    
    public func setUseClosestPointRotationMuteInside(bool useClosestPointRotationMuteInside: Bool) {
        Mach1DecodePositionalCAPI_setUseClosestPointRotationMuteInside(M1obj, useClosestPointRotationMuteInside)
    }
    
    public func setUseYawForRotation(bool useYawForRotation: Bool) {
        Mach1DecodePositionalCAPI_setUseYawForRotation(M1obj, useYawForRotation)
        
    }
    public func setUsePitchForRotation(bool usePitchForRotation: Bool) {
        Mach1DecodePositionalCAPI_setUsePitchForRotation(M1obj, usePitchForRotation)
        
    }
    public func setUseRollForRotation(bool useRollForRotation: Bool) {
        Mach1DecodePositionalCAPI_setUseRollForRotation(M1obj, useRollForRotation)
        
    }
    
    public func setCameraPosition(point: Mach1Point3D) {
        Mach1DecodePositionalCAPI_setCameraPosition(M1obj, point)
        /// Sets the device/camera's position in desired x,y,z space
        ///
        /// - Parameters:
        ///     - Mach1Point3D Position: x,y,z
    }
    public func setCameraRotation(point: Mach1Point3D) {
        Mach1DecodePositionalCAPI_setCameraRotation(M1obj, point)
        /// Sets the device/camera's orientation with Euler angles
        ///
        /// - Parameters:
        ///     - Mach1Point3D Rotation: yaw,pitch,roll
    }
    public func setCameraRotationQuat(point: Mach1Point4D) {
        Mach1DecodePositionalCAPI_setCameraRotationQuat(M1obj, point)
        /// Sets the device/camera's orientation with Quaternion
        ///
        /// - Parameters:
        ///     - Mach1Point4D Rotation: Quaternion
    }
    public func setDecoderAlgoPosition(point: Mach1Point3D) {
        Mach1DecodePositionalCAPI_setDecoderAlgoPosition(M1obj, point)
        /// Sets the decode object's position in desired x,y,z space
        ///
        /// - Parameters:
        ///     - Mach1Point3D Rotation: x,y,z
    }
    public func setDecoderAlgoRotation(point: Mach1Point3D) {
        Mach1DecodePositionalCAPI_setDecoderAlgoRotation(M1obj, point)
        /// Sets the decode object's orientation with Euler angles
        ///
        /// - Parameters:
        ///     - Mach1Point3D Rotation: yaw,pitch,roll
    }
    public func setDecoderAlgoRotationQuat(point: Mach1Point4D) {
        Mach1DecodePositionalCAPI_setDecoderAlgoRotationQuat(M1obj, point)
        /// Sets the decode object's orientation with Quaternion
        ///
        /// - Parameters:
        ///     - Mach1Point4D Rotation: Quaternion
    }
    public func setDecoderAlgoScale(point: Mach1Point3D) {
        Mach1DecodePositionalCAPI_setDecoderAlgoScale(M1obj, point)
        /// Sets the decode object's scale in desired x,y,z space
        ///
        /// - Parameters:
        ///     - Mach1Point3D Scale: x,y,z multiplier
    }
    
    public func evaluatePositionResults() {
        Mach1DecodePositionalCAPI_evaluatePositionResults(M1obj)
    }
    
    public func getVolumesWalls(result: inout [Float]) {
        let pointer: UnsafeMutablePointer< Float > = UnsafeMutablePointer(&result)
        Mach1DecodePositionalCAPI_getVolumesWalls(M1obj, pointer);
    }
    public func getVolumesRoom(result: inout [Float]) {
        let pointer: UnsafeMutablePointer< Float > = UnsafeMutablePointer(&result)
        Mach1DecodePositionalCAPI_getVolumesRoom(M1obj, pointer);
    }
    public func getDist() -> Float {
        return Mach1DecodePositionalCAPI_getDist(M1obj)
    }
    
    public func getCurrentAngle() -> Mach1Point3D {
        return Mach1DecodePositionalCAPI_getCurrentAngle(M1obj)
    }
    
    public func getVolumeRotation() -> Mach1Point3D {
        return Mach1DecodePositionalCAPI_getVolumeRotation(M1obj)
    }
    
    public func setFilterSpeed(filterSpeed: Float) {
        Mach1DecodePositionalCAPI_setFilterSpeed(M1obj, filterSpeed)
    }
    
}