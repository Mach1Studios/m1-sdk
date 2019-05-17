//  Mach1 SDK
//  Copyright © 2017 Mach1. All rights reserved.

using System;
using System.Runtime.InteropServices;

namespace Mach1
{
    public class Mach1DecodePositional : IDisposable
    {
#if UNITY_IOS && !UNITY_EDITOR
        internal const string libname = "__Internal";
#elif UNITY_STANDALONE_WIN || UNITY_EDITOR_WIN
        internal const string libname = "Mach1DecodePositionalCAPI";
#else
        internal const string libname = "libMach1DecodePositionalCAPI";
#endif

        [DllImport(libname)]
        internal static extern IntPtr Mach1DecodePositionalCAPI_create();

        [DllImport(libname)]
        internal static extern void Mach1DecodePositionalCAPI_delete(IntPtr M1obj);

        [DllImport(libname)]
        internal static extern void Mach1DecodePositionalCAPI_setPlatformType(IntPtr M1obj, Mach1PlatformType type);

        [DllImport(libname)]
        internal static extern void Mach1DecodePositionalCAPI_setDecodeAlgoType(IntPtr M1obj, Mach1DecodeAlgoType type);

        [DllImport(libname)]
        internal static extern void Mach1DecodePositionalCAPI_setUseBlendMode(IntPtr M1obj, bool useBlendMode);

        [DllImport(libname)]
        internal static extern void Mach1DecodePositionalCAPI_setIgnoreTopBottom(IntPtr M1obj, bool ignoreTopBottom);

        [DllImport(libname)]
        internal static extern void Mach1DecodePositionalCAPI_setMuteWhenOutsideObject(IntPtr M1obj, bool muteWhenOutsideObject);

        [DllImport(libname)]
        internal static extern void Mach1DecodePositionalCAPI_setMuteWhenInsideObject(IntPtr M1obj, bool muteWhenInsideObject);

        [DllImport(libname)]
        internal static extern void Mach1DecodePositionalCAPI_setUseAttenuation(IntPtr M1obj, bool useAttenuation);

        [DllImport(libname)]
        internal static extern void Mach1DecodePositionalCAPI_setAttenuationCurve(IntPtr M1obj, float attenuationCurve);

        [DllImport(libname)]
        internal static extern void Mach1DecodePositionalCAPI_setAttenuationCurveBlendMode(IntPtr M1obj, float attenuationCurveBlendMode);

        [DllImport(libname)]
        internal static extern void Mach1DecodePositionalCAPI_setUsePlaneCalculation(IntPtr M1obj, bool usePlaneCalculation);

        [DllImport(libname)]
        internal static extern void Mach1DecodePositionalCAPI_setUseYawForRotation(IntPtr M1obj, bool useYawForRotation);

        [DllImport(libname)]
        internal static extern void Mach1DecodePositionalCAPI_setUsePitchForRotation(IntPtr M1obj, bool usePitchForRotation);

        [DllImport(libname)]
        internal static extern void Mach1DecodePositionalCAPI_setUseRollForRotation(IntPtr M1obj, bool useRollForRotation);

        [DllImport(libname)]
        internal static extern void Mach1DecodePositionalCAPI_setPlatformType(IntPtr M1obj, bool type);

        [DllImport(libname)]
        internal static extern void Mach1DecodePositionalCAPI_setListenerPosition(IntPtr M1obj, Mach1Point3D point);

        [DllImport(libname)]
        internal static extern void Mach1DecodePositionalCAPI_setListenerRotation(IntPtr M1obj, Mach1Point3D point);

        [DllImport(libname)]
        internal static extern void Mach1DecodePositionalCAPI_setListenerRotationQuat(IntPtr M1obj, Mach1Point4D quat);

        [DllImport(libname)]
        internal static extern void Mach1DecodePositionalCAPI_setDecoderAlgoPosition(IntPtr M1obj, Mach1Point3D point);

        [DllImport(libname)]
        internal static extern void Mach1DecodePositionalCAPI_setDecoderAlgoRotation(IntPtr M1obj, Mach1Point3D point);

        [DllImport(libname)]
        internal static extern void Mach1DecodePositionalCAPI_setDecoderAlgoRotationQuat(IntPtr M1obj, Mach1Point4D quat);

        [DllImport(libname)]
        internal static extern void Mach1DecodePositionalCAPI_setDecoderAlgoScale(IntPtr M1obj, Mach1Point3D point);

        [DllImport(libname)]
        internal static extern void Mach1DecodePositionalCAPI_evaluatePositionResults(IntPtr M1obj);

        [DllImport(libname)]
        internal static extern void Mach1DecodePositionalCAPI_getCoefficients(IntPtr M1obj, IntPtr data);

        [DllImport(libname)]
        internal static extern void Mach1DecodePositionalCAPI_getCoefficientsInterior(IntPtr M1obj, IntPtr data);

