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
        internal static extern IntPtr Mach1DecodeCAPI_decode(IntPtr M1obj, float Yaw, float Pitch, float Roll, IntPtr data, int bufferSize = 0, int sampleIndex = 0);

        [DllImport(libname)]
        internal static extern void Mach1DecodeCAPI_setFilterSpeed(IntPtr M1obj, float filterSpeed);

        [DllImport(libname)]
        internal static extern void Mach1DecodeCAPI_beginBuffer(IntPtr M1obj);

        [DllImport(libname)]
        internal static extern void Mach1DecodeCAPI_endBuffer(IntPtr M1obj);

        [DllImport(libname)]
        internal static extern long Mach1DecodeCAPI_getCurrentTime(IntPtr M1obj);

        [DllImport(libname)]
        internal static extern string Mach1DecodeCAPI_getLog(IntPtr M1obj);

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
            ///     - Mach1DecodeAlgoSpatial_8 (default spatial | 8 channels)
            ///     - Mach1DecodeAlgoHorizon_4 (compass / yaw | 4 channels)
            ///     - Mach1DecodeAlgoHorizonPairs (compass / yaw | 4x stereo mastered pairs)
            ///     - Mach1DecodeAlgoSpatialPairs (experimental periphonic pairs | 8x stereo mastered pairs)
            ///     - Mach1DecodeAlgoSpatial_12 (higher order spatial | 12 channels)
            ///     - Mach1DecodeAlgoSpatial_14 (higher order spatial | 14 channels)
            ///     - Mach1DecodeAlgoSpatial_18 (higher order spatial | 18 channels)
            ///     - Mach1DecodeAlgoSpatial_22 (higher order spatial | 22 channels)
            ///     - Mach1DecodeAlgoSpatial_32 (higher order spatial | 32 channels)
            ///     - Mach1DecodeAlgoSpatial_36 (higher order spatial | 36 channels)
            ///     - Mach1DecodeAlgoSpatial_48 (higher order spatial | 48 channels)
            ///     - Mach1DecodeAlgoSpatial_60 (higher order spatial | 60 channels)
        }

        public void setPlatformType(Mach1PlatformType type)
        {
            Mach1DecodeCAPI_setPlatformType(M1obj, type);
            /// Set the device's angle order and convention if applicable
            ///
            /// - Parameters:
            ///     - Mach1PlatformDefault = 0
            ///     - Mach1PlatformUnity = 1
            ///     - Mach1PlatformUE = 2
            ///     - Mach1PlatformOfEasyCam = 3
            ///     - Mach1PlatformAndroid = 4
            ///     - Mach1PlatformiOS = 5
        }

        public void decode(float Yaw, float Pitch, float Roll, ref float[] data,  int bufferSize = 0, int sampleIndex = 0)
        {
            //if(data.Length < 18) data = new float[18];
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

        public void beginBuffer()
        {
            Mach1DecodeCAPI_beginBuffer(M1obj);
            /// Call this function before reading from the Mach1Decode buffer
        }

        public void endBuffer()
        {
            Mach1DecodeCAPI_endBuffer(M1obj);
            /// Call this function after reading from the Mach1Decode buffer
        }

        public long getCurrentTime()
        {
            return Mach1DecodeCAPI_getCurrentTime(M1obj);
            /// Returns the current elapsed time in milliseconds (ms) since Mach1Decode object creation
        }

        public string getLog()
        {
            return Mach1DecodeCAPI_getLog(M1obj);
            /// Returns the current elapsed time in milliseconds (ms) since Mach1Decode object creation
        }

        public Mach1Point3D getCurrentAngle()
        {
            return Mach1DecodeCAPI_getCurrentAngle(M1obj);
            /// Returns the Mach1Decode lib object's current 3D angle for feedback design
        }

    }
}
