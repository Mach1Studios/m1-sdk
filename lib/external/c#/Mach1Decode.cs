//  Mach1 SDK
//  Copyright © 2018 Mach1. All rights reserved.

using System;
using System.Runtime.InteropServices;

namespace Mach1
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Mach1Point3D
    {
        public float x;
        public float y;
        public float z;
    }

    public enum Mach1PlatformType : int
    {
        Mach1PlatformDefault = 0, Mach1PlatformUnity, Mach1PlatformUE, Mach1PlatformOfEasyCam, Mach1PlatformAndroid, Mach1PlatformiOSPortrait, Mach1PlatformiOSLandscape
    };

    public enum Mach1DecodeAlgoType : int
    {
        Mach1DecodeAlgoSpatial = 0, Mach1DecodeAlgoAltSpatial, Mach1DecodeAlgoHorizon, Mach1DecodeAlgoHorizonPairs, Mach1DecodeAlgoSpatialPairs
    };

    public class Mach1Decode : IDisposable
    {
        internal const string libname = "Mach1DecodeCAPI";

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
        }

        public void setPlatformType(Mach1PlatformType type)
        {
            Mach1DecodeCAPI_setPlatformType(M1obj, type);
        }

        public void decode(float Yaw, float Pitch, float Roll, ref float[] data,  int bufferSize = 0, int sampleIndex = 0)
        {
            //if(data.Length < 18) data = new float[18];
            GCHandle pinnedArray = GCHandle.Alloc(data, GCHandleType.Pinned);
            IntPtr pointer = pinnedArray.AddrOfPinnedObject();

            Mach1DecodeCAPI_decode(M1obj, Yaw, Pitch, Roll, pointer, bufferSize, sampleIndex);

            pinnedArray.Free();
        } 
        
        public void setFilterSpeed(float filterSpeed)
        {
            Mach1DecodeCAPI_setFilterSpeed(M1obj, (float)filterSpeed);
        }

        public void beginBuffer()
        {
            Mach1DecodeCAPI_beginBuffer(M1obj);
        }

        public void endBuffer()
        {
            Mach1DecodeCAPI_endBuffer(M1obj);
        }

        public long getCurrentTime()
        {
            return Mach1DecodeCAPI_getCurrentTime(M1obj);
        }

        public string getLog()
        {
            return Mach1DecodeCAPI_getLog(M1obj);
        }

        public Mach1Point3D getCurrentAngle()
        {
            return Mach1DecodeCAPI_getCurrentAngle(M1obj);
        }

    }
}
