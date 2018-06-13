//  Mach1 SDK
//  Copyright © 2017 Mach1. All rights reserved.

using System;
using System.Runtime.InteropServices;

namespace Mach1
{
    public class Mach1DecodePositional : IDisposable
    {
		#if UNITY_IOS
		internal const string libname = "__Internal";
		#else
		internal const string libname = "Mach1DecodePositionalCAPI";
		#endif

        [DllImport(libname)]
        internal static extern IntPtr Mach1DecodePositionalCAPI_create();

        [DllImport(libname)]
        internal static extern void Mach1DecodePositionalCAPI_delete(IntPtr M1obj);

        [DllImport(libname)]
        internal static extern void Mach1DecodePositionalCAPI_setPlatformType(IntPtr M1obj, Mach1PlatformType type);

        [DllImport(libname)]
        internal static extern void Mach1DecodePositionalCAPI_setCameraPosition(IntPtr M1obj, Mach1Point3D point);

        [DllImport(libname)]
        internal static extern void Mach1DecodePositionalCAPI_setCameraRotation(IntPtr M1obj, Mach1Point3D point);

        [DllImport(libname)]
        internal static extern void Mach1DecodePositionalCAPI_setDecoderAlgoPosition(IntPtr M1obj, Mach1Point3D point);

        [DllImport(libname)]
        internal static extern void Mach1DecodePositionalCAPI_setDecoderAlgoRotation(IntPtr M1obj, Mach1Point3D point);

        [DllImport(libname)]
        internal static extern void Mach1DecodePositionalCAPI_setDecoderAlgoScale(IntPtr M1obj, Mach1Point3D point);

        [DllImport(libname)]
        internal static extern void Mach1DecodePositionalCAPI_evaluatePostionResults(IntPtr M1obj);

        [DllImport(libname)]
        internal static extern float Mach1DecodePositionalCAPI_getVolumeWalls(IntPtr M1obj);

        [DllImport(libname)]
        internal static extern float Mach1DecodePositionalCAPI_getVolumeRoom(IntPtr M1obj);

        [DllImport(libname)]
        internal static extern Mach1Point3D Mach1DecodePositionalCAPI_getVolumeRotation(IntPtr M1obj);

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
        }

        public void setCameraPosition(Mach1Point3D point)
        {
            Mach1DecodePositionalCAPI_setCameraPosition(M1obj, point);
        }

        public void setCameraRotation(Mach1Point3D point)
        {
            Mach1DecodePositionalCAPI_setCameraRotation(M1obj, point);
        }

        public void setDecoderAlgoPosition(Mach1Point3D point)
        {
            Mach1DecodePositionalCAPI_setDecoderAlgoPosition(M1obj, point);
        }

        public void setDecoderAlgoRotation(Mach1Point3D point)
        {
            Mach1DecodePositionalCAPI_setDecoderAlgoRotation(M1obj, point);
        }

        public void setDecoderAlgoScale(Mach1Point3D point)
        {
            Mach1DecodePositionalCAPI_setDecoderAlgoScale(M1obj, point);
        }

        public void evaluatePostionResults(Mach1Point3D point)
        {
            Mach1DecodePositionalCAPI_evaluatePostionResults(M1obj);
        }

        public float getVolumeWalls(Mach1Point3D point)
        {
            return Mach1DecodePositionalCAPI_getVolumeWalls(M1obj);
        }

        public float getVolumeRoom(Mach1Point3D point)
        {
            return Mach1DecodePositionalCAPI_getVolumeRoom(M1obj);
        }

        public Mach1Point3D getVolumeRotation()
        {
            return Mach1DecodePositionalCAPI_getVolumeRotation(M1obj);
        }

    }
}