        [DllImport(libname)]
        internal static extern float Mach1DecodePositionalCAPI_getDist(IntPtr M1obj);

        [DllImport(libname)]
        internal static extern Mach1Point3D Mach1DecodePositionalCAPI_getCoefficientsRotation(IntPtr M1obj);

/* DEPRECATED START */
        [DllImport(libname)]
        [Obsolete("setUseFalloff is deprecated, please use setUseAttenuation instead.")]
        internal static extern void Mach1DecodePositionalCAPI_setUseFalloff(IntPtr M1obj, bool useFalloff);
        [DllImport(libname)]
        [Obsolete("setFalloffCurve is deprecated, please use setAttenuationCurve instead.")]
        internal static extern void Mach1DecodePositionalCAPI_setFalloffCurve(IntPtr M1obj, float falloffCurve);
        [DllImport(libname)]
        [Obsolete("setFalloffCurveBlendMode is deprecated, please use setAttenuationCurveBlendMode instead.")]
        internal static extern void Mach1DecodePositionalCAPI_setFalloffCurveBlendMode(IntPtr M1obj, float falloffCurveBlendMode);
        [DllImport(libname)]
        [Obsolete("setUseClosestPointRotationMuteInside is deprecated, please use setUsePlaneCalculation instead.")]
        internal static extern void Mach1DecodePositionalCAPI_setUseClosestPointRotationMuteInside(IntPtr M1obj, bool useClosestPointRotationMuteInside);
        [DllImport(libname)]
        [Obsolete("setCameraPosition is deprecated, please use setListenerPosition instead.")]
        internal static extern void Mach1DecodePositionalCAPI_setCameraPosition(IntPtr M1obj, Mach1Point3D point);
        [DllImport(libname)]
        [Obsolete("setCameraRotation is deprecated, please use setListenerRotation instead.")]
        internal static extern void Mach1DecodePositionalCAPI_setCameraRotation(IntPtr M1obj, Mach1Point3D point);
        [DllImport(libname)]
        [Obsolete("setCameraRotationQuat is deprecated, please use setListenerRotationQuat instead.")]
        internal static extern void Mach1DecodePositionalCAPI_setCameraRotationQuat(IntPtr M1obj, Mach1Point4D quat);
        [DllImport(libname)]
        [Obsolete("getVolumesWalls is deprecated, please use getCoefficients instead.")]
        internal static extern void Mach1DecodePositionalCAPI_getVolumesWalls(IntPtr M1obj, IntPtr data);
        [DllImport(libname)]
        [Obsolete("getVolumesRoom is deprecated, please use getCoefficientsInterior instead.")]
        internal static extern void Mach1DecodePositionalCAPI_getVolumesRoom(IntPtr M1obj, IntPtr data);
        [DllImport(libname)]
        [Obsolete("getVolumeRotation is deprecated, please use getCoefficientsRotation instead.")]
        internal static extern Mach1Point3D Mach1DecodePositionalCAPI_getVolumeRotation(IntPtr M1obj);
/* DEPRECATED END */

        internal IntPtr M1obj;

        public Mach1DecodePositional()
        {
            M1obj = Mach1DecodePositionalCAPI_create();
        }

        public void Dispose()
        {
            Mach1DecodePositionalCAPI_delete(M1obj);
        }

        public void setPlatformType(Mach1PlatformType type)
        {
            Mach1DecodePositionalCAPI_setPlatformType(M1obj, type);
            // Set the device's angle order and convention if applicable
            //
            // - Parameters:
            //     - Mach1PlatformDefault = 0
            //     - Mach1PlatformUnity = 1
            //     - Mach1PlatformUE = 2
            //     - Mach1PlatformOfEasyCam = 3
            //     - Mach1PlatformAndroid = 4
            //     - Mach1PlatformiOS = 5
        }

        public void setDecodeAlgoType(Mach1DecodeAlgoType type)
		{
			Mach1DecodePositionalCAPI_setDecodeAlgoType(M1obj, type);
            // Set the decoding algorithm
            //
            // - Parameters:
            //     - Mach1DecodeAlgoSpatial = 0 (default spatial | 8 channels)
            //     - Mach1DecodeAlgoAltSpatial = 1 (periphonic spatial | 8 channels)
            //     - Mach1DecodeAlgoHorizon = 2 (compass / yaw | 4 channels)
            //     - Mach1DecodeAlgoHorizonPairs = 3 (compass / yaw | 4x stereo mastered pairs)
            //     - Mach1DecodeAlgoSpatialPairs = 4 (experimental periphonic pairs | 8x stereo mastered pairs)
		}

        // settings

