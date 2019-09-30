//  Mach1 SDK
//  Copyright © 2017 Mach1. All rights reserved.

using System;
using System.Runtime.InteropServices;

namespace Mach1
{
    public class Mach1Encode : IDisposable
    {
#if UNITY_IOS && !UNITY_EDITOR
        internal const string libname = "__Internal";
#elif UNITY_STANDALONE_WIN || UNITY_EDITOR_WIN
        internal const string libname = "Mach1EncodeCAPI";
#else
        internal const string libname = "libMach1EncodeCAPI";
#endif

        [DllImport(libname)]
        internal static extern IntPtr Mach1EncodeCAPI_create();

        [DllImport(libname)]
        internal static extern void Mach1EncodeCAPI_delete(IntPtr M1obj);


        [DllImport(libname)]
        internal static extern IntPtr Mach1EncodeCAPI_getPoints(IntPtr M1obj);

        [DllImport(libname)]
        internal static extern IntPtr Mach1EncodeCAPI_getGains(IntPtr M1obj);

        [DllImport(libname)]
        internal static extern IntPtr Mach1EncodeCAPI_getPointsNames(IntPtr M1obj);

        [DllImport(libname)]
        internal static extern IntPtr Mach1EncodeCAPI_getGainsForInputChannelNamed(IntPtr M1obj, [MarshalAs(UnmanagedType.LPStr)] string pointName);

        [DllImport(libname)]
        internal static extern void Mach1EncodeCAPI_generatePointResults(IntPtr M1obj);


        [DllImport(libname)]
        internal static extern int Mach1EncodeCAPI_getPointsCount(IntPtr M1obj);

        [DllImport(libname)]
        internal static extern IntPtr Mach1EncodeCAPI_getResultingVolumesDecoded(IntPtr M1obj, Mach1DecodeAlgoType decodeType, IntPtr decodeResult);


        [DllImport(libname)]
        internal static extern void Mach1EncodeCAPI_setInputMode(IntPtr M1obj, Mach1EncodeInputModeType inputMode);

        [DllImport(libname)]
        internal static extern void Mach1EncodeCAPI_setOutputMode(IntPtr M1obj, Mach1EncodeOutputModeType outputMode);

        [DllImport(libname)]
        internal static extern void Mach1EncodeCAPI_setRotation(IntPtr M1obj, float rotation);

        [DllImport(libname)]
        internal static extern void Mach1EncodeCAPI_setDiverge(IntPtr M1obj, float diverge);

        [DllImport(libname)]
        internal static extern void Mach1EncodeCAPI_setPitch(IntPtr M1obj, float pitch);

        [DllImport(libname)]
        internal static extern void Mach1EncodeCAPI_setStereoRotate(IntPtr M1obj, float sRotate);

        [DllImport(libname)]
        internal static extern void Mach1EncodeCAPI_setStereoSpread(IntPtr M1obj, float sSpread);

        [DllImport(libname)]
        internal static extern void Mach1EncodeCAPI_setAutoOrbit(IntPtr M1obj, bool autoOrbit);

        [DllImport(libname)]
        internal static extern void Mach1EncodeCAPI_setIsotropicEncode(IntPtr M1obj, bool isotropicEncode);

        internal IntPtr M1obj;

        public Mach1Encode()
        {
            M1obj = Mach1EncodeCAPI_create();
        }

        public void Dispose()
        {
            Mach1EncodeCAPI_delete(M1obj);
        }

        public Mach1Point3D[] getPoints()
        {
            IntPtr[] pointsPtr = new IntPtr[getPointsCount()];
            Marshal.Copy(Mach1EncodeCAPI_getPoints(M1obj), pointsPtr, 0, getPointsCount());

            Mach1Point3D[] points = new Mach1Point3D[getPointsCount()];
            for (int i = 0; i < points.Length; i++)
            {
                points[i] = (Mach1Point3D)Marshal.PtrToStructure(pointsPtr[i], typeof(Mach1Point3D));
            }
            return points;
        }

        public float[][] getGains()
        {
            IntPtr[] gainsPtr = new IntPtr[getPointsCount()];
            Marshal.Copy(Mach1EncodeCAPI_getGains(M1obj), gainsPtr, 0, getPointsCount());

            float[][] gains = new float[getPointsCount()][];
            for (int i = 0; i < gains.Length; i++)
            {
                gains[i] = new float[8];
                Marshal.Copy(gainsPtr[i], gains[i], 0, gains[i].Length);
            }
            return gains;
        }

        public string[] getPointsNames()
        {
            IntPtr[] namesPtr = new IntPtr[getPointsCount()];
            Marshal.Copy(Mach1EncodeCAPI_getPointsNames(M1obj), namesPtr, 0, getPointsCount());

            string[] names = new string[getPointsCount()];
            for(int i=0; i<names.Length; i++)
            {
                names[i] = Marshal.PtrToStringAnsi(namesPtr[i]);
            }
            return names;
        }

        public float[] getGainsForInputChannelNamed(string pointName)
        {
            IntPtr gainsPtr = Mach1EncodeCAPI_getGainsForInputChannelNamed(M1obj, pointName);

            float[] gains = new float[8];
            Marshal.Copy(gainsPtr, gains, 0, gains.Length);
            return gains;
        }

        public void generatePointResults()
        {
            Mach1EncodeCAPI_generatePointResults(M1obj);
        }

        public int getPointsCount()
        {
            return Mach1EncodeCAPI_getPointsCount(M1obj);
        }

        public float[] getResultingVolumesDecoded(Mach1DecodeAlgoType decodeType, ref float[] decodeResult)
        {
            GCHandle pinnedArray = GCHandle.Alloc(decodeResult, GCHandleType.Pinned);
            IntPtr pointer = pinnedArray.AddrOfPinnedObject();

            IntPtr volumesPtr = Mach1EncodeCAPI_getResultingVolumesDecoded(M1obj, decodeType, pointer);

            float[] volumes = new float[14];
            Marshal.Copy(volumesPtr, volumes, 0, volumes.Length);

            pinnedArray.Free();

            return volumes;
        }

        public void setInputMode(Mach1EncodeInputModeType inputMode)
        {
            Mach1EncodeCAPI_setInputMode(M1obj, inputMode);
        }

        public void setOutputMode(Mach1EncodeOutputModeType outputMode)
        {
            Mach1EncodeCAPI_setOutputMode(M1obj, outputMode);
        }

        public void setRotation(float rotation)
        {
            Mach1EncodeCAPI_setRotation(M1obj, rotation);
        }

        public void setDiverge(float diverge)
        {
            Mach1EncodeCAPI_setDiverge(M1obj, diverge);
        }

        public void setPitch(float pitch)
        {
            Mach1EncodeCAPI_setPitch(M1obj, pitch);
        }

        public void setStereoRotate(float sRotate)
        {
            Mach1EncodeCAPI_setStereoRotate(M1obj, sRotate);
        }

        public void setStereoSpread(float sSpread)
        {
            Mach1EncodeCAPI_setStereoSpread(M1obj, sSpread);
        }

        public void setAutoOrbit(bool autoOrbit)
        {
            Mach1EncodeCAPI_setAutoOrbit(M1obj, autoOrbit);
        }

        public void setIsotropicEncode(bool isotropicEncode)
        {
            Mach1EncodeCAPI_setIsotropicEncode(M1obj, isotropicEncode);
        }

    }
}
