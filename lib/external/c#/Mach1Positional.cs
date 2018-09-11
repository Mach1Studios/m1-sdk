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
        internal static extern void Mach1DecodePositionalCAPI_setDecodeAlgoType(IntPtr M1obj, Mach1DecodeAlgoType type);

        // settings

        [DllImport(libname)]
        internal static extern void Mach1DecodePositionalCAPI_setUseBlendMode(IntPtr M1obj, bool useBlendMode);

        [DllImport(libname)]
        internal static extern void Mach1DecodePositionalCAPI_setIgnoreTopBottom(IntPtr M1obj, bool ignoreTopBottom);

        [DllImport(libname)]
        internal static extern void Mach1DecodePositionalCAPI_setMuteWhenOutsideObject(IntPtr M1obj, bool muteWhenOutsideObject);

        [DllImport(libname)]
        internal static extern void Mach1DecodePositionalCAPI_setMuteWhenInsideObject(IntPtr M1obj, bool muteWhenInsideObject);

        [DllImport(libname)]
        internal static extern void Mach1DecodePositionalCAPI_setUseFalloff(IntPtr M1obj, bool useFalloff);

        [DllImport(libname)]
        internal static extern void Mach1DecodePositionalCAPI_setFalloffCurve(IntPtr M1obj, float falloffCurve);

        [DllImport(libname)]
        internal static extern void Mach1DecodePositionalCAPI_setFalloffCurveBlendMode(IntPtr M1obj, float falloffCurveBlendMode);

        [DllImport(libname)]
        internal static extern void Mach1DecodePositionalCAPI_setUseClosestPointRotationMuteInside(IntPtr M1obj, bool useClosestPointRotationMuteInside);

        [DllImport(libname)]
        internal static extern void Mach1DecodePositionalCAPI_setUseYawForRotation(IntPtr M1obj, bool useYawForRotation);

        [DllImport(libname)]
        internal static extern void Mach1DecodePositionalCAPI_setUsePitchForRotation(IntPtr M1obj, bool usePitchForRotation);

        [DllImport(libname)]
        internal static extern void Mach1DecodePositionalCAPI_setUseRollForRotation(IntPtr M1obj, bool useRollForRotation);

        // parameters for algo

        [DllImport(libname)]
        internal static extern void Mach1DecodePositionalCAPI_setPlatformType(IntPtr M1obj, bool type);

        [DllImport(libname)]
        internal static extern void Mach1DecodePositionalCAPI_setCameraPosition(IntPtr M1obj, Mach1Point3D point);

        [DllImport(libname)]
        internal static extern void Mach1DecodePositionalCAPI_setCameraRotation(IntPtr M1obj, Mach1Point3D point);

        [DllImport(libname)]
        internal static extern void Mach1DecodePositionalCAPI_setCameraRotationQuat(IntPtr M1obj, Mach1Point4D quat);

        [DllImport(libname)]
        internal static extern void Mach1DecodePositionalCAPI_setDecoderAlgoPosition(IntPtr M1obj, Mach1Point3D point);

        [DllImport(libname)]
        internal static extern void Mach1DecodePositionalCAPI_setDecoderAlgoRotation(IntPtr M1obj, Mach1Point3D point);

        [DllImport(libname)]
        internal static extern void Mach1DecodePositionalCAPI_setDecoderAlgoRotationQuat(IntPtr M1obj, Mach1Point4D quat);

        [DllImport(libname)]
        internal static extern void Mach1DecodePositionalCAPI_setDecoderAlgoScale(IntPtr M1obj, Mach1Point3D point);

        [DllImport(libname)]
        internal static extern void Mach1DecodePositionalCAPI_evaluatePostionResults(IntPtr M1obj);

        [DllImport(libname)]
        internal static extern void Mach1DecodePositionalCAPI_getVolumesWalls(IntPtr M1obj, IntPtr data);

        [DllImport(libname)]
        internal static extern void Mach1DecodePositionalCAPI_getVolumesRoom(IntPtr M1obj, IntPtr data);

        [DllImport(libname)]
        internal static extern float Mach1DecodePositionalCAPI_getDist(IntPtr M1obj);

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
		
        public void setDecodeAlgoType(Mach1DecodeAlgoType type)
		{
			Mach1DecodePositionalCAPI_setDecodeAlgoType(M1obj, type);
		}

        // settings

        public void setUseBlendMode(bool useBlendMode)
        {
            Mach1DecodePositionalCAPI_setUseBlendMode(M1obj, useBlendMode);
        }

        public void setIgnoreTopBottom(bool ignoreTopBottom)
        {
            Mach1DecodePositionalCAPI_setIgnoreTopBottom(M1obj, ignoreTopBottom);
        }

        public void setMuteWhenOutsideObject(bool muteWhenOutsideObject)
        {
            Mach1DecodePositionalCAPI_setMuteWhenOutsideObject(M1obj, muteWhenOutsideObject);
        }

        public void setMuteWhenInsideObject(bool muteWhenInsideObject)
        {
            Mach1DecodePositionalCAPI_setMuteWhenInsideObject(M1obj, muteWhenInsideObject);
        }

        public void setUseFalloff(bool useFalloff)
        {
            Mach1DecodePositionalCAPI_setUseFalloff(M1obj, useFalloff);
        }

        public void setFalloffCurve(float falloffCurve)
        {
            Mach1DecodePositionalCAPI_setFalloffCurve(M1obj, falloffCurve);
        }

        public void setFalloffCurveBlendMode(float falloffCurveBlendMode)
        {
            Mach1DecodePositionalCAPI_setFalloffCurveBlendMode(M1obj, falloffCurveBlendMode);
        }

        public void setUseClosestPointRotationMuteInside(bool useClosestPointRotationMuteInside)
        {
            Mach1DecodePositionalCAPI_setUseClosestPointRotationMuteInside(M1obj, useClosestPointRotationMuteInside);
        }

        public void setUseYawForRotation(bool useYawForRotation)
        {
            Mach1DecodePositionalCAPI_setUseYawForRotation(M1obj, useYawForRotation);
        }

        public void setUsePitchForRotation(bool usePitchForRotation)
        {
            Mach1DecodePositionalCAPI_setUsePitchForRotation(M1obj, usePitchForRotation);
        }

        public void setUseRollForRotation(bool useRollForRotation)
        {
            Mach1DecodePositionalCAPI_setUseRollForRotation(M1obj, useRollForRotation);
        }
 
        // parameters for algo
        public void setCameraPosition(Mach1Point3D point)
        {
            Mach1DecodePositionalCAPI_setCameraPosition(M1obj, point);
        }

        public void setCameraRotation(Mach1Point3D point)
        {
            Mach1DecodePositionalCAPI_setCameraRotation(M1obj, point);
        }

        public void setCameraRotationQuat(Mach1Point4D quat)
        {
            Mach1DecodePositionalCAPI_setCameraRotationQuat(M1obj, quat);
        }

        public void setDecoderAlgoPosition(Mach1Point3D point)
        {
            Mach1DecodePositionalCAPI_setDecoderAlgoPosition(M1obj, point);
        }

        public void setDecoderAlgoRotation(Mach1Point3D point)
        {
            Mach1DecodePositionalCAPI_setDecoderAlgoRotation(M1obj, point);
        }

        public void setDecoderAlgoRotationQuat(Mach1Point4D quat)
        {
            Mach1DecodePositionalCAPI_setDecoderAlgoRotationQuat(M1obj, quat);
        }

        public void setDecoderAlgoScale(Mach1Point3D point)
        {
            Mach1DecodePositionalCAPI_setDecoderAlgoScale(M1obj, point);
        }

        public void evaluatePostionResults()
        {
            Mach1DecodePositionalCAPI_evaluatePostionResults(M1obj);
        }

        public void getVolumesWalls(ref float[] data)
        {
            //if(data.Length < 18) data = new float[18];
            GCHandle pinnedArray = GCHandle.Alloc(data, GCHandleType.Pinned);
            IntPtr pointer = pinnedArray.AddrOfPinnedObject();

            Mach1DecodePositionalCAPI_getVolumesWalls(M1obj, pointer);

            pinnedArray.Free();
        }

        public void getVolumesRoom(ref float[] data)
        {
            //if(data.Length < 18) data = new float[18];
            GCHandle pinnedArray = GCHandle.Alloc(data, GCHandleType.Pinned);
            IntPtr pointer = pinnedArray.AddrOfPinnedObject();

            Mach1DecodePositionalCAPI_getVolumesRoom(M1obj, pointer);

            pinnedArray.Free();
        }

        public float getDist()
        {
            return Mach1DecodePositionalCAPI_getDist(M1obj);
        }

        public Mach1Point3D getVolumeRotation()
        {
            return Mach1DecodePositionalCAPI_getVolumeRotation(M1obj);
        }

    }
}