        public void setUseBlendMode(bool useBlendMode)
        {
            Mach1DecodePositionalCAPI_setUseBlendMode(M1obj, useBlendMode);
            // Warning: Experimental feature
            // Remark: Part of BlendMode
        }

        public void setIgnoreTopBottom(bool ignoreTopBottom)
        {
            Mach1DecodePositionalCAPI_setIgnoreTopBottom(M1obj, ignoreTopBottom);
            // Warning: Experimental feature
            // Remark: Part of BlendMode

        }

        public void setMuteWhenOutsideObject(bool muteWhenOutsideObject)
        {
            Mach1DecodePositionalCAPI_setMuteWhenOutsideObject(M1obj, muteWhenOutsideObject);
            // Mute mach1decode object (all coefficifient results becomes 0)
            // when outside the positional reference shape/point
        }

        public void setMuteWhenInsideObject(bool muteWhenInsideObject)
        {
            Mach1DecodePositionalCAPI_setMuteWhenInsideObject(M1obj, muteWhenInsideObject);
            // Mute mach1decode object (all coefficifient results becomes 0)
            // when inside the positional reference shape/point
        }

        public void setUseAttenuation(bool useAttenuation)
        {
            Mach1DecodePositionalCAPI_setUseAttenuation(M1obj, useAttenuation);
            // Set distance attenuation calculations on that mach1decode object on/off
            //
            // Remark: When off, distance remains 1.0
        }

        public void setAttenuationCurve(float attenuationCurve)
        {
            Mach1DecodePositionalCAPI_setAttenuationCurve(M1obj, attenuationCurve);
        }

        public void setAttenuationCurveBlendMode(float attenuationCurveBlendMode)
        {
            Mach1DecodePositionalCAPI_setAttenuationCurveBlendMode(M1obj, attenuationCurveBlendMode);
            // Warning: Experimental feature
            // Remark: Part of BlendMode
        }

        public void setUsePlaneCalculation(bool usePlaneCalculation)
        {
            Mach1DecodePositionalCAPI_setUsePlaneCalculation(M1obj, usePlaneCalculation);
        }

        public void setUseYawForRotation(bool useYawForRotation)
        {
            Mach1DecodePositionalCAPI_setUseYawForRotation(M1obj, useYawForRotation);
            // Set use/ignore Yaw angle rotation results from pivoting positionally
            //
            // Remark: Default is true
        }

        public void setUsePitchForRotation(bool usePitchForRotation)
        {
            Mach1DecodePositionalCAPI_setUsePitchForRotation(M1obj, usePitchForRotation);
            // Set use/ignore Pitch angle rotation results from pivoting positionally
            //
            // Remark: Default is true
        }

        public void setUseRollForRotation(bool useRollForRotation)
        {
            Mach1DecodePositionalCAPI_setUseRollForRotation(M1obj, useRollForRotation);
            // Set use/ignore Roll angle rotation results from pivoting positionally
            //
            // Remark: Default is true
        }

        // parameters for algo
        public void setListenerPosition(Mach1Point3D point)
        {
            Mach1DecodePositionalCAPI_setListenerPosition(M1obj, point);
            // Sets the device/camera's position in desired x,y,z space
            //
            // - Parameters:
            //     - Mach1Point3D Position: x,y,z
        }

        public void setListenerRotation(Mach1Point3D point)
        {
            Mach1DecodePositionalCAPI_setListenerRotation(M1obj, point);
            // Sets the device/camera's orientation with Euler angles
            //
            // - Parameters:
            //     - Mach1Point3D Rotation: yaw,pitch,roll
        }

        public void setListenerRotationQuat(Mach1Point4D quat)
        {
            Mach1DecodePositionalCAPI_setListenerRotationQuat(M1obj, quat);
            // Sets the device/camera's orientation with Quaternion
            //
            // - Parameters:
            //     - Mach1Point4D Rotation: Quaternion
        }

        public void setDecoderAlgoPosition(Mach1Point3D point)
        {
            Mach1DecodePositionalCAPI_setDecoderAlgoPosition(M1obj, point);
            // Sets the decode object's position in desired x,y,z space
            //
            // - Parameters:
            //     - Mach1Point3D Rotation: x,y,z
        }

        public void setDecoderAlgoRotation(Mach1Point3D point)
        {
            Mach1DecodePositionalCAPI_setDecoderAlgoRotation(M1obj, point);
            // Sets the decode object's orientation with Euler angles
            //
            // - Parameters:
            //     - Mach1Point3D Rotation: yaw,pitch,roll
        }

        public void setDecoderAlgoRotationQuat(Mach1Point4D quat)
        {
            Mach1DecodePositionalCAPI_setDecoderAlgoRotationQuat(M1obj, quat);
            // Sets the decode object's orientation with Quaternion
            //
            // - Parameters:
            //     - Mach1Point4D Rotation: Quaternion
        }

