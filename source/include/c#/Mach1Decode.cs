//  Mach1 SDK
//  Copyright © 2017 Mach1. All rights reserved.

using System;
using System.Runtime.InteropServices;

namespace Mach1
{
    public class Mach1Decode : IDisposable
    {
#if UNITY_IOS && !UNITY_EDITOR
        internal const string libname = "__Internal";
#elif UNITY_STANDALONE_WIN || UNITY_EDITOR_WIN
        internal const string libname = "Mach1DecodeCAPI";
#else
        internal const string libname = "libMach1DecodeCAPI";
#endif

        [DllImport(libname)]
        internal static extern IntPtr Mach1DecodeCAPI_create();

        [DllImport(libname)]
        internal static extern void Mach1DecodeCAPI_delete(IntPtr M1obj);

        [DllImport(libname)]
        internal static extern void Mach1DecodeCAPI_setDecodeAlgoType(IntPtr M1obj, Mach1DecodeAlgoType type);

        [DllImport(libname)]
        internal static extern void Mach1DecodeCAPI_setPlatformType(IntPtr M1obj, Mach1PlatformType type);

        [DllImport(libname)]
        internal static extern Mach1DecodeAlgoType Mach1DecodeCAPI_getDecodeAlgoType(IntPtr M1obj);

        [DllImport(libname)]
        internal static extern Mach1PlatformType Mach1DecodeCAPI_getPlatformType(IntPtr M1obj);

        [DllImport(libname)]
        internal static extern IntPtr Mach1DecodeCAPI_decode(IntPtr M1obj, float Yaw, float Pitch, float Roll, IntPtr data, int bufferSize = 0, int sampleIndex = 0);

        [DllImport(libname)]
        internal static extern IntPtr Mach1DecodeCAPI_decodeCoeffs(IntPtr M1obj, IntPtr data, int bufferSize = 0, int sampleIndex = 0);

        [DllImport(libname)]
        internal static extern IntPtr Mach1DecodeCAPI_decodePannedCoeffs(IntPtr M1obj, IntPtr data, int bufferSize = 0, int sampleIndex = 0, bool applyPanLaw = true);

        [DllImport(libname)]
        internal static extern IntPtr Mach1DecodeCAPI_decodeCoeffsUsingTranscodeMatrix(IntPtr M1obj, IntPtr matrix, int channels, IntPtr data, int bufferSize = 0, int sampleIndex = 0);

        [DllImport(libname)]
        internal static extern void Mach1DecodeCAPI_setFilterSpeed(IntPtr M1obj, float filterSpeed);

        [DllImport(libname)]
        internal static extern int Mach1DecodeCAPI_getFormatChannelCount(IntPtr M1obj);

        [DllImport(libname)]
        internal static extern int Mach1DecodeCAPI_getFormatCoeffCount(IntPtr M1obj);

        [DllImport(libname)]
        internal static extern void Mach1DecodeCAPI_setRotation(IntPtr M1obj, Mach1Point3D newRotationFromMinusOnetoOne);

        [DllImport(libname)]
        internal static extern void Mach1DecodeCAPI_setRotationDegrees(IntPtr M1obj, Mach1Point3D newRotationDegrees);

        [DllImport(libname)]
        internal static extern void Mach1DecodeCAPI_setRotationRadians(IntPtr M1obj, Mach1Point3D newRotationRadians);

        [DllImport(libname)]
        internal static extern void Mach1DecodeCAPI_setRotationQuat(IntPtr M1obj, Mach1Point3D newRotationQuat);

        [DllImport(libname)]
        internal static extern long Mach1DecodeCAPI_getCurrentTime(IntPtr M1obj);

        [DllImport(libname)]
        internal static extern long Mach1DecodeCAPI_getLastCalculationTime(IntPtr M1obj);

        [DllImport(libname)]
        internal static extern IntPtr Mach1DecodeCAPI_getLog(IntPtr M1obj);

        [DllImport(libname)]
        internal static extern Mach1Point3D Mach1DecodeCAPI_getCurrentAngle(IntPtr M1obj);

        internal IntPtr M1obj;

        public Mach1Decode()
        {
            M1obj = Mach1DecodeCAPI_create();
        }

