//  Mach1 SDK
//  Copyright © 2017-2021 Mach1. All rights reserved.

using System;
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
        internal static extern int Mach1TranscodeCAPI_getInputFormat(IntPtr M1obj);

        [DllImport(libname)]
        internal static extern int Mach1TranscodeCAPI_getOutputFormat(IntPtr M1obj);

        [DllImport(libname)]
        internal static extern int Mach1TranscodeCAPI_getInputNumChannels(IntPtr M1obj);

        [DllImport(libname)]
        internal static extern int Mach1TranscodeCAPI_getOutputNumChannels(IntPtr M1obj);

        [DllImport(libname)]
        internal static extern int Mach1TranscodeCAPI_getFormatFromString(IntPtr M1obj, string str);

        [DllImport(libname)]
        internal static extern IntPtr Mach1TranscodeCAPI_getFormatName(IntPtr M1obj, int fmt);

        [DllImport(libname)]
        internal static extern IntPtr Mach1TranscodeCAPI_getAllFormatNames(IntPtr M1obj);

        [DllImport(libname)]
        internal static extern int Mach1TranscodeCAPI_getFormatsCount(IntPtr M1obj);

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
        internal static extern void Mach1TranscodeCAPI_setInputFormatCustomPointsJson(IntPtr M1obj, string inJson);

        [DllImport(libname)]
        internal static extern void Mach1TranscodeCAPI_setInputFormatCustomPoints(IntPtr M1obj, IntPtr points, int count);

        [DllImport(libname)]
        internal static extern void Mach1TranscodeCAPI_setOutputFormat(IntPtr M1obj, int outFmt);

        [DllImport(libname)]
        internal static extern void Mach1TranscodeCAPI_setOutputFormatCustomPointsJson(IntPtr M1obj, string outJson);

        [DllImport(libname)]
        internal static extern void Mach1TranscodeCAPI_setOutputFormatCustomPoints(IntPtr M1obj, IntPtr points, int count);

        [DllImport(libname)]
        internal static extern void Mach1TranscodeCAPI_setCustomPointsSamplerCallback(IntPtr M1obj, IntPtr callback);

        [DllImport(libname)]
        internal static extern bool Mach1TranscodeCAPI_processConversionPath(IntPtr M1obj);

        [DllImport(libname)]
        internal static extern void Mach1TranscodeCAPI_getMatrixConversion(IntPtr M1obj, IntPtr matrix);

        [DllImport(libname)]
        internal static extern void Mach1TranscodeCAPI_processConversion(IntPtr M1obj, IntPtr inBufs, IntPtr outBufs, int numSamples);

        [DllImport(libname)]
        internal static extern IntPtr Mach1TranscodeCAPI_getFormatConversionPath(IntPtr M1obj, IntPtr count);

        internal IntPtr M1obj;

        public Mach1Transcode()
        {
            M1obj = Mach1TranscodeCAPI_create();
        }

        public void Dispose()
        {
            Mach1TranscodeCAPI_delete(M1obj);
        }

        public int getInputFormat()
        {
            return Mach1TranscodeCAPI_getInputFormat(M1obj);
            /// Returns the current input format
        }

        public int getOutputFormat()
        {
            return Mach1TranscodeCAPI_getOutputFormat(M1obj);
            /// Returns the current output format
        }

        public int getInputNumChannels()
        {
            return Mach1TranscodeCAPI_getInputNumChannels(M1obj);
            /// Returns the number of channels for the current input format
        }

        public int getOutputNumChannels()
        {
            return Mach1TranscodeCAPI_getOutputNumChannels(M1obj);
            /// Returns the number of channels for the current output format
        }

        public int getFormatFromString(string str)
        {
            return Mach1TranscodeCAPI_getFormatFromString(M1obj, str);
            /// Returns the format ID from a format name string
        }

        public string getFormatName(int fmt)
        {
            return Marshal.PtrToStringAnsi(Mach1TranscodeCAPI_getFormatName(M1obj, fmt));
            /// Returns the name of a format from its ID
        }

        public string[] getAllFormatNames()
        {
            int count = getFormatsCount();
            string[] formatNames = new string[count];
            IntPtr namesPtr = Mach1TranscodeCAPI_getAllFormatNames(M1obj);
            
            for (int i = 0; i < count; i++)
            {
                IntPtr strPtr = Marshal.ReadIntPtr(namesPtr, i * IntPtr.Size);
                formatNames[i] = Marshal.PtrToStringAnsi(strPtr);
            }
            
            return formatNames;
            /// Returns all available format names
        }

        public int getFormatsCount()
        {
            return Mach1TranscodeCAPI_getFormatsCount(M1obj);
            /// Returns the count of all available formats
        }

        public float processNormalization(float[][] bufs, int numSamples)
        {
            // This implementation is a simplified version - would need to properly marshal the 2D array
            IntPtr bufsPtr = Marshal.AllocHGlobal(bufs.Length * IntPtr.Size);
            
            try
            {
                for (int i = 0; i < bufs.Length; i++)
                {
                    IntPtr bufPtr = Marshal.AllocHGlobal(numSamples * sizeof(float));
                    Marshal.Copy(bufs[i], 0, bufPtr, numSamples);
                    Marshal.WriteIntPtr(bufsPtr, i * IntPtr.Size, bufPtr);
                }
                
                float result = Mach1TranscodeCAPI_processNormalization(M1obj, bufsPtr, numSamples);
                
                return result;
            }
            finally
            {
                for (int i = 0; i < bufs.Length; i++)
                {
                    IntPtr bufPtr = Marshal.ReadIntPtr(bufsPtr, i * IntPtr.Size);
                    Marshal.FreeHGlobal(bufPtr);
                }
                Marshal.FreeHGlobal(bufsPtr);
            }
            /// Process normalization on buffer
        }

        public void processMasterGain(float[][] bufs, int numSamples, float masterGain)
        {
            // This implementation is a simplified version - would need to properly marshal the 2D array
            IntPtr bufsPtr = Marshal.AllocHGlobal(bufs.Length * IntPtr.Size);
            
            try
            {
                for (int i = 0; i < bufs.Length; i++)
                {
                    IntPtr bufPtr = Marshal.AllocHGlobal(numSamples * sizeof(float));
                    Marshal.Copy(bufs[i], 0, bufPtr, numSamples);
                    Marshal.WriteIntPtr(bufsPtr, i * IntPtr.Size, bufPtr);
                }
                
                Mach1TranscodeCAPI_processMasterGain(M1obj, bufsPtr, numSamples, masterGain);
                
                // Copy back the processed data
                for (int i = 0; i < bufs.Length; i++)
                {
                    IntPtr bufPtr = Marshal.ReadIntPtr(bufsPtr, i * IntPtr.Size);
                    Marshal.Copy(bufPtr, bufs[i], 0, numSamples);
                }
            }
            finally
            {
                for (int i = 0; i < bufs.Length; i++)
                {
                    IntPtr bufPtr = Marshal.ReadIntPtr(bufsPtr, i * IntPtr.Size);
                    Marshal.FreeHGlobal(bufPtr);
                }
                Marshal.FreeHGlobal(bufsPtr);
            }
            /// Apply master gain to buffer
        }

        public float db2level(float db)
        {
            return Mach1TranscodeCAPI_db2level(M1obj, db);
            /// Convert decibels to linear level
        }

        public float level2db(float level)
        {
            return Mach1TranscodeCAPI_level2db(M1obj, level);
            /// Convert linear level to decibels
        }

        public void setLFESub(int[] subChannelIndices, int numChannels, int sampleRate)
        {
            IntPtr indicesPtr = Marshal.AllocHGlobal(numChannels * sizeof(int));
            try
            {
                Marshal.Copy(subChannelIndices, 0, indicesPtr, numChannels);
                Mach1TranscodeCAPI_setLFESub(M1obj, indicesPtr, numChannels, sampleRate);
            }
            finally
            {
                Marshal.FreeHGlobal(indicesPtr);
            }
            /// Set LFE sub settings
        }

        public void setSpatialDownmixer(float corrThreshold)
        {
            Mach1TranscodeCAPI_setSpatialDownmixer(M1obj, corrThreshold);
            /// Set spatial downmixer with correlation threshold
        }

        public bool getSpatialDownmixerPossibility()
        {
            return Mach1TranscodeCAPI_getSpatialDownmixerPossibility(M1obj);
            /// Returns whether spatial downmixing is possible
        }

        public float[] getAvgSamplesDiff()
        {
            IntPtr diffPtr = Mach1TranscodeCAPI_getAvgSamplesDiff(M1obj);
            int count = getInputNumChannels() * getOutputNumChannels();
            float[] result = new float[count];
            Marshal.Copy(diffPtr, result, 0, count);
            return result;
            /// Returns the average samples difference
        }

        public void setInputFormat(int inFmt)
        {
            Mach1TranscodeCAPI_setInputFormat(M1obj, inFmt);
            /// Set the input format using format ID
        }

        public void setInputFormatCustomPointsJson(string inJson)
        {
            Mach1TranscodeCAPI_setInputFormatCustomPointsJson(M1obj, inJson);
            /// Set the input format using custom points in JSON format
        }

        public void setInputFormatCustomPoints(Mach1Point3D[] points)
        {
            int size = Marshal.SizeOf<Mach1Point3D>();
            IntPtr pointsPtr = Marshal.AllocHGlobal(points.Length * size);
            
            try
            {
                for (int i = 0; i < points.Length; i++)
                {
                    IntPtr elemPtr = new IntPtr(pointsPtr.ToInt64() + i * size);
                    Marshal.StructureToPtr(points[i], elemPtr, false);
                }
                
                Mach1TranscodeCAPI_setInputFormatCustomPoints(M1obj, pointsPtr, points.Length);
            }
            finally
            {
                Marshal.FreeHGlobal(pointsPtr);
            }
            /// Set the input format using custom points
        }

        public void setOutputFormat(int outFmt)
        {
            Mach1TranscodeCAPI_setOutputFormat(M1obj, outFmt);
            /// Set the output format using format ID
        }

        public void setOutputFormatCustomPointsJson(string outJson)
        {
            Mach1TranscodeCAPI_setOutputFormatCustomPointsJson(M1obj, outJson);
            /// Set the output format using custom points in JSON format
        }

        public void setOutputFormatCustomPoints(Mach1Point3D[] points)
        {
            int size = Marshal.SizeOf<Mach1Point3D>();
            IntPtr pointsPtr = Marshal.AllocHGlobal(points.Length * size);
            
            try
            {
                for (int i = 0; i < points.Length; i++)
                {
                    IntPtr elemPtr = new IntPtr(pointsPtr.ToInt64() + i * size);
                    Marshal.StructureToPtr(points[i], elemPtr, false);
                }
                
                Mach1TranscodeCAPI_setOutputFormatCustomPoints(M1obj, pointsPtr, points.Length);
            }
            finally
            {
                Marshal.FreeHGlobal(pointsPtr);
            }
            /// Set the output format using custom points
        }

        // Note: Custom points sampler callback implementation would be more complex
        // This is a placeholder that would need to be expanded based on usage requirements
        public void setCustomPointsSamplerCallback(Func<long, int, Mach1Point3D[]> callback)
        {
            // Implementation would depend on callback requirements
            /// Set custom points sampler callback
        }

        public bool processConversionPath()
        {
            return Mach1TranscodeCAPI_processConversionPath(M1obj);
            /// Process the conversion path and return whether it was successful
        }

        public void getMatrixConversion(ref float[] matrix)
        {
            IntPtr matrixPtr = Marshal.AllocHGlobal(matrix.Length * sizeof(float));
            try
            {
                Mach1TranscodeCAPI_getMatrixConversion(M1obj, matrixPtr);
                Marshal.Copy(matrixPtr, matrix, 0, matrix.Length);
            }
            finally
            {
                Marshal.FreeHGlobal(matrixPtr);
            }
            /// Get the conversion matrix
        }

        public void processConversion(float[][] inBufs, float[][] outBufs, int numSamples)
        {
            IntPtr inBufsPtr = Marshal.AllocHGlobal(inBufs.Length * IntPtr.Size);
            IntPtr outBufsPtr = Marshal.AllocHGlobal(outBufs.Length * IntPtr.Size);
            
            try
            {
                // Setup input buffers
                for (int i = 0; i < inBufs.Length; i++)
                {
                    IntPtr bufPtr = Marshal.AllocHGlobal(numSamples * sizeof(float));
                    Marshal.Copy(inBufs[i], 0, bufPtr, numSamples);
                    Marshal.WriteIntPtr(inBufsPtr, i * IntPtr.Size, bufPtr);
                }
                
                // Setup output buffers
                for (int i = 0; i < outBufs.Length; i++)
                {
                    IntPtr bufPtr = Marshal.AllocHGlobal(numSamples * sizeof(float));
                    Marshal.WriteIntPtr(outBufsPtr, i * IntPtr.Size, bufPtr);
                }
                
                Mach1TranscodeCAPI_processConversion(M1obj, inBufsPtr, outBufsPtr, numSamples);
                
                // Copy back the processed data
                for (int i = 0; i < outBufs.Length; i++)
                {
                    IntPtr bufPtr = Marshal.ReadIntPtr(outBufsPtr, i * IntPtr.Size);
                    Marshal.Copy(bufPtr, outBufs[i], 0, numSamples);
                }
            }
            finally
            {
                // Free input buffers
                for (int i = 0; i < inBufs.Length; i++)
                {
                    IntPtr bufPtr = Marshal.ReadIntPtr(inBufsPtr, i * IntPtr.Size);
                    Marshal.FreeHGlobal(bufPtr);
                }
                Marshal.FreeHGlobal(inBufsPtr);
                
                // Free output buffers
                for (int i = 0; i < outBufs.Length; i++)
                {
                    IntPtr bufPtr = Marshal.ReadIntPtr(outBufsPtr, i * IntPtr.Size);
                    Marshal.FreeHGlobal(bufPtr);
                }
                Marshal.FreeHGlobal(outBufsPtr);
            }
            /// Process the format conversion
        }

        public int[] getFormatConversionPath()
        {
            IntPtr countPtr = Marshal.AllocHGlobal(sizeof(int));
            try
            {
                IntPtr pathPtr = Mach1TranscodeCAPI_getFormatConversionPath(M1obj, countPtr);
                int count = Marshal.ReadInt32(countPtr);
                
                int[] path = new int[count];
                Marshal.Copy(pathPtr, path, 0, count);
                
                return path;
            }
            finally
            {
                Marshal.FreeHGlobal(countPtr);
            }
            /// Get the format conversion path
        }
    }
}
