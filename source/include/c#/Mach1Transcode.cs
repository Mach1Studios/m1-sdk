//  Mach1 Spatial SDK
//  Copyright © 2017 Mach1, Corp. All rights reserved.

using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace Mach1
{
    public class Mach1Transcode : IDisposable
    {
#if UNITY_IOS && !UNITY_EDITOR
        internal const string libname = "__Internal";
#elif UNITY_STANDALONE_WIN || UNITY_EDITOR_WIN
        internal const string libname = "Mach1TranscodeCAPI";
#else
        internal const string libname = "libMach1TranscodeCAPI";
#endif

        [DllImport(libname)]
        internal static extern IntPtr Mach1TranscodeCAPI_create();

        [DllImport(libname)]
        internal static extern void Mach1TranscodeCAPI_delete(IntPtr M1obj);

        [DllImport(libname)]
        internal static extern int Mach1TranscodeCAPI_getInputNumChannels(IntPtr M1obj);

        [DllImport(libname)]
        internal static extern int Mach1TranscodeCAPI_getOutputNumChannels(IntPtr M1obj);

        [DllImport(libname)]
        internal static extern int Mach1TranscodeCAPI_getFormatFromString(IntPtr M1obj, IntPtr str);

        [DllImport(libname)]
        internal static extern IntPtr Mach1TranscodeCAPI_getFormatName(IntPtr M1obj, int fmt);

        [DllImport(libname)]
        internal static extern float Mach1TranscodeCAPI_processNormalization(IntPtr M1obj, IntPtr bufs, int numSamples);

        [DllImport(libname)]
        internal static extern void Mach1TranscodeCAPI_processMasterGain(IntPtr M1obj, IntPtr bufs, int numSamples, float masterGain);

        [DllImport(libname)]
        internal static extern float Mach1TranscodeCAPI_db2level(IntPtr M1obj, float db);

        [DllImport(libname)]
        internal static extern float Mach1TranscodeCAPI_level2db(IntPtr M1obj, float level);

        [DllImport(libname)]
        internal static extern void Mach1TranscodeCAPI_setLFESub(IntPtr M1obj, IntPtr subChannelIndices, int numChannels, int sampleRate);

        [DllImport(libname)]
        internal static extern void Mach1TranscodeCAPI_setSpatialDownmixer(IntPtr M1obj, float corrThreshold);

        [DllImport(libname)]
        internal static extern bool Mach1TranscodeCAPI_getSpatialDownmixerPossibility(IntPtr M1obj);

        [DllImport(libname)]
        internal static extern IntPtr Mach1TranscodeCAPI_getAvgSamplesDiff(IntPtr M1obj);

        [DllImport(libname)]
        internal static extern void Mach1TranscodeCAPI_setInputFormat(IntPtr M1obj, int inFmt);

        [DllImport(libname)]
        internal static extern void Mach1TranscodeCAPI_setInputFormatCustomPointsJson(IntPtr M1obj, IntPtr inJson);

        [DllImport(libname)]
        internal static extern void Mach1TranscodeCAPI_setInputFormatCustomPoints(IntPtr M1obj, IntPtr points, int count);

        [DllImport(libname)]
        internal static extern void Mach1TranscodeCAPI_setOutputFormat(IntPtr M1obj, int outFmt);

        [DllImport(libname)]
        internal static extern void Mach1TranscodeCAPI_setOutputFormatCustomPointsJson(IntPtr M1obj, IntPtr outJson);

        [DllImport(libname)]
        internal static extern void Mach1TranscodeCAPI_setOutputFormatCustomPoints(IntPtr M1obj, IntPtr points, int count);

        // void Mach1TranscodeCAPI_setCustomPointsSamplerCallback(IntPtr M1obj, Mach1Point3D*(* callback)(long long, int &));

        [DllImport(libname)]
        internal static extern bool Mach1TranscodeCAPI_processConversionPath(IntPtr M1obj);

        [DllImport(libname)]
        internal static extern void Mach1TranscodeCAPI_getMatrixConversion(IntPtr M1obj, IntPtr matrix);

        [DllImport(libname)]
        internal static extern void Mach1TranscodeCAPI_processConversion(IntPtr M1obj, IntPtr inBufs, IntPtr outBufs, int numSamples);

        [DllImport(libname)]
        internal static extern IntPtr Mach1TranscodeCAPI_getFormatConversionPath(IntPtr M1obj, ref int count);

        internal IntPtr M1obj;

        public Mach1Transcode()
        {
            M1obj = Mach1TranscodeCAPI_create();
        }

        public void Dispose()
        {
            Mach1TranscodeCAPI_delete(M1obj);
        }

        public int getInputNumChannels()
        {
            return Mach1TranscodeCAPI_getInputNumChannels(M1obj);
        }

        public int getOutputNumChannels()
        {
            return Mach1TranscodeCAPI_getOutputNumChannels(M1obj);
        }

        public int getFormatFromString(string str)
        {
            IntPtr strPtr = Marshal.StringToHGlobalAnsi(str);
            int fmt = Mach1TranscodeCAPI_getFormatFromString(M1obj, strPtr);
            Marshal.FreeHGlobal(strPtr);
            return fmt;
        }

        public string getFormatName(int fmt)
        {
            return Marshal.PtrToStringAnsi(Mach1TranscodeCAPI_getFormatName(M1obj, fmt));
        }

        public void processMasterGain(List<float[]> bufs, int numSamples, float masterGain = 1.0f)
        {
            IntPtr[] array = new IntPtr[bufs.Count];
            for (int i = 0; i < bufs.Count; i++)
            {
                array[i] = GCHandle.Alloc(bufs[i], GCHandleType.Pinned).AddrOfPinnedObject();
            }
            IntPtr ptr = GCHandle.Alloc(array, GCHandleType.Pinned).AddrOfPinnedObject();

            Mach1TranscodeCAPI_processMasterGain(M1obj, ptr, numSamples, masterGain);

            for (int i = 0; i < array.Length; i++)
            {
                GCHandle.FromIntPtr(array[i]).Free();
            }
            GCHandle.FromIntPtr(ptr).Free();
        }

        public float processNormalization(List<float[]> bufs, int numSamples)
        {
            IntPtr[] array = new IntPtr[bufs.Count];
            for (int i = 0; i < bufs.Count; i++)
            {
                array[i] = GCHandle.Alloc(bufs[i], GCHandleType.Pinned).AddrOfPinnedObject();
            }
            IntPtr ptr = GCHandle.Alloc(array, GCHandleType.Pinned).AddrOfPinnedObject();

            float peak = Mach1TranscodeCAPI_processNormalization(M1obj, ptr, numSamples);

            for (int i = 0; i < array.Length; i++)
            {
                GCHandle.FromIntPtr(array[i]).Free();
            }
            GCHandle.FromIntPtr(ptr).Free();

            return peak;
        }

        public float db2level(float db)
        {
            return Mach1TranscodeCAPI_db2level(M1obj, db);
        }

        public float level2db(float level)
        {
            return Mach1TranscodeCAPI_level2db(M1obj, level);
        }

        public void setLFESub(int[] subChannelIndices, int sampleRate)
        {
            GCHandle pinnedArray = GCHandle.Alloc(subChannelIndices, GCHandleType.Pinned);
            IntPtr pointer = pinnedArray.AddrOfPinnedObject();

            Mach1TranscodeCAPI_setLFESub(M1obj, pointer, subChannelIndices.Length, sampleRate);

            pinnedArray.Free();
        }

        public void setSpatialDownmixer(float corrThreshold = 0.1f)
        {
            Mach1TranscodeCAPI_setSpatialDownmixer(M1obj, corrThreshold);
        }

        public bool getSpatialDownmixerPossibility()
        {
            return Mach1TranscodeCAPI_getSpatialDownmixerPossibility(M1obj);
        }

        public float[] getAvgSamplesDiff()
        {
            float[] avg = new float[4];
            IntPtr ptr = Mach1TranscodeCAPI_getAvgSamplesDiff(M1obj);
            Marshal.Copy(ptr, avg, 0, 4);
            return avg;
        }

        public void setInputFormat(int inFmt)
        {
            Mach1TranscodeCAPI_setInputFormat(M1obj, inFmt);
        }

        public void setInputFormatCustomPointsJson(string inJson)
        {
            IntPtr strPtr = Marshal.StringToHGlobalAnsi(inJson);
            Mach1TranscodeCAPI_setInputFormatCustomPointsJson(M1obj, strPtr);
            Marshal.FreeHGlobal(strPtr);
        }

        public void setInputFormatCustomPoints(Mach1Point3D[] points)
        {
            GCHandle handle = GCHandle.Alloc(points, GCHandleType.Pinned);
            IntPtr pointsPtr = handle.AddrOfPinnedObject();

            Mach1TranscodeCAPI_setInputFormatCustomPoints(M1obj, pointsPtr, points.Length);

            handle.Free();
        }

        public void setOutputFormat(int outFmt)
        {
            Mach1TranscodeCAPI_setOutputFormat(M1obj, outFmt);
        }

        public void setOutputFormatCustomPointsJson(string strJson)
        {
            IntPtr strPtr = Marshal.StringToHGlobalAnsi(strJson);
            Mach1TranscodeCAPI_setOutputFormatCustomPointsJson(M1obj, strPtr);
            Marshal.FreeHGlobal(strPtr);
        }

        public void setOutputFormatCustomPoints(Mach1Point3D[] points)
        {
            GCHandle handle = GCHandle.Alloc(points, GCHandleType.Pinned);
            IntPtr pointsPtr = handle.AddrOfPinnedObject();

            Mach1TranscodeCAPI_setInputFormatCustomPoints(M1obj, pointsPtr, points.Length);

            handle.Free();
        }

        public bool processConversionPath()
        {
            return Mach1TranscodeCAPI_processConversionPath(M1obj);
        }

        public float[][] getMatrixConversion()
        {
            float[] matrix = new float[getInputNumChannels() * getOutputNumChannels()];
            IntPtr ptr = Marshal.AllocHGlobal(matrix.Length);

            Mach1TranscodeCAPI_getMatrixConversion(M1obj, ptr);
            Marshal.Copy(ptr, matrix, 0, matrix.Length);

            Marshal.FreeHGlobal(ptr);

            float[][] vec = new float[getOutputNumChannels()][];
            for (int i = 0; i < vec.Length; i++)
            {
                vec[i] = new float[getInputNumChannels()];
                for (int j = 0; j < vec[i].Length; j++)
                {
                    vec[i][j] = matrix[i * getInputNumChannels() + j];
                }
            }

            return vec;
        }

        public void processConversion(List<float[]> inBufs, List<float[]> outBufs)
        {
            if (inBufs.Count == 0 || outBufs.Count == 0)
                return;

            IntPtr[] pinnedArrays1 = new IntPtr[inBufs.Count];
            for (int i = 0; i < inBufs.Count; i++)
            {
                pinnedArrays1[i] = GCHandle.Alloc(inBufs[i], GCHandleType.Pinned).AddrOfPinnedObject();
            }
            IntPtr ptr1 = GCHandle.Alloc(pinnedArrays1, GCHandleType.Pinned).AddrOfPinnedObject();

            IntPtr[] pinnedArrays2 = new IntPtr[outBufs.Count];
            for (int i = 0; i < outBufs.Count; i++)
            {
                pinnedArrays2[i] = GCHandle.Alloc(outBufs[i], GCHandleType.Pinned).AddrOfPinnedObject();
            }
            IntPtr ptr2 = GCHandle.Alloc(pinnedArrays2, GCHandleType.Pinned).AddrOfPinnedObject();

            Mach1TranscodeCAPI_processConversion(M1obj, ptr1, ptr2, inBufs[0].Length);

            for (int i = 0; i < pinnedArrays1.Length; i++)
            {
                GCHandle.FromIntPtr(pinnedArrays1[i]).Free();
            }
            GCHandle.FromIntPtr(ptr1).Free();

            for (int i = 0; i < pinnedArrays2.Length; i++)
            {
                GCHandle.FromIntPtr(pinnedArrays2[i]).Free();
            }
            GCHandle.FromIntPtr(ptr2).Free();
        }

        public int[] getFormatConversionPath()
        {
            int count = 0;
            IntPtr ptr = Mach1TranscodeCAPI_getFormatConversionPath(M1obj, ref count);

            int[] vec = new int[count];
            Marshal.Copy(ptr, vec, 0, count);
            return vec;
        }
    }
}