        public void Dispose()
        {
            Mach1DecodeCAPI_delete(M1obj);
        }

        public void setDecodeAlgoType(Mach1DecodeAlgoType type)
        {
            Mach1DecodeCAPI_setDecodeAlgoType(M1obj, type);
            /// Set the decoding algorithm
            ///
            /// - Parameters:
            ///     - Mach1DecodeAlgoHorizon_4 (compass / yaw | 4 channels)
            ///     - Mach1DecodeAlgoSpatial_8 (spatial | 8 channels)
            ///     - Mach1DecodeAlgoSpatial_12 (higher order spatial | 12 channels)
            ///     - Mach1DecodeAlgoSpatial_14 (higher order spatial | 14 channels)
        }

        public void setPlatformType(Mach1PlatformType type)
        {
            Mach1DecodeCAPI_setPlatformType(M1obj, type);
            /// Set the device's angle order and convention if applicable
            ///
            /// - Parameters:
            ///     - Mach1PlatformDefault = 0
            ///     - Mach1PlatformUnity
            ///     - Mach1PlatformUE
            ///     - Mach1PlatformOfEasyCam
            ///     - Mach1PlatformAndroid
            ///     - Mach1PlatformiOS
            ///     - Mach1PlatformiOSTableTop_ZVertical
            ///     - Mach1PlatformiOSPortraitHandheld_YVertical
            ///     - Mach1PlatformiOSPortrait_YawOnly
        }

        public Mach1DecodeAlgoType getDecodeAlgoType()
        {
            return Mach1DecodeCAPI_getDecodeAlgoType(M1obj);
            /// Returns the current decoding algorithm (or also known as the Mach1Decode's input channel mode)
        }

        public Mach1PlatformType getPlatformType()
        {
            return Mach1DecodeCAPI_getPlatformType(M1obj);
            /// Returns the current platform type being used to re-order input orientation
        }

        public void decode(float Yaw, float Pitch, float Roll, ref float[] data,  int bufferSize = 0, int sampleIndex = 0)
        {
            GCHandle pinnedArray = GCHandle.Alloc(data, GCHandleType.Pinned);
            IntPtr pointer = pinnedArray.AddrOfPinnedObject();

            Mach1DecodeCAPI_decode(M1obj, Yaw, Pitch, Roll, pointer, bufferSize, sampleIndex);

            pinnedArray.Free();
            /// Call with current update's angles to return the resulting coefficients
            /// to apply to the audioplayer's volume
            ///
            /// Includes two modes of use:
            /// + Update decode results via audio callback
            ///   + *Use your audio player's buffersize and current sample index for sync callbacks*
            /// + Update decode results via main loop (or any loop)
            ///   + *Default null or 0 values to **bufferSize** or **sampleIndex** will use the second mode*
            ///
            /// - Parameters: 
            ///     - Yaw: float for device/listener yaw angle: [Range: 0->360 | -180->180]
            ///     - Pitch: float for device/listener pitch angle: [Range: -90->90]
            ///     - Roll: float for device/listener roll angle: [Range: -90->90]
            ///     - bufferSize: int for number of samples in a buffer, ideally supplied from your audioplayer/engine
            ///     - sampleIndex: int for current sample index array, ideally supplied from your audioplayer/engine
        }

        public void decodeCoeffs(ref float[] data,  int bufferSize = 0, int sampleIndex = 0)
        {
            GCHandle pinnedArray = GCHandle.Alloc(data, GCHandleType.Pinned);
            IntPtr pointer = pinnedArray.AddrOfPinnedObject();

            Mach1DecodeCAPI_decodeCoeffs(M1obj, pointer, bufferSize, sampleIndex);

            pinnedArray.Free();
            /// Call with current update's angles to return the resulting coefficients
            /// to apply to the audioplayer's volume
            ///
            /// Includes two modes of use:
            /// + Update decode results via audio callback
            ///   + *Use your audio player's buffersize and current sample index for sync callbacks*
            /// + Update decode results via main loop (or any loop)
            ///   + *Default null or 0 values to **bufferSize** or **sampleIndex** will use the second mode*
            ///
            /// - Parameters: 
            ///     - bufferSize: int for number of samples in a buffer, ideally supplied from your audioplayer/engine
            ///     - sampleIndex: int for current sample index array, ideally supplied from your audioplayer/engine
        }

