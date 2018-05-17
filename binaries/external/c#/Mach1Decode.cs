//  Mach1 SDK
//  Copyright © 2017 Mach1. All rights reserved.

using System;
using System.Runtime.InteropServices;

namespace Mach1
{
    public class M1Decode : IDisposable
    {
        public enum AngularSettingsType : int
        {
            m1Default = 0, m1Unity, m1UE, m1oFEasyCam, m1Android, m1iOSPortrait, m1iOSLandscape
        };

        internal const string libname = "Mach1DecodeCAPI";

        [DllImport(libname)]
        internal static extern IntPtr Mach1DecodeCAPI_create();

        [DllImport(libname)]
        internal static extern void Mach1DecodeCAPI_delete(IntPtr M1obj);

        [DllImport(libname)]
        internal static extern IntPtr Mach1DecodeCAPI_horizonAlgo(IntPtr M1obj, float Yaw, float Pitch, float Roll, int bufferSize = 0, int sampleIndex = 0);

        [DllImport(libname)]
        internal static extern IntPtr Mach1DecodeCAPI_horizonPairsAlgo(IntPtr M1obj, float Yaw, float Pitch, float Roll, int bufferSize = 0, int sampleIndex = 0);

        [DllImport(libname)]
        internal static extern IntPtr Mach1DecodeCAPI_spatialAlgo(IntPtr M1obj, float Yaw, float Pitch, float Roll, int bufferSize = 0, int sampleIndex = 0);

        [DllImport(libname)]
        internal static extern IntPtr Mach1DecodeCAPI_spatialAltAlgo(IntPtr M1obj, float Yaw, float Pitch, float Roll, int bufferSize = 0, int sampleIndex = 0);

        [DllImport(libname)]
        internal static extern IntPtr Mach1DecodeCAPI_spatialPairsAlgo(IntPtr M1obj, float Yaw, float Pitch, float Roll, int bufferSize = 0, int sampleIndex = 0);

        [DllImport(libname)]
        internal static extern void Mach1DecodeCAPI_setAngularSettingsType(IntPtr M1obj, int type);

        [DllImport(libname)]
        internal static extern void Mach1DecodeCAPI_beginBuffer(IntPtr M1obj);

        [DllImport(libname)]
        internal static extern void Mach1DecodeCAPI_endBuffer(IntPtr M1obj);

        [DllImport(libname)]
        internal static extern long Mach1DecodeCAPI_getCurrentTime(IntPtr M1obj);

        internal IntPtr M1obj;

        public M1Decode()
        {
            M1obj = Mach1DecodeCAPI_create();
        }

        public void Dispose()
        {
            Mach1DecodeCAPI_delete(M1obj);
        }

        public float[] horizonAlgo(float Yaw, float Pitch, float Roll, int bufferSize = 0, int sampleIndex = 0)
        {
            IntPtr ptr = Mach1DecodeCAPI_horizonAlgo(M1obj, Yaw, Pitch, Roll, bufferSize, sampleIndex);

            float[] data = new float[10];
            Marshal.Copy(ptr, data, 0, data.Length);
            return data;
        }

        public float[] horizonPairsAlgo(float Yaw, float Pitch, float Roll, int bufferSize = 0, int sampleIndex = 0)
        {
            IntPtr ptr = Mach1DecodeCAPI_horizonPairsAlgo(M1obj, Yaw, Pitch, Roll, bufferSize, sampleIndex);

            float[] data = new float[8];
            Marshal.Copy(ptr, data, 0, data.Length);
            return data;
        }


        public float[] spatialAlgo(float Yaw, float Pitch, float Roll, int bufferSize = 0, int sampleIndex = 0)
        {
            IntPtr ptr = Mach1DecodeCAPI_spatialAlgo(M1obj, Yaw, Pitch, Roll, bufferSize, sampleIndex);

            float[] data = new float[18];
            Marshal.Copy(ptr, data, 0, data.Length);
            return data;
        }

        public float[] spatialAltAlgo(float Yaw, float Pitch, float Roll, int bufferSize = 0, int sampleIndex = 0)
        {
            IntPtr ptr = Mach1DecodeCAPI_spatialAltAlgo(M1obj, Yaw, Pitch, Roll, bufferSize, sampleIndex);

            float[] data = new float[18];
            Marshal.Copy(ptr, data, 0, data.Length);
            return data;
        }

        public float[] spatialPairsAlgo(float Yaw, float Pitch, float Roll, int bufferSize = 0, int sampleIndex = 0)
        {
            IntPtr ptr = Mach1DecodeCAPI_spatialPairsAlgo(M1obj, Yaw, Pitch, Roll, bufferSize, sampleIndex);

            float[] data = new float[16];
            Marshal.Copy(ptr, data, 0, data.Length);
            return data;
        }

        public void setAngularSettingsType(AngularSettingsType type)
        {
            Mach1DecodeCAPI_setAngularSettingsType(M1obj, (int)type);
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

    }
}
