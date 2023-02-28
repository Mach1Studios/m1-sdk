//  Mach1 Spatial SDK
//  Copyright © 2017 Mach1, Corp. All rights reserved.

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
        internal static extern int Mach1TranscodeCAPI_getInputNumChannels(IntPtr M1obj);

        [DllImport(libname)]
        internal static extern int Mach1TranscodeCAPI_getOutputNumChannels(IntPtr M1obj);

        // [DllImport(libname)]
        // internal static extern int Mach1TranscodeCAPI_getFormatFromString(IntPtr M1obj, char str);
       
        // [DllImport(libname)]
        // internal static extern float Mach1TranscodeCAPI_processNormalization(IntPtr M1obj, IntPtr bufs, int numSamples);

        // [DllImport(libname)]
        // internal static extern void Mach1TranscodeCAPI_processMasterGain(IntPtr M1obj, InPtr bufs, int numSamples, float masterGain);

        [DllImport(libname)]
        internal static extern float Mach1TranscodeCAPI_db2level(IntPtr M1obj, float db);

        [DllImport(libname)]
        internal static extern float Mach1TranscodeCAPI_level2db(IntPtr M1obj, float level);


        internal IntPtr M1obj;

        public Mach1Transcode()
        {
            M1obj = Mach1TranscodeCAPI_create();
        }

        public void Dispose()
        {
            Mach1TranscodeCAPI_delete(M1obj);
        }

        
    }
}