        public void setFilterSpeed(float filterSpeed)
        {
            Mach1DecodeCAPI_setFilterSpeed(M1obj, (float)filterSpeed);
            /// Filter speed determines the amount of angle smoothing applied
            /// to the orientation angles used for the Mach1DecodeCore class
            ///
            /// - Parameters:
            ///     - value range: 0.0001 -> 1.0 (where 0.1 would be a slow filter
            ///     and 1.0 is no filter)
        }

        public void setRotation(Mach1Point3D newRotationFromMinusOnetoOne)
        {
            Mach1DecodeCAPI_setRotation(M1obj, (Mach1Point3D)newRotationFromMinusOnetoOne);
            /// Set current buffer/sample intended decoding orientation YPR.
            ///
            /// - Parameters:
            ///     - Yaw: float for device/listener yaw angle:     [Range: 0.0 -> 1.0 | -0.5 -> 0.5]
            ///                                                     [Range: 0.0 -> 360 | -180 -> 180]
            ///     - Pitch: float for device/listener pitch angle: [Range: -0.25 -> 0.25]
            ///                                                     [Range: -90   -> 90]
            ///     - Roll: float for device/listener roll angle:   [Range: -0.25 -> 0.25]
            ///                                                     [Range: -90   -> 90]
        }

        public void setRotationDegrees(Mach1Point3D newRotationDegrees)
        {
            Mach1DecodeCAPI_setRotationDegrees(M1obj, (Mach1Point3D)newRotationDegrees);
            /// Set current buffer/sample intended decoding orientation YPR.
            ///
            /// - Parameters:
            ///     - Yaw: float for device/listener yaw angle: [Range: 0.0 -> 360 | -180 -> 180]
            ///     - Pitch: float for device/listener pitch angle: [Range: -90 -> 90]
            ///     - Roll: float for device/listener roll angle: [Range: -90 -> 90]
        }

        public void setRotationRadians(Mach1Point3D newRotationRadians)
        {
            Mach1DecodeCAPI_setRotationRadians(M1obj, (Mach1Point3D)newRotationRadians);
            /// Set current buffer/sample intended decoding orientation YPR in radians.
            ///
            /// - Parameters:
            ///     - Yaw: float for device/listener yaw angle:     [Range: 0.0 -> 2PI | -PI  -> PI]
            ///                                                     [Range: 0.0 -> 360 | -180 -> 180]
            ///     - Pitch: float for device/listener pitch angle: [Range: -PI/2 -> PI/2]
            ///                                                     [Range: -90   -> 90]
            ///     - Roll: float for device/listener roll angle:   [Range: -PI/2 -> PI/2]
            ///                                                     [Range: -90   -> 90]
        }

        public void setRotationQuat(Mach1Point3D newRotationQuat)
        {
            Mach1DecodeCAPI_setRotationQuat(M1obj, (Mach1Point3D)newRotationQuat);
            /// Set current buffer/sample intended decoding orientation YPR in quaternion.
            ///
            /// - Parameters:
            ///     - W: float for device/listener W: [Range: -1.0->1.0]
            ///     - X: float for device/listener X: [Range: -1.0->1.0]
            ///     - Y: float for device/listener Y: [Range: -1.0->1.0]
            ///     - Z: float for device/listener Z: [Range: -1.0->1.0]
        }

        public Mach1Point3D getCurrentAngle()
        {
            return Mach1DecodeCAPI_getCurrentAngle(M1obj);
            /// Returns the Mach1Decode lib object's current 3D angle for feedback design
        }

        public long getCurrentTime()
        {
            return Mach1DecodeCAPI_getCurrentTime(M1obj);
            /// Returns the current elapsed time in milliseconds (ms) since Mach1Decode object creation
        }

        public long getLastCalculationTime()
        {
            return Mach1DecodeCAPI_getLastCalculationTime(M1obj);
            /// Returns the last elapsed time in milliseconds (ms) since Mach1Decode object creation
        }

        public string getLog()
        {
            return Marshal.PtrToStringAuto(Mach1DecodeCAPI_getLog(M1obj));
            /// Returns the current elapsed time in milliseconds (ms) since Mach1Decode object creation
        }
    }
}