        public void setDecoderAlgoScale(Mach1Point3D point)
        {
            Mach1DecodePositionalCAPI_setDecoderAlgoScale(M1obj, point);
            // Sets the decode object's scale in desired x,y,z space
            //
            // - Parameters:
            //     - Mach1Point3D Scale: x,y,z multiplier
        }

        public void evaluatePositionResults()
        {
            Mach1DecodePositionalCAPI_evaluatePositionResults(M1obj);
        }

        public void getCoefficients(ref float[] data)
        {
            //if(data.Length < 18) data = new float[18];
            GCHandle pinnedArray = GCHandle.Alloc(data, GCHandleType.Pinned);
            IntPtr pointer = pinnedArray.AddrOfPinnedObject();

            Mach1DecodePositionalCAPI_getCoefficients(M1obj, pointer);

            pinnedArray.Free();

            // Return the current coefficients to be applied to the audiopla yer's volume
        }

        public void getCoefficientsInterior(ref float[] data)
        {
            //if(data.Length < 18) data = new float[18];
            GCHandle pinnedArray = GCHandle.Alloc(data, GCHandleType.Pinned);
            IntPtr pointer = pinnedArray.AddrOfPinnedObject();

            Mach1DecodePositionalCAPI_getCoefficientsInterior(M1obj, pointer);

            pinnedArray.Free();

            // Warning: Experimental feature
            // Remark: Part of BlendMode
        }

        public float getDist()
        {
            return Mach1DecodePositionalCAPI_getDist(M1obj);
            // Return a distance calculation of the position of the device/camera
            // to the position of the m1obj
        }

        public Mach1Point3D getCoefficientsRotation()
        {
            return Mach1DecodePositionalCAPI_getCoefficientsRotation(M1obj);
        }

/* DEPRECATED START */
        public void setUseFalloff(bool useFalloff)
        {
            Mach1DecodePositionalCAPI_setUseFalloff(M1obj, useFalloff);
            // Set distance attenuation calculations on that mach1decode object on/off
            //
            // Remark: When off, distance remains 1.0
        }

        public void setFalloffCurve(float falloffCurve)
        {
            Mach1DecodePositionalCAPI_setFalloffCurve(M1obj, falloffCurve);
        }

        public void setFalloffCurveBlendMode(float falloffCurveBlendMode)
        {
            Mach1DecodePositionalCAPI_setFalloffCurveBlendMode(M1obj, falloffCurveBlendMode);
            // Warning: Experimental feature
            // Remark: Part of BlendMode
        }

        public void setUseClosestPointRotationMuteInside(bool useClosestPointRotationMuteInside)
        {
            Mach1DecodePositionalCAPI_setUseClosestPointRotationMuteInside(M1obj, useClosestPointRotationMuteInside);
        }

        public void setCameraPosition(Mach1Point3D point)
        {
            Mach1DecodePositionalCAPI_setCameraPosition(M1obj, point);
            // Sets the device/camera's position in desired x,y,z space
            //
            // - Parameters:
            //     - Mach1Point3D Position: x,y,z
        }

        public void setCameraRotation(Mach1Point3D point)
        {
            Mach1DecodePositionalCAPI_setCameraRotation(M1obj, point);
            // Sets the device/camera's orientation with Euler angles
            //
            // - Parameters:
            //     - Mach1Point3D Rotation: yaw,pitch,roll
        }

        public void setCameraRotationQuat(Mach1Point4D quat)
        {
            Mach1DecodePositionalCAPI_setCameraRotationQuat(M1obj, quat);
            // Sets the device/camera's orientation with Quaternion
            //
            // - Parameters:
            //     - Mach1Point4D Rotation: Quaternion
        }

        public void getVolumesWalls(ref float[] data)
        {
            //if(data.Length < 18) data = new float[18];
            GCHandle pinnedArray = GCHandle.Alloc(data, GCHandleType.Pinned);
            IntPtr pointer = pinnedArray.AddrOfPinnedObject();

            Mach1DecodePositionalCAPI_getVolumesWalls(M1obj, pointer);

            pinnedArray.Free();

            // Return the current coefficients to be applied to the audiopla yer's volume
        }

        public void getVolumesRoom(ref float[] data)
        {
            //if(data.Length < 18) data = new float[18];
            GCHandle pinnedArray = GCHandle.Alloc(data, GCHandleType.Pinned);
            IntPtr pointer = pinnedArray.AddrOfPinnedObject();

            Mach1DecodePositionalCAPI_getVolumesRoom(M1obj, pointer);

            pinnedArray.Free();

            // Warning: Experimental feature
            // Remark: Part of BlendMode
        }

        public Mach1Point3D getVolumeRotation()
        {
            return Mach1DecodePositionalCAPI_getVolumeRotation(M1obj);
        }
/* DEPRECATED END */

    }
}
