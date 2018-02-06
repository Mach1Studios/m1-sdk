//  Mach1 SDK
//  Copyright © 2017 Mach1. All rights reserved.

using System;
using System.Runtime.InteropServices;

namespace Mach1
{
    public class Mach1Decode : IDisposable
    {
		#if UNITY_IOS
		internal const string libname = "__Internal";
		#else
		internal const string libname = "Mach1DecodeCAPI";
		#endif

		[DllImport(libname)]
        internal static extern IntPtr Mach1DecodeCAPI_create();

        [DllImport(libname)]
        internal static extern void Mach1DecodeCAPI_delete(IntPtr M1obj);

        [DllImport(libname)]
        internal static extern IntPtr Mach1DecodeCAPI_horizonAlgo(IntPtr M1obj, float Yaw, float Pitch, float Roll, bool smoothAngles);

        [DllImport(libname)]
        internal static extern IntPtr Mach1DecodeCAPI_horizonPairsAlgo(IntPtr M1obj, float Yaw, float Pitch, float Roll, bool smoothAngles);

        [DllImport(libname)]
        internal static extern IntPtr Mach1DecodeCAPI_spatialAlgo(IntPtr M1obj, float Yaw, float Pitch, float Roll, bool smoothAngles);

        [DllImport(libname)]
        internal static extern IntPtr Mach1DecodeCAPI_spatialAltAlgo(IntPtr M1obj, float Yaw, float Pitch, float Roll, bool smoothAngles);

        [DllImport(libname)]
        internal static extern IntPtr Mach1DecodeCAPI_spatialPairsAlgo(IntPtr M1obj, float Yaw, float Pitch, float Roll, bool smoothAngles);

        [DllImport(libname)]
        internal static extern long Mach1DecodeCAPI_getCurrentTime(IntPtr M1obj);

        internal IntPtr M1obj;

        public Mach1Decode()
        {
            M1obj = Mach1DecodeCAPI_create();
        }

        public void Dispose()
        {
            Mach1DecodeCAPI_delete(M1obj);
        }

        public float[] horizonAlgo(float Yaw, float Pitch, float Roll, bool smoothAngles)
        {
            IntPtr ptr = Mach1DecodeCAPI_horizonAlgo(M1obj, Yaw, Pitch, Roll, smoothAngles);

            float[] data = new float[10];
            Marshal.Copy(ptr, data, 0, data.Length);
            return data;
        }

        public float[] horizonPairsAlgo(float Yaw, float Pitch, float Roll, bool smoothAngles)
        {
            IntPtr ptr = Mach1DecodeCAPI_horizonPairsAlgo(M1obj, Yaw, Pitch, Roll, smoothAngles);

            float[] data = new float[16];
            Marshal.Copy(ptr, data, 0, data.Length);
            return data;
        }


        public float[] spatialAlgo(float Yaw, float Pitch, float Roll, bool smoothAngles)
        {
            IntPtr ptr = Mach1DecodeCAPI_spatialAlgo(M1obj, Yaw, Pitch, Roll, smoothAngles);

            float[] data = new float[18];
            Marshal.Copy(ptr, data, 0, data.Length);
            return data;
        }

        public float[] spatialAltAlgo(float Yaw, float Pitch, float Roll, bool smoothAngles)
        {
            IntPtr ptr = Mach1DecodeCAPI_spatialAltAlgo(M1obj, Yaw, Pitch, Roll, smoothAngles);

            float[] data = new float[18];
            Marshal.Copy(ptr, data, 0, data.Length);
            return data;
        }

        public float[] spatialPairsAlgo(float Yaw, float Pitch, float Roll, bool smoothAngles)
        {
            IntPtr ptr = Mach1DecodeCAPI_spatialPairsAlgo(M1obj, Yaw, Pitch, Roll, smoothAngles);

            float[] data = new float[32];
            Marshal.Copy(ptr, data, 0, data.Length);
            return data;
        }

        public long getCurrentTime()
        {
            return Mach1DecodeCAPI_getCurrentTime(M1obj);
        }
    }
}
